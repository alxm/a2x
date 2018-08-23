/*
    Copyright 2010, 2016-2018 Alex Margarit

    This file is part of a2x-framework.

    a2x-framework is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    a2x-framework is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with a2x-framework.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "a2x_system_includes.h"
#include "a2x_pack_sound.v.h"

#include "a2x_pack_draw.v.h"
#include "a2x_pack_embed.v.h"
#include "a2x_pack_file.v.h"
#include "a2x_pack_input.v.h"
#include "a2x_pack_input_button.v.h"
#include "a2x_pack_math.v.h"
#include "a2x_pack_mem.v.h"
#include "a2x_pack_platform.v.h"
#include "a2x_pack_screen.v.h"
#include "a2x_pack_settings.v.h"
#include "a2x_pack_timer.v.h"

struct AMusic {
    APlatformMusic* platformMusic;
};

struct ASfx {
    APlatformSfx* platformSfx;
    int channel;
};

static bool g_soundOn;
static int g_volume;
static int g_musicVolume;
static int g_sfxVolume;
static int g_volumeMax;

#if A_PLATFORM_SYSTEM_GP2X || A_PLATFORM_SYSTEM_WIZ
    #define A_VOLUME_STEP 1
    #define A_VOLBAR_SHOW_MS 500
    static ATimer* g_volTimer;
    static AInputButton* g_volumeUpButton;
    static AInputButton* g_volumeDownButton;
    static APixel g_volbarBackground;
    static APixel g_volbarBorder;
    static APixel g_volbarFill;
#endif

#if A_DEVICE_HAS_KEYBOARD
    static AInputButton* g_musicOnOffButton;
#endif

static void adjustSoundVolume(int Volume)
{
    g_volume = a_math_clamp(Volume, 0, g_volumeMax);
    g_musicVolume = a_settings_getInt("sound.music.scale") * g_volume / 100;
    g_sfxVolume = a_settings_getInt("sound.sfx.scale") * g_volume / 100;

    a_platform__sfxVolumeSetAll(g_sfxVolume);
    a_platform__musicVolumeSet(g_musicVolume);
}

static void inputCallback(void)
{
    #if A_PLATFORM_SYSTEM_GP2X || A_PLATFORM_SYSTEM_WIZ
        if(!g_soundOn) {
            return;
        }

        int adjust = 0;

        if(a_button_pressGet(g_volumeUpButton)) {
            adjust = A_VOLUME_STEP;
        } else if(a_button_pressGet(g_volumeDownButton)) {
            adjust = -A_VOLUME_STEP;
        }

        if(adjust) {
            adjustSoundVolume(g_volume + adjust);
            a_timer_start(g_volTimer);
        }
    #endif

    #if A_DEVICE_HAS_KEYBOARD
        if(g_soundOn && a_button_pressGetOnce(g_musicOnOffButton)) {
            a_platform__musicToggle();
        }
    #endif
}

#if A_PLATFORM_SYSTEM_GP2X || A_PLATFORM_SYSTEM_WIZ
    static void screenCallback(void)
    {
        if(!g_soundOn
            || !a_timer_isRunning(g_volTimer)
            || a_timer_isExpired(g_volTimer)) {

            return;
        }

        a_pixel_blendSet(A_PIXEL_BLEND_PLAIN);

        a_pixel_colorSetPixel(g_volbarBackground);
        a_draw_rectangle(0, 181, g_volumeMax / A_VOLUME_STEP + 5, 16);

        a_pixel_colorSetPixel(g_volbarBorder);
        a_draw_hline(0, g_volumeMax / A_VOLUME_STEP + 4, 180);
        a_draw_hline(0, g_volumeMax / A_VOLUME_STEP + 4, 183 + 14);
        a_draw_vline(g_volumeMax / A_VOLUME_STEP + 4 + 1, 181, 183 + 13);

        a_pixel_colorSetPixel(g_volbarFill);
        a_draw_rectangle(0, 186, g_volume / A_VOLUME_STEP, 6);
    }
#endif

void a_sound__init(void)
{
    g_soundOn = a_settings_getBool("sound.on");

    if(!g_soundOn) {
        return;
    }

    g_volumeMax = a_platform__volumeGetMax();

    #if A_PLATFORM_SYSTEM_GP2X || A_PLATFORM_SYSTEM_WIZ
        adjustSoundVolume(g_volumeMax / 16);
        g_volTimer = a_timer_new(A_TIMER_MS, A_VOLBAR_SHOW_MS, false);
    #else
        adjustSoundVolume(g_volumeMax);
    #endif

    #if A_PLATFORM_SYSTEM_GP2X || A_PLATFORM_SYSTEM_WIZ
        g_volumeUpButton = a_button_new("gamepad.b.volUp");
        g_volumeDownButton = a_button_new("gamepad.b.volDown");
    #endif

    #if A_DEVICE_HAS_KEYBOARD
        g_musicOnOffButton = a_button_new("key.m");
    #endif

    a_input__callbackAdd(inputCallback);

    #if A_PLATFORM_SYSTEM_GP2X || A_PLATFORM_SYSTEM_WIZ
        const char* color;

        color = a_settings_getString("sound.volbar.background");
        g_volbarBackground = a_pixel_fromHex((uint32_t)strtol(color, NULL, 16));

        color = a_settings_getString("sound.volbar.border");
        g_volbarBorder = a_pixel_fromHex((uint32_t)strtol(color, NULL, 16));

        color = a_settings_getString("sound.volbar.fill");
        g_volbarFill = a_pixel_fromHex((uint32_t)strtol(color, NULL, 16));

        a_screen__callbackAdd(screenCallback);
    #endif
}

void a_sound__uninit(void)
{
    if(!g_soundOn) {
        return;
    }

    #if A_PLATFORM_SYSTEM_GP2X || A_PLATFORM_SYSTEM_WIZ
        a_timer_free(g_volTimer);
    #endif

    #if A_DEVICE_HAS_KEYBOARD
        a_button_free(g_musicOnOffButton);
    #endif

    a_music_stop();
}

AMusic* a_music_new(const char* Path)
{
    if(g_soundOn) {
        APlatformMusic* music = a_platform__musicNew(Path);

        if(music) {
            AMusic* m = a_mem_malloc(sizeof(AMusic));

            m->platformMusic = music;

            return m;
        }
    }

    return NULL;
}

void a_music_free(AMusic* Music)
{
    if(g_soundOn) {
        a_platform__musicFree(Music->platformMusic);
        free(Music);
    }
}

void a_music_play(const AMusic* Music)
{
    if(g_soundOn) {
        a_platform__musicPlay(Music->platformMusic);
    }
}

void a_music_stop(void)
{
    if(g_soundOn) {
        a_platform__musicStop();
    }
}

ASfx* a_sfx_new(const char* Path)
{
    if(g_soundOn) {
        APlatformSfx* sfx = NULL;

        if(a_file_exists(Path)) {
            sfx = a_platform__sfxNewFromFile(Path);
        } else {
            const uint8_t* data;
            size_t size;

            if(a_embed__get(Path, &data, &size)) {
                sfx = a_platform__sfxNewFromData(data, (int)size);
            }
        }

        if(sfx) {
            ASfx* s = a_mem_malloc(sizeof(ASfx));

            s->platformSfx = sfx;
            s->channel = a_platform__sfxChannelGet();

            return s;
        }
    }

    return NULL;
}

ASfx* a_sfx_dup(const ASfx* Sfx)
{
    if(!g_soundOn) {
        return NULL;
    }

    ASfx* s = a_mem_dup(Sfx, sizeof(ASfx));

    a_platform__sfxRef(s->platformSfx);
    s->channel = a_platform__sfxChannelGet();

    return s;
}

void a_sfx_free(ASfx* Sfx)
{
    if(g_soundOn) {
        a_platform__sfxFree(Sfx->platformSfx);
        free(Sfx);
    }
}

int a_channel_new(void)
{
    return a_platform__sfxChannelGet();
}

void a_channel_play(int Channel, const ASfx* Sfx, AChannelFlags Flags)
{
    if(!g_soundOn) {
        return;
    }

    if(Flags & A_CHANNEL_RESTART) {
        a_platform__sfxStop(Channel);
    } else if((Flags & A_CHANNEL_YIELD) && a_platform__sfxIsPlaying(Channel)) {
        return;
    }

    a_platform__sfxPlay(Sfx->platformSfx, Channel, Flags & A_CHANNEL_LOOP);
}

void a_channel_stop(int Channel)
{
    if(g_soundOn) {
        a_platform__sfxStop(Channel);
    }
}

bool a_channel_isPlaying(int Channel)
{
    return g_soundOn && a_platform__sfxIsPlaying(Channel);
}
