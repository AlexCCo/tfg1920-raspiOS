#ifndef _VIEW_H
#define _VIEW_H

#include <utils/color.h>
#include <utils/list.h>

typedef enum TEXT_ALIGN {
    CENTER,
    LEFT,
    RIGHT
} TEXT_ALIGN;

typedef struct VIEW {
    int width;
    int height;
    int x;
    int y;
    int fontSize;
    color_24 bgColor;
    color_24 textColor;
    DEFINE_LINK(VIEW);

    char* text;
    TEXT_ALIGN textAlign;
    int textLines;
} VIEW;

void new_view(VIEW* v, int width, int height, int x, int y);
void draw(VIEW* v);

#endif /* _VIEW_H */