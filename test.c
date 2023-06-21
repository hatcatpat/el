#include "el.h"

#include <stdio.h>
#include <math.h>
#include <time.h>
#include <cairo/cairo.h>
#include <stdlib.h>

#define MIN(a, b) ((a) < (b) ? (a) : (b))
int width = 600, height = 600;
float x, y, th, amt;

cairo_t *cr;
cairo_surface_t *sf;

void
init()
{
    sf = cairo_image_surface_create(CAIRO_FORMAT_RGB24, width, height);
    cr = cairo_create(sf);

    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_paint(cr);

    cairo_set_source_rgb(cr, 1, 1, 1);
}

void
deinit()
{
    cairo_surface_write_to_png(sf, "output.png");
    cairo_destroy(cr);
    cairo_surface_destroy(sf);
}

void
F(char *output, int pos)
{
    float rad = th * 3.14 / 180;
    x += amt * cos(rad);
    y += amt * sin(rad);
    cairo_line_to(cr, x, y);
}

void
plus(char *output, int pos)
{
    th -= 90;
}

void
minus(char *output, int pos)
{
    th += 90;
}

int
main(int argc, char *argv[])
{
    char *output;
    el_system_t* L = el_create();
    int n = 4;

    srand(time(NULL));

    el_add_symbol(L, "F", F);
    el_add_symbol(L, "+", plus);
    el_add_symbol(L, "-", minus);

    el_add_rule(L, "F", "F+F-F-F+F");

    output = el_perform(L, "F", n);

    init();

    x = 0, y = height, th = 0;
    amt = (float)width / (float)pow(3, n);

    cairo_move_to(cr, x, y);
    el_enact(L, output);
    cairo_stroke(cr);

    deinit();

    el_free(L);

    return 0;
}
