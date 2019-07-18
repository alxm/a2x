/*
    Copyright 2010, 2016-2019 Alex Margarit <alex@alxm.org>
    This file is part of a2x, a C video game framework.

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef A_INC_GRAPHICS_SCREEN_V_H
#define A_INC_GRAPHICS_SCREEN_V_H

#include "graphics/a_screen.p.h"

typedef struct AScreen AScreen;

#include "general/a_main.v.h"
#include "graphics/a_sprite.v.h"

struct AScreen {
    APixels* pixels;
    #if !A_CONFIG_LIB_RENDER_SOFTWARE
        APlatformTexture* texture;
    #endif
    ASprite* sprite;
    unsigned frame;
    int clipX, clipY;
    int clipX2, clipY2;
    int clipWidth, clipHeight;
};

extern const APack a_pack__screen;

extern AScreen a__screen;

extern void a_screen__tick(void);
extern void a_screen__draw(void);

extern void a_screen__toSprite(ASprite* Sprite, unsigned Frame);

static inline APixel* a_screen__bufferGetFrom(int X, int Y)
{
    return a_pixels__bufferGetFrom(a__screen.pixels, a__screen.frame, X, Y);
}

#endif // A_INC_GRAPHICS_SCREEN_V_H
