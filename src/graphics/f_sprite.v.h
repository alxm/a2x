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

#ifndef F_INC_GRAPHICS_SPRITE_V_H
#define F_INC_GRAPHICS_SPRITE_V_H

#include "f_sprite.p.h"

#include "../platform/f_platform.v.h"

extern FPixels* f_sprite__pixelsGet(FSprite* Sprite);
extern FPlatformTexture* f_sprite__textureGet(const FSprite* Sprite, unsigned Frame);
extern void f_sprite__textureCommit(FSprite* Sprite, unsigned Frame);

#endif // F_INC_GRAPHICS_SPRITE_V_H