#include <ui/layouts.h>
#include <utils/stdlib.h>
#include <io/uart.h>

void horizontal_linear_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }

    int widthX = vg->view.width / ccount;
    int x = 0;
    VIEW* node = head_VIEW_list(&vg->children);
    for (int i = 0; i < ccount; i++) {
        node->x = x;
        node->y = 0;
        node->width = widthX;
        node->height = vg->view.height;
        node = next_VIEW_list(node);
        x += widthX;
    }
}

void vertical_linear_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }

    int heightX = vg->view.height / ccount;
    int y = 0;
    VIEW* node = head_VIEW_list(&vg->children);
    for (int i = 0; i < ccount; i++) {
        node->x = 0;
        node->y = y;
        node->width = vg->view.width;
        node->height = heightX;
        node = next_VIEW_list(node);
        y += heightX;
    }
}

void three_columns_layout(void* self) {
    VIEW_GROUP* vg = (VIEW_GROUP*)self;
    int ccount = vg->children.size;
    if (ccount == 0) {
        return;
    }
    int extra = ccount % 3 == 0 ? 0 : 1;
    int its = ccount / 3;
    int width = vg->view.width;
    int height = vg->view.height;
    int width3 = width / 3;
    int heightX = height / (its + extra);

    VIEW* node = head_VIEW_list(&vg->children);
    for (int i = 0; i < its; i++) {
        int x = 0;
        for (int j = 0; j < 3; j++) {
            node->x = x;
            node->y = heightX*i;
            node->width = width3;
            node->height = heightX * (i + 1);
            node = next_VIEW_list(node);
            x += width3;
        }
    }
    
    if (extra == 1) {
        int x = 0;
        for (int i = 0; i < ccount % 3; i++) {
            node->x = x;
            node->y = heightX * its;
            node->width = width3;
            node->height = heightX * (its + 1);
            x += width3;
            node = next_VIEW_list(node);
        }
    }
}