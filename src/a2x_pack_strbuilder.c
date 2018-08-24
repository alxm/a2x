/*
    Copyright 2016-2018 Alex Margarit

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
#include "a2x_pack_strbuilder.v.h"

#include "a2x_pack_mem.v.h"
#include "a2x_pack_out.v.h"

struct AStrBuilder {
    size_t size;
    size_t index;
    char buffer[];
};

AStrBuilder* a_strbuilder_new(size_t Bytes)
{
    if(Bytes == 0) {
        a_out__fatal("a_strbuilder_new: invalid size 0");
    }

    AStrBuilder* b = a_mem_malloc(sizeof(AStrBuilder) + Bytes);

    b->size = Bytes;
    b->index = 0;
    b->buffer[0] = '\0';

    return b;
}

void a_strbuilder_free(AStrBuilder* Builder)
{
    if(Builder == NULL) {
        return;
    }

    free(Builder);
}

const char* a_strbuilder_get(AStrBuilder* Builder)
{
    return Builder->buffer;
}

bool a_strbuilder_add(AStrBuilder* Builder, const char* String)
{
    const size_t limit = Builder->size - 1;
    size_t index = Builder->index;

    while(*String != '\0' && index < limit) {
        Builder->buffer[index] = *String;
        String++;
        index++;
    }

    Builder->buffer[index] = '\0';
    Builder->index = index;

    return *String == '\0';
}

bool a_strbuilder_addf(AStrBuilder* Builder, const char* Format, ...)
{
    va_list args;
    va_start(args, Format);

    int bytesNeeded = vsnprintf(NULL, 0, Format, args) + 1;

    if(bytesNeeded <= 0) {
        return false;
    }

    va_end(args);
    va_start(args, Format);

    bool ret = false;
    char* buffer = a_mem_malloc((size_t)bytesNeeded);

    if(vsnprintf(buffer, (size_t)bytesNeeded, Format, args) >= 0) {
        ret = a_strbuilder_add(Builder, buffer);
    }

    free(buffer);

    va_end(args);

    return ret;
}
