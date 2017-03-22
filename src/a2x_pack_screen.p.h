/*
    Copyright 2010, 2016 Alex Margarit

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

#pragma once

#include "a2x_system_includes.h"

#include "a2x_pack_pixel.p.h"
#include "a2x_pack_sprite.p.h"

extern APixel* a_screen_pixels(void);
extern int a_screen_width(void);
extern int a_screen_height(void);

extern APixel* a_screen_dup(void);
extern APixel* a_screen_new(void);
extern void a_screen_copy(APixel* Dst, const APixel* Src);
extern void a_screen_copyPart(APixel* Dst, int X, int Y, int Width, int Height);
extern APixel a_screen_getPixel(int X, int Y);

extern void a_screen_setTarget(APixel* Pixels, int Width, int Height);
extern void a_screen_setTargetSprite(ASprite* Sprite);
extern void a_screen_resetTarget(void);

extern void a_screen_setClip(int X, int Y, int Width, int Height);
extern void a_screen_resetClip(void);

extern bool a_screen_boxOnScreen(int X, int Y, int W, int H);
extern bool a_screen_boxInsideScreen(int X, int Y, int W, int H);
extern bool a_screen_boxOnClip(int X, int Y, int W, int H);
extern bool a_screen_boxInsideClip(int X, int Y, int W, int H);
