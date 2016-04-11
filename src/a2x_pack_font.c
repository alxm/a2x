/*
    Copyright 2010 Alex Margarit

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

#include "a2x_pack_font.v.h"

#define NUM_ASCII 256

#define FONT_SPACE       1
#define FONT_BLANK_SPACE 3

typedef struct Font {
    Sprite* sprites[NUM_ASCII];
    int maxWidth;
} Font;

static const char chars[] =
    "AaBbCcDdEeFfGgHhIiJjKkLlMmNnOoPpQqRrSsTtUuVvWwXxYyZz"
    "0123456789"
    "_-+=*/\\&$@!?'\"()[]{}.,~:;%^#<>|`";

#define CHARS_NUM (sizeof(chars) / sizeof(char) - 1)

static uint8_t gfx_default_font[];

static List* fontsList;
static Font** fonts;
static int font;
static FontAlign align;
static int x;
static int y;

static int charIndex(char c);

void a_font__init(void)
{
    fontsList = a_list_new();
    fonts = NULL;
    font = 0;
    align = A_FONT_ALIGN_LEFT;
    x = 0;
    y = 0;

    Sprite* const fontSprite = a_sprite_fromData(gfx_default_font);

    Pixel colors[A_FONT_MAX];
    colors[A_FONT_WHITE] = a_pixel_make(255, 255, 255);
    colors[A_FONT_GREEN] = a_pixel_make(0, 255, 0);
    colors[A_FONT_YELLOW] = a_pixel_make(255, 255, 0);
    colors[A_FONT_RED] = a_pixel_make(255, 0, 0);
    colors[A_FONT_BLUE] = a_pixel_make(0, 0, 255);

    a_font_load(fontSprite, 0, 0, 1, A_FONT_LOAD_ALL);

    for(int f = 1; f < A_FONT_MAX; f++) {
        a_font_copy(A_FONT_WHITE, colors[f]);
    }
}

void a_font__uninit(void)
{
    A_LIST_ITERATE(fontsList, Font, f) {
        free(f);
    }
    a_list_free(fontsList);
    free(fonts);
}

int a_font_load(const Sprite* sheet, int x, int y, int zoom, FontLoad loader)
{
    Font* const f = malloc(sizeof(Font));

    for(int i = NUM_ASCII; i--; ) {
        f->sprites[i] = NULL;
    }

    f->maxWidth = 0;

    a_list_addLast(fontsList, f);

    free(fonts);
    fonts = (Font**)a_list_array(fontsList);

    int start = 0;
    int end = CHARS_NUM - 1;

    if(loader & A_FONT_LOAD_ALPHANUMERIC) {
        end = charIndex('9');
    } else if(loader & A_FONT_LOAD_ALPHA) {
        end = charIndex('z');
    } else if(loader & A_FONT_LOAD_NUMERIC) {
        start = charIndex('0');
        end = charIndex('9');
    }

    SpriteFrames* const sf = a_spriteframes_new(sheet, x, y, 0);

    for(int i = start; i <= end; i++) {
        f->sprites[(int)chars[i]] = a_spriteframes_next(sf);

        if(f->sprites[(int)chars[i]]->w > f->maxWidth) {
            f->maxWidth = f->sprites[(int)chars[i]]->w;
        }

        if((loader & A_FONT_LOAD_CAPS) && isalpha(chars[i])) {
            f->sprites[(int)chars[i + 1]] = f->sprites[(int)chars[i]];
            i++;
        }
    }

    a_spriteframes_free(sf);

    return a_list_size(fontsList) - 1;
}

int a_font_copy(int font, Pixel color)
{
    const Font* const src = fonts[font];
    Font* const f = malloc(sizeof(Font));

    for(int i = NUM_ASCII; i--; ) {
        if(src->sprites[i]) {
            f->sprites[i] = a_sprite_clone(src->sprites[i]);

            Sprite* const s = f->sprites[i];
            Pixel* d = s->data;

            for(int j = s->w * s->h; j--; d++) {
                if(*d != A_SPRITE_TRANSPARENT) {
                    *d = color;
                }
            }
        } else {
            f->sprites[i] = NULL;
        }
    }

    f->maxWidth = src->maxWidth;

    a_list_addLast(fontsList, f);

    free(fonts);
    fonts = (Font**)a_list_array(fontsList);

    return a_list_size(fontsList) - 1;
}

