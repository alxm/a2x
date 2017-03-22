/*
    Copyright 2011, 2016, 2017 Alex Margarit

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

typedef struct ATimer ATimer;

typedef enum {
    A_TIMER_MS,
    A_TIMER_SEC,
    A_TIMER_FRAMES,
    A_TIMER_NUM
} ATimerType;

extern ATimer* a_timer_new(ATimerType Type, unsigned Period);
extern void a_timer_free(ATimer* Timer);

extern void a_timer_start(ATimer* Timer);
extern void a_timer_stop(ATimer* Timer);

extern bool a_timer_running(ATimer* Timer);
extern bool a_timer_expired(ATimer* Timer);

extern unsigned a_timer_elapsed(ATimer* Timer);
extern void a_timer_setPeriod(ATimer* Timer, unsigned Period);
