#ifndef GLOBALVAL_H
#define GLOBALVAL_H

#include <gtk/gtk.h>

const char * theme = "\
#draw_btn {\
    font-size: 18px;\
}\
#funcs_input_text_area {\
    font-size: 18px;\
}";


struct coordinate_params {
    double startx;
    double endx;
    double middlex;      /**< 起始的 x 坐标值 */
    double stepx;
    double scalex;         /**< 坐标值到屏幕像素的缩放大小 */
    int x_axis_pos;
    int spacex;
    double starty;
    double middley;      /**< 起始的 x 坐标值 */
    double endy;
    double stepy;
    double scaley;         /**< 坐标值到屏幕像素的缩放大小 */
    int y_axis_pos;
    int spacey;
    int wwidth;
    int wheight;
};

typedef struct global_var {
    GtkWidget *window;
    GtkBuilder *builder;

    /* Surface to store current scribbles */
    cairo_surface_t *surface;

    struct coordinate_params coordinate;
} global_var_t;

struct _coo_point {
    double x;
    double y;
};

typedef struct coo_points {
    int total;
    struct _coo_point *vertexs;
} coo_points_t;

extern global_var_t g_var;

#endif