void a_font_setFace(int f)
{
    font = f;
}

void a_font_setAlign(FontAlign a)
{
    align = a;
}

void a_font_setCoords(int X, int Y)
{
    x = X;
    y = Y;
}

int a_font_getX(void)
{
    return x;
}

void a_font_text(const char* text)
{
    Font* const f = fonts[font];

    if(align & A_FONT_ALIGN_MIDDLE) {
        x -= a_font_width(text) / 2;
    }

    if(align & A_FONT_ALIGN_RIGHT) {
        x -= a_font_width(text);
    }

    if(align & A_FONT_MONOSPACED) {
        const int maxWidth = f->maxWidth;

        for( ; *text != '\0'; text++) {
            Sprite* spr = f->sprites[(int)*text];

            if(spr) {
                a_blit(spr, x + (maxWidth - spr->w) / 2, y);
                x += maxWidth + FONT_SPACE;
            } else if(*text == ' ') {
                x += maxWidth + FONT_SPACE;
            }
        }
    } else {
        for( ; *text != '\0'; text++) {
            Sprite* spr = f->sprites[(int)*text];

            if(spr) {
                a_blit(spr, x, y);
                x += spr->w + FONT_SPACE;
            } else if(*text == ' ') {
                x += FONT_BLANK_SPACE + FONT_SPACE;
            }
        }
    }
}

void a_font_int(int number)
{
    char s[21];
    sprintf(s, "%d", number);

    a_font_text(s);
}

void a_font_float(float number)
{
    char s[64];
    sprintf(s, "%f", number);

    a_font_text(s);
}

void a_font_double(double number)
{
    char s[64];
    sprintf(s, "%lf", number);

    a_font_text(s);
}

void a_font_char(char ch)
{
    char s[2];
    sprintf(s, "%c", ch);

    a_font_text(s);
}

void a_font_fixed(int width, const char* text)
{
    char* buffer;
    int tally = 0;
    int numChars = 0;
    const int dotsWidth = a_font_width("...");
    Font* const f = fonts[font];

    if(*text == '\0' || width == 0) {
        return;
    }

    if(a_font_width(text) <= width) {
        a_font_text(text);
        return;
    }

    if(dotsWidth > width) {
        return;
    }

    if(align & A_FONT_MONOSPACED) {
        const int maxWidth = f->maxWidth;

        for(int i = 0; text[i] != '\0'; i++) {
            numChars++;

            if(f->sprites[(int)text[i]] || text[i] == ' ') {
                tally += maxWidth;

                if(tally > width) {
                    if(numChars < 3) {
                        return;
                    }

                    tally += FONT_SPACE + dotsWidth;

                    for(int j = i; j >= 0; j--) {
                        numChars--;
                        tally -= FONT_SPACE + maxWidth;

                        if(tally <= width) {
                            goto fits;
                        }
                    }

                    return;
                }

                tally += FONT_SPACE;
            }
        }
    } else {
        for(int i = 0; text[i] != '\0'; i++) {
            Sprite* spr = f->sprites[(int)text[i]];

            numChars++;

            if(spr || text[i] == ' ') {
                tally += spr ? spr->w : FONT_BLANK_SPACE;

                if(tally > width) {
                    if(numChars < 3) {
                        return;
                    }

                    tally += FONT_SPACE + dotsWidth;

                    for(int j = i; j >= 0; j--) {
                        spr = f->sprites[(int)text[j]];

                        numChars--;

                        if(spr) {
                            tally -= FONT_SPACE + spr->w;
                        } else if(text[j] == ' ') {
                            tally -= FONT_SPACE + FONT_BLANK_SPACE;
                        }

                        if(tally <= width) {
                            goto fits;
                        }
                    }

                    return;
                }

                tally += FONT_SPACE;
            }
        }
    }

fits:
    buffer = a_str_dup(text);

    buffer[numChars] = '.';
    buffer[numChars + 1] = '.';
    buffer[numChars + 2] = '.';
    buffer[numChars + 3] = '\0';

    a_font_text(buffer);
}

