/*
    Copyright 2010, 2016-2019 Alex Margarit <alex@alxm.org>
    This file is part of Faur, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License version 3,
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "f_sprite.v.h"
#include <faur.v.h>

struct FSprite {
    FPixels pixels;
    FPlatformTexture* textures[]; // [pixels.framesNum]
};

static FSprite* spriteNew(const FPixels* Pixels, unsigned Frame, int X, int Y, int FrameWidth, int FrameHeight)
{
    FVectorInt gridDim;

    if(X == 0 && Y == 0 && FrameWidth < 1 && FrameHeight < 1) {
        gridDim = (FVectorInt){Pixels->w, Pixels->h};
    } else {
        gridDim = f_pixels__boundsFind(Pixels, Frame, X, Y);
    }

    if(FrameWidth < 1 || FrameHeight < 1) {
        FrameWidth = gridDim.x;
        FrameHeight = gridDim.y;
    }

    unsigned framesNum =
        (unsigned)((gridDim.x / FrameWidth) * (gridDim.y / FrameHeight));

    if(framesNum == 0) {
        F__FATAL("Cannot create a %dx%d sprite from %dx%d @ %d,%d",
                 FrameWidth,
                 FrameHeight,
                 Pixels->w,
                 Pixels->h,
                 X,
                 Y);
    }

    int endX = X + gridDim.x - (gridDim.x % FrameWidth);
    int endY = Y + gridDim.y - (gridDim.y % FrameHeight);

    FSprite* s = f_mem_malloc(sizeof(FSprite)
                                + sizeof(FPlatformTexture*) * framesNum);

    f_pixels__init(
        &s->pixels, FrameWidth, FrameHeight, framesNum, F_PIXELS__ALLOC);

    unsigned f = 0;

    for(int y = Y; y < endY; y += FrameHeight) {
        for(int x = X; x < endX; x += FrameWidth) {
            f_pixels__copyFrameEx(&s->pixels, f, Pixels, Frame, x, y);

            s->textures[f] = f_platform_api__textureNew(&s->pixels, f);

            f++;
        }
    }

    return s;
}

#if F_CONFIG_LIB_PNG
FSprite* f_sprite_newFromPng(const char* Path, int X, int Y, int FrameWidth, int FrameHeight)
{
    FPixels* pixels = f_png__readFile(Path);

    if(pixels == NULL) {
        F__FATAL("f_sprite_newFromPng(%s): Cannot read file", Path);
    }

    if(FrameWidth < 1 || FrameHeight < 1) {
        char* suffix = f_str_suffixGetFromLast(Path, '_');

        if(suffix) {
            int n = sscanf(suffix, "%dx%d", &FrameWidth, &FrameHeight);

            if(n != 2) {
                FrameWidth = 0;
                FrameHeight = 0;
            }

            f_mem_free(suffix);
        }
    }

    FSprite* s = spriteNew(pixels, 0, X, Y, FrameWidth, FrameHeight);

    f_pixels__free(pixels);

    return s;
}
#endif

FSprite* f_sprite_newFromSprite(const FSprite* Sheet, int X, int Y, int FrameWidth, int FrameHeight)
{
    return spriteNew(&Sheet->pixels, 0, X, Y, FrameWidth, FrameHeight);
}

FSprite* f_sprite_newBlank(int Width, int Height, unsigned Frames, bool ColorKeyed)
{
    if(Frames == 0) {
        F__FATAL("f_sprite_newBlank: Frames == 0");
    }

    FSprite* s = f_mem_malloc(sizeof(FSprite)
                                + sizeof(FPlatformTexture*) * Frames);

    f_pixels__init(&s->pixels, Width, Height, Frames, F_PIXELS__ALLOC);

    for(unsigned f = Frames; f--; ) {
        if(ColorKeyed) {
            f_pixels__fill(&s->pixels, f, f_color__key);
        }

        s->textures[f] = f_platform_api__textureNew(&s->pixels, f);
    }

    return s;
}

FSprite* f_sprite_dup(const FSprite* Sprite)
{
    FSprite* s = f_mem_malloc(sizeof(FSprite)
                                + sizeof(FPlatformTexture*)
                                    * Sprite->pixels.framesNum);

    f_pixels__copy(&s->pixels, &Sprite->pixels);

    for(unsigned f = Sprite->pixels.framesNum; f--; ) {
        s->textures[f] = f_platform_api__textureNew(&s->pixels, f);

        #if !F_CONFIG_LIB_RENDER_SOFTWARE
            if(F_FLAGS_TEST_ANY(Sprite->pixels.flags, F_PIXELS__DIRTY)) {
                // The sprite's pixel buffer may be stale if the texture
                // was already set as render target and drawn to
                f_color_push();
                f_screen_push(s, f);

                f_color_reset();
                f_sprite_blit(Sprite, f, 0, 0);

                f_screen_pop();
                f_color_pop();
            }
        #endif
    }

    return s;
}

void f_sprite_free(FSprite* Sprite)
{
    if(Sprite == NULL) {
        return;
    }

    for(unsigned f = Sprite->pixels.framesNum; f--; ) {
        f_platform_api__textureFree(Sprite->textures[f]);
    }

    f_pixels__free(&Sprite->pixels);

    f_mem_free(Sprite);
}

void f_sprite_blit(const FSprite* Sprite, unsigned Frame, int X, int Y)
{
    Frame %= Sprite->pixels.framesNum;

    f_platform_api__textureBlit(
        Sprite->textures[Frame], &Sprite->pixels, Frame, X, Y);
}

void f_sprite_blitEx(const FSprite* Sprite, unsigned Frame, int X, int Y, FFix Scale, unsigned Angle, FFix CenterX, FFix CenterY)
{
    Frame %= Sprite->pixels.framesNum;

    CenterX = f_math_clamp(CenterX, -F_FIX_ONE, F_FIX_ONE);
    CenterY = f_math_clamp(CenterY, -F_FIX_ONE, F_FIX_ONE);

    f_platform_api__textureBlitEx(Sprite->textures[Frame],
                                  &Sprite->pixels,
                                  Frame,
                                  X,
                                  Y,
                                  Scale,
                                  f_fix_angleWrap(Angle),
                                  CenterX,
                                  CenterY);
}

void f_sprite_swapColor(FSprite* Sprite, FPixel OldColor, FPixel NewColor)
{
    for(unsigned f = Sprite->pixels.framesNum; f--; ) {
        FPixel* buffer = f_pixels__bufferGetStart(&Sprite->pixels, f);

        for(unsigned i = Sprite->pixels.bufferLen; i--; ) {
            if(buffer[i] == OldColor) {
                buffer[i] = NewColor;
            }
        }

        #if !F_CONFIG_LIB_RENDER_SOFTWARE
            if(Sprite->textures[f]) {
                f_platform_api__textureFree(Sprite->textures[f]);
            }

            Sprite->textures[f] = f_platform_api__textureNew(
                                    &Sprite->pixels, f);
        #endif
    }
}

void f_sprite_swapColors(FSprite* Sprite, const FPixel* OldColors, const FPixel* NewColors, unsigned NumColors)
{
    for(unsigned f = Sprite->pixels.framesNum; f--; ) {
        FPixel* buffer = f_pixels__bufferGetStart(&Sprite->pixels, f);

        for(unsigned i = Sprite->pixels.bufferLen; i--; ) {
            const FPixel pixel = buffer[i];

            for(unsigned c = NumColors; c--; ) {
                if(pixel == OldColors[c]) {
                    buffer[i] = NewColors[c];
                    break;
                }
            }
        }

        #if !F_CONFIG_LIB_RENDER_SOFTWARE
            if(Sprite->textures[f]) {
                f_platform_api__textureFree(Sprite->textures[f]);
            }

            Sprite->textures[f] = f_platform_api__textureNew(
                                    &Sprite->pixels, f);
        #endif
    }
}

FVectorInt f_sprite_sizeGet(const FSprite* Sprite)
{
    return (FVectorInt){Sprite->pixels.w, Sprite->pixels.h};
}

int f_sprite_sizeGetWidth(const FSprite* Sprite)
{
    return Sprite->pixels.w;
}

int f_sprite_sizeGetHeight(const FSprite* Sprite)
{
    return Sprite->pixels.h;
}

unsigned f_sprite_framesNumGet(const FSprite* Sprite)
{
    return Sprite->pixels.framesNum;
}

FPixels* f_sprite__pixelsGet(FSprite* Sprite)
{
    return &Sprite->pixels;
}

FPlatformTexture* f_sprite__textureGet(const FSprite* Sprite, unsigned Frame)
{
    return Sprite->textures[Frame];
}

void f_sprite__textureCommit(FSprite* Sprite, unsigned Frame)
{
    if(Sprite->textures[Frame]) {
        f_platform_api__textureFree(Sprite->textures[Frame]);
    }

    Sprite->textures[Frame] = f_platform_api__textureNew(
                                &Sprite->pixels, Frame);
}

const FPixel* f_sprite_pixelsGetBuffer(const FSprite* Sprite, unsigned Frame)
{
    return f_pixels__bufferGetStart(&Sprite->pixels, Frame);
}

FPixel f_sprite_pixelsGetValue(const FSprite* Sprite, unsigned Frame, int X, int Y)
{
    return f_pixels__bufferGetValue(&Sprite->pixels, Frame, X, Y);
}