int a_font_width(const char* text)
{
    int width = 0;
    Font* const f = fonts[font];

    if(*text == '\0') {
        return 0;
    }

    if(align & A_FONT_MONOSPACED) {
        const int maxWidth = f->maxWidth;

        for( ; *text != '\0'; text++) {
            if(f->sprites[(int)*text] || *text == ' ') {
                width += maxWidth + FONT_SPACE;
            }
        }
    } else {
        for( ; *text != '\0'; text++) {
            Sprite* spr = f->sprites[(int)*text];

            if(spr) {
                width += spr->w + FONT_SPACE;
            } else if(*text == ' ') {
                width += FONT_BLANK_SPACE + FONT_SPACE;
            }
        }
    }

    return width - FONT_SPACE;
}

static int charIndex(char c)
{
    for(int i = 0; i < CHARS_NUM; i++) {
        if(chars[i] == c) return i;
    }

    return -1;
}

static uint8_t gfx_default_font[] = {
    0x89,0x50,0x4e,0x47,0x0d,0x0a,0x1a,0x0a,
    0x00,0x00,0x00,0x0d,0x49,0x48,0x44,0x52,
    0x00,0x00,0x01,0xfc,0x00,0x00,0x00,0x09,
    0x08,0x02,0x00,0x00,0x00,0x58,0x18,0x09,
    0x5a,0x00,0x00,0x00,0x09,0x70,0x48,0x59,
    0x73,0x00,0x00,0x0b,0x13,0x00,0x00,0x0b,
    0x13,0x01,0x00,0x9a,0x9c,0x18,0x00,0x00,
    0x00,0x07,0x74,0x49,0x4d,0x45,0x07,0xe0,
    0x01,0x09,0x17,0x2a,0x1c,0xf8,0xfb,0xf2,
    0x8a,0x00,0x00,0x03,0x93,0x49,0x44,0x41,
    0x54,0x68,0xde,0xed,0x59,0xcb,0x92,0xe3,
    0x30,0x08,0xa4,0xab,0xfc,0xff,0xbf,0xdc,
    0x7b,0x99,0xf5,0xe8,0x01,0x0d,0x92,0xe2,
    0x64,0x0e,0xd1,0x61,0x2a,0x23,0x63,0x84,
    0x78,0x36,0x18,0x34,0x1a,0xed,0x67,0xc1,
    0x40,0x10,0xf7,0xff,0x66,0x66,0x20,0x1a,
    0x8a,0xff,0x34,0x46,0x43,0x4f,0x83,0x2d,
    0x3e,0x2d,0x0d,0x0d,0xd6,0xd3,0xdc,0xa7,
    0x09,0x3e,0x2d,0x0d,0x3b,0x01,0x73,0x61,
    0x30,0x1d,0x0d,0xe7,0x94,0xfb,0x91,0x2f,
    0xf0,0xbd,0x33,0x31,0x09,0xd9,0xc2,0x91,
    0xc4,0x17,0xa9,0x22,0x76,0x43,0xf3,0x1a,
    0x43,0x7c,0x96,0x8f,0x79,0x97,0x4a,0x6f,
    0x7d,0xac,0xba,0x97,0x1c,0x34,0xdc,0xa5,
    0x6a,0x1d,0xf6,0xbf,0xe7,0xa7,0xae,0xf6,
    0x38,0xfb,0xba,0x13,0x0e,0x8a,0xa6,0x7e,
    0xd6,0x70,0x85,0x77,0xee,0xd0,0xba,0xf0,
    0x69,0x76,0x76,0xf4,0x69,0xfc,0x8d,0x68,
    0x33,0xc3,0x98,0x34,0xba,0x60,0x1f,0x32,
    0x40,0x94,0x6d,0x6e,0x56,0xcd,0x2d,0xc6,
    0x1f,0xfc,0x39,0xeb,0x7e,0xbd,0x23,0x68,
    0x38,0x74,0x4e,0xc5,0x91,0xad,0x50,0x97,
    0xda,0x34,0x36,0xf7,0xb2,0xab,0xd5,0xa9,
    0x8d,0xf4,0x9d,0x9b,0x24,0x34,0xaf,0xe2,
    0xd3,0xd2,0x08,0xb2,0x99,0x86,0xbf,0x5a,
    0xab,0x0a,0xa3,0x17,0xec,0x36,0x55,0x77,
    0x9c,0xa6,0x1f,0x14,0xb2,0x7a,0x7a,0xe5,
    0x2d,0x2d,0xcf,0x5f,0x33,0x68,0x85,0x0f,
    0x26,0x0b,0x5a,0x59,0xed,0x7f,0x6d,0xe9,
    0xcb,0xce,0xda,0x60,0xf6,0xd4,0x65,0xc8,
    0x72,0x38,0x54,0x08,0xb8,0x62,0xf1,0x3d,
    0xb7,0x34,0x19,0x4d,0x91,0xc5,0xa3,0x57,
    0x98,0xe9,0x73,0xe3,0x46,0xec,0x43,0x5e,
    0x78,0xe6,0xb0,0x83,0xec,0xfa,0x2e,0x67,
    0x5b,0x91,0xa7,0xa8,0xc0,0xa6,0xd2,0x87,
    0x3c,0xcd,0x2e,0xf1,0x4c,0xbd,0xac,0x69,
    0x30,0x39,0x74,0x85,0x8f,0x6d,0x9d,0x55,
    0xcf,0xc5,0xf5,0x83,0xb0,0xe5,0xd0,0x29,
    0x3d,0xcb,0xd2,0x0e,0x32,0x0f,0x96,0x46,
    0xe0,0xcd,0x38,0x50,0x20,0x83,0x04,0xb1,
    0x7a,0x85,0x56,0xf8,0x13,0xc7,0x48,0x4d,
    0x36,0xc7,0x61,0xb1,0xd8,0xcc,0x51,0x64,
    0x9e,0xaa,0xef,0xbf,0xf5,0x83,0xdc,0x92,
    0xcf,0x82,0x45,0x74,0x28,0x6d,0x20,0x83,
    0x4a,0x02,0x7a,0x74,0x71,0x65,0x9f,0x2b,
    0x00,0xcb,0x35,0x6b,0x05,0x1a,0xce,0x4e,
    0x88,0x00,0xae,0x3d,0x84,0x03,0x84,0xd8,
    0xf0,0xdc,0x06,0x5e,0xc8,0x30,0x28,0xff,
    0x6e,0x71,0x82,0x7f,0xf1,0xab,0x8a,0x56,
    0x18,0xab,0xec,0x04,0xe2,0xb1,0xa0,0x8b,
    0x22,0x8d,0x9b,0x79,0x57,0x7d,0x05,0xbb,
    0x30,0x07,0x01,0xfa,0xd8,0x08,0x36,0xc8,
    0x32,0xe0,0xba,0x29,0xce,0x94,0x7c,0x62,
    0x50,0xc6,0x8d,0x11,0x83,0xa4,0x56,0x81,
    0xf0,0x28,0x64,0xdb,0xe7,0x42,0x54,0x48,
    0x88,0x02,0x70,0x1b,0x2a,0xb4,0xc5,0x25,
    0x41,0x23,0x0f,0x7a,0x1a,0xd6,0x78,0x42,
    0x43,0x01,0x06,0x99,0x6e,0x09,0x87,0x32,
    0x83,0x99,0xe6,0x01,0x73,0x16,0x02,0x33,
    0xc5,0x37,0x16,0x43,0x0a,0x78,0x65,0xdb,
    0x24,0x72,0xda,0xc6,0x91,0x5c,0xdc,0xdf,
    0x68,0x95,0x20,0x5d,0xae,0xc5,0x13,0x90,
    0xad,0xb3,0xe7,0xbd,0x57,0x62,0xc5,0xa5,
    0xb1,0x4c,0x7d,0xf4,0xb1,0x94,0xf5,0x74,
    0x86,0x12,0x39,0x22,0x4d,0x19,0x1b,0x97,
    0x5a,0x4d,0x58,0xf5,0xb7,0x1e,0x55,0x69,
    0x11,0x04,0x6d,0xdc,0x57,0x28,0x39,0x2a,
    0x27,0x38,0x6b,0xdd,0x9e,0x80,0xa5,0x88,
    0xdb,0x6a,0x4c,0x62,0xa3,0x00,0xa5,0x75,
    0x12,0xc7,0x8a,0xc2,0x45,0xa1,0x65,0x3c,
    0x0e,0x85,0x9c,0x84,0x58,0xd6,0x2c,0xb2,
    0xec,0x39,0x8c,0x67,0x20,0x91,0xbb,0x6a,
    0x55,0x60,0xe2,0x19,0x8d,0x5c,0xea,0x09,
    0x17,0x6f,0x09,0xbd,0x25,0x54,0x77,0x3e,
    0xcc,0x14,0xa0,0x9e,0x2a,0x36,0x2f,0xc7,
    0xd2,0xe7,0x03,0xf1,0x4a,0x15,0x79,0x22,
    0x43,0x3d,0x97,0x22,0x0f,0xd1,0x22,0xce,
    0xf2,0xfe,0x07,0x33,0x63,0x11,0x03,0xa2,
    0xd6,0x1f,0x6c,0x94,0x5b,0xd6,0x9e,0x46,
    0x68,0x08,0x35,0x58,0x3d,0x67,0x46,0x78,
    0xdf,0x8a,0x36,0x5c,0x4b,0x24,0xf1,0x62,
    0xc1,0x10,0x35,0x40,0x60,0xff,0xf3,0x2a,
    0xc8,0x33,0x0e,0x7b,0x0d,0x71,0xd4,0xc8,
    0xce,0xe3,0x32,0xc4,0xb5,0x0a,0x1f,0x0d,
    0x9f,0x74,0xe8,0xc4,0xb8,0x1e,0x33,0x63,
    0x3b,0x8f,0x34,0x90,0xe1,0x00,0x73,0x93,
    0x7e,0xc5,0x89,0xdd,0x2a,0x1d,0xe5,0x85,
    0x68,0xc7,0x6a,0x1d,0x25,0x25,0x46,0xd8,
    0x4e,0xf1,0xab,0x95,0x8c,0x81,0x36,0x2b,
    0xed,0xb6,0xc9,0x81,0xcc,0xcb,0x8b,0xa2,
    0x86,0x54,0x08,0x42,0xa5,0x92,0x40,0x5d,
    0xa8,0x5b,0x51,0x1a,0x32,0x88,0x97,0xca,
    0x83,0x18,0xe8,0xe9,0xfd,0xb4,0xf4,0xae,
    0x16,0x86,0xd5,0xd2,0x28,0x40,0xa8,0xeb,
    0x3f,0x22,0x53,0x54,0x6c,0xb7,0x67,0x71,
    0x61,0x17,0xec,0x2a,0x41,0x74,0x03,0x36,
    0x8d,0xb6,0xb1,0xa2,0x52,0x04,0xe8,0x15,
    0xf1,0x64,0x7c,0xb5,0xcf,0x1e,0xca,0x7c,
    0xdd,0x2e,0x27,0x4a,0xdb,0xf3,0x31,0x04,
    0x5f,0xf2,0x6b,0x37,0xbd,0xf2,0x79,0x19,
    0xcb,0xdf,0x97,0x52,0xb2,0xf4,0x2d,0x4a,
    0x77,0x59,0x3a,0x1d,0x71,0xfa,0xc6,0xe2,
    0x24,0xb1,0xf6,0x79,0x64,0x99,0x6d,0xd4,
    0x1d,0x17,0x85,0x8c,0xf8,0xbf,0xc4,0x10,
    0xf3,0xdc,0x93,0x19,0xa4,0x82,0xac,0x7f,
    0xe7,0xfe,0x13,0x3d,0x5d,0xfa,0x10,0x8a,
    0x80,0xad,0x9e,0xa5,0x6a,0xf0,0xc1,0x98,
    0x03,0x25,0xa5,0x4e,0x55,0x69,0x36,0xc1,
    0xa2,0x86,0x37,0x76,0xde,0xfc,0x35,0x45,
    0x5f,0x33,0xd2,0xff,0x9e,0xb7,0x17,0x1b,
    0xf4,0x74,0x14,0x16,0xb5,0x62,0xcc,0xd0,
    0xa7,0xf8,0xd2,0xa0,0xbf,0x4b,0xe3,0x5c,
    0xa3,0x7f,0xc1,0xae,0xdf,0xf5,0x5d,0xdf,
    0xf5,0x5d,0xdf,0xf5,0x96,0xf1,0xd3,0x3f,
    0x48,0x87,0x10,0x2d,0x07,0x27,0xcf,0x6e,
    0x00,0x00,0x00,0x00,0x49,0x45,0x4e,0x44,
    0xae,0x42,0x60,0x82,
};
