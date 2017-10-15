/******************************************************************************
 ******************************************************************************
 *
 * @file main.c
 *
 * @author Deran Pan 
 * @date 2017-10-15
 *
 ******************************************************************************
 ******************************************************************************
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "global.h"

// 窗口坐标系和实数坐标系之间的转换
#define PIX2REAL_X(x) (x)/(double)(g_var.coordinate.wwidth) \
                *(g_var.coordinate.endx-g_var.coordinate.startx) \
                +g_var.coordinate.startx
#define PIX2REAL_Y(y) (1-(y)/(double)(g_var.coordinate.wheight)) \
                *(g_var.coordinate.endy-g_var.coordinate.starty) \
                +g_var.coordinate.starty
#define REAL2PIX_X(x) ((x)-g_var.coordinate.startx) \
                /(g_var.coordinate.endx-g_var.coordinate.startx) \
                *g_var.coordinate.wwidth
#define REAL2PIX_Y(y) (1-(((y)-g_var.coordinate.starty)\
                /(g_var.coordinate.endy-g_var.coordinate.starty))) \
                *g_var.coordinate.wheight

/// 表示RGB颜色（0~1）
typedef struct rgb {
    double r;
    double g;
    double b;
} rgb_t;

// 向前声明
static void calculate_coordinate();
static void draw_coordinate(GtkWidget *widget);
void append_output_text(char *s);
void start_btn_begin(GtkButton *button, gpointer user_data);

global_var_t g_var;     /**< 存储一些全局变量 */

rgb_t colors[] = {
{0.11765, 0.56471, 1},
{1,       0.84314, 0},
{0.80392, 0.36078, 0.36078},
{1,       0.07843, 0.57647},
{0.57647, 0.43922, 0.85882},
{0.75294, 1,       0.24314},
{0.5451 , 0.53725, 0.53725},
{0,       0.69804, 0.93333},
{0.32941, 1,       0.62353},
{0.5451 , 0.39608, 0.03137},
{0.37255, 0.61961, 0.62745},
{0.4902 , 0.14902, 0.80392},
{0.1098 , 0.1098 , 0.1098},
{0,       0,       0.80392},
{1,       0.49804, 0.14118},
{0.13333, 0.5451,  0.13333},
{0.93333, 0.77255, 0.56863},
{0.93333, 0.93333, 0},
{0.43137, 0.5451,  0.23922},
{0.73333, 1,       1}    
};                      /**< 预定义颜色，用户绘制不同的分类集   */

/// 新建一个颜色结构体
rgb_t rgb_new(double r, double g, double b)
{
    rgb_t ret;
    ret.r = r;
    ret.g = g;
    ret.b = b;
    return ret;
}

/// 将画布置为空白
static void clear_surface (void)
{
    cairo_t *cr;
    cr = cairo_create (g_var.surface);
    cairo_set_source_rgb (cr, 0.95, 0.95, 0.95);
    cairo_paint (cr);
    cairo_destroy (cr);
}

// 画出所有二维向量的坐标点
/**
 * @param widget 指向DrawingArea控件的指针
 * @param list 存储了所有向量的链表
 */
static void draw_vectors(GtkWidget *widget, GList *list, rgb_t c) 
{
    cairo_t *cr;
    GList *loop;

    cr = cairo_create (g_var.surface);


    for (loop = list; loop != NULL; loop = loop->next) {
        cairo_set_source_rgb(cr, c.r, c.g, c.b);
        cairo_move_to(cr,   REAL2PIX_X(((coo_point_t *)(loop->data))->x),
                            REAL2PIX_Y(((coo_point_t *)(loop->data))->y));
        cairo_arc(cr,   REAL2PIX_X(((coo_point_t *)(loop->data))->x),
                        REAL2PIX_Y(((coo_point_t *)(loop->data))->y),
                        5,
                        0,
                        2*M_PI);
        cairo_fill(cr);
    } 
        
    cairo_destroy (cr);
    gtk_widget_queue_draw (widget);
}

/// 单击 DrawingArea 控件区域的回调函数1
/**
 * 　　参数和返回值解释详见 GTK+ Reference。将鼠标单击处的点加入二维向量列表
 * 并在控件中画出。
 */
static gboolean drawingarea_mouse_press_callbock1(GtkWidget *widget,
                                                    GdkEventButton *event,
                                                    gpointer data)
{
    coo_point_t *vector = NULL;
    char text[128];

    if (event->button == GDK_BUTTON_PRIMARY) {
        vector = coo_point_new(PIX2REAL_X(event->x), PIX2REAL_Y(event->y));   
        g_var.kd->vectors = g_list_append(g_var.kd->vectors, vector);
        draw_coordinate(widget);
        draw_vectors(widget, g_var.kd->vectors, rgb_new(0,0,0));

        memset(text, 0, 128);
        sprintf(text, "X%d(%.4f,%.4f)\n", g_list_length(g_var.kd->vectors),
                PIX2REAL_X(event->x), PIX2REAL_Y(event->y));
        append_output_text(text);
    }

    return TRUE;
}

/// 单击 DrawingArea 控件区域的回调函数
/**
 * 　　将鼠标单击处的点选为初始聚类中心点,并在控件中画出。
 */
static gboolean drawingarea_mouse_press_callbock2(GtkWidget *widget,
                                                    GdkEventButton *event,
                                                    gpointer data)
{
    coo_point_t *vector = NULL;
    char text[128];

    if (event->button == GDK_BUTTON_PRIMARY) {
        vector = coo_point_new(PIX2REAL_X(event->x), PIX2REAL_Y(event->y));   
        g_var.kd->init_gcenters = g_list_append(g_var.kd->init_gcenters, vector);
        draw_coordinate(widget);
        draw_vectors(widget, g_var.kd->init_gcenters, rgb_new(1,0,0));

        memset(text, 0, 128);
        sprintf(text, "Z%d(%.4f,%.4f)\n", g_list_length(g_var.kd->init_gcenters),
                PIX2REAL_X(event->x), PIX2REAL_Y(event->y));
        append_output_text(text);
    }
    return TRUE;
}

/// 计算坐标系相关参数
/**
 * @param widget 指向画布控件
 * @param start_x 起始 x 值
 * @param end_x 结束 x 值
 * @param start_y 起始 y 值
 * @param end_y 结束 y 值
 */
static void _calculate_coordinate(GtkWidget *widget, double start_x, double end_x,
                                    double start_y, double end_y)
{
    // 计算坐标系参数
    g_var.coordinate.spacex = 50;
    g_var.coordinate.spacey = 50;
    
    g_var.coordinate.wwidth = gtk_widget_get_allocated_width (widget);
    g_var.coordinate.wheight = gtk_widget_get_allocated_height (widget);

    g_var.coordinate.startx = start_x;
    g_var.coordinate.endx = end_x;
    g_var.coordinate.middlex = start_x + (end_x - start_x) * 0.5;
    g_var.coordinate.stepx = (end_x - start_x)
                    / (g_var.coordinate.wwidth / g_var.coordinate.spacex);
    g_var.coordinate.scalex = g_var.coordinate.wwidth 
                    / (g_var.coordinate.endx - g_var.coordinate.startx);

    g_var.coordinate.starty = start_y;
    g_var.coordinate.endy = end_y;
    g_var.coordinate.middley = start_y + (end_y - start_y) * 0.5;
    g_var.coordinate.stepy = (end_y - start_y)
                    / (g_var.coordinate.wheight / g_var.coordinate.spacey);
    g_var.coordinate.scaley = g_var.coordinate.wheight 
                    / (g_var.coordinate.endy - g_var.coordinate.starty);

    // 计算原点坐标位置
    if (g_var.coordinate.startx*g_var.coordinate.endx < 0) {
        g_var.coordinate.origin_vertex.x = -g_var.coordinate.startx
                    / (g_var.coordinate.endx - g_var.coordinate.startx);
    }
    else if (g_var.coordinate.startx >= 0) {
        g_var.coordinate.origin_vertex.x = 0;
    }
    else {
        g_var.coordinate.origin_vertex.x = 1;
    }
    if (g_var.coordinate.starty*g_var.coordinate.endy < 0) {
        g_var.coordinate.origin_vertex.y = -g_var.coordinate.starty
                    / (g_var.coordinate.endy - g_var.coordinate.starty);
        g_var.coordinate.origin_vertex.y = 1 - g_var.coordinate.origin_vertex.y;
    }
    else if (g_var.coordinate.starty >= 0) {
        g_var.coordinate.origin_vertex.y = 1;
    }
    else {
        g_var.coordinate.origin_vertex.y = 0;
    }
}

/// 计算坐标系相关参数
static void calculate_coordinate(void) 
{
    GtkWidget *drawing_area;
    char *funcs_strs = NULL;
    double startx, endx;

    // 寻找 TextView 和 DrawArea
    drawing_area = GTK_WIDGET(gtk_builder_get_object(g_var.builder,
                                                     "drawingarea"));
    
    clear_surface();
    _calculate_coordinate(drawing_area,
            startx=atof(gtk_entry_get_text(GTK_ENTRY(
                    gtk_builder_get_object(g_var.builder, "startx_entry")))),
            endx=atof(gtk_entry_get_text(GTK_ENTRY(
                    gtk_builder_get_object(g_var.builder, "endx_entry")))),
            atof(gtk_entry_get_text(GTK_ENTRY(
                    gtk_builder_get_object(g_var.builder, "starty_entry")))),
            atof(gtk_entry_get_text(GTK_ENTRY(
                    gtk_builder_get_object(g_var.builder, "endy_entry")))));

    // 释放内存资源
    free(funcs_strs);
}

/// 绘制坐标系
static void draw_coordinate(GtkWidget *widget)
{
    cairo_t *cr;
    int i;
    double loop;

    // 绘制
    cr = cairo_create (g_var.surface);
    cairo_set_source_rgb(cr, 0.61176, 0.61176, 0.61176);

    // 绘制x轴
    cairo_move_to(cr, 0,
            g_var.coordinate.origin_vertex.y*g_var.coordinate.wheight);
    cairo_line_to(cr, 
            g_var.coordinate.wwidth, 
            g_var.coordinate.origin_vertex.y*g_var.coordinate.wheight);

    for (i=0;
            (loop=g_var.coordinate.origin_vertex.x 
             + i*g_var.coordinate.stepx 
             / (g_var.coordinate.endx - g_var.coordinate.startx)) <= 1.0;
            i++) {
        cairo_move_to(cr, 
                loop*g_var.coordinate.wwidth, 
                g_var.coordinate.origin_vertex.y*g_var.coordinate.wheight+3);
        cairo_line_to(cr, 
                loop*g_var.coordinate.wwidth, 
                g_var.coordinate.origin_vertex.y*g_var.coordinate.wheight-5);
    }
    for (i=0;
            (loop=g_var.coordinate.origin_vertex.x 
             - i*g_var.coordinate.stepx 
             / (g_var.coordinate.endx - g_var.coordinate.startx)) >= 0.0;
            i++) {
        cairo_move_to(cr, 
                loop*g_var.coordinate.wwidth, 
                g_var.coordinate.origin_vertex.y*g_var.coordinate.wheight+3);
        cairo_line_to(cr, 
                loop*g_var.coordinate.wwidth, 
                g_var.coordinate.origin_vertex.y*g_var.coordinate.wheight-5);
    }

    // 绘制y轴
    cairo_move_to(cr, 
            g_var.coordinate.origin_vertex.x*g_var.coordinate.wwidth,
            0);
    cairo_line_to(cr, 
            g_var.coordinate.origin_vertex.x*g_var.coordinate.wwidth,
            g_var.coordinate.wheight);
    for (i=0;
            (loop=g_var.coordinate.origin_vertex.y 
             + i*g_var.coordinate.stepy
             / (g_var.coordinate.endy-g_var.coordinate.starty)) <= 1.0;
            i++) {
        cairo_move_to(cr, 
                g_var.coordinate.origin_vertex.x*g_var.coordinate.wwidth-3, 
                loop*g_var.coordinate.wheight);
        cairo_line_to(cr, 
                g_var.coordinate.origin_vertex.x*g_var.coordinate.wwidth+5, 
                loop*g_var.coordinate.wheight);
    }
    for (i=0;
            (loop=g_var.coordinate.origin_vertex.y 
             - i*g_var.coordinate.stepy 
             / (g_var.coordinate.endy-g_var.coordinate.starty)) >= 0.0;
            i++) {
        cairo_move_to(cr, 
                g_var.coordinate.origin_vertex.x*g_var.coordinate.wwidth-3, 
                loop*g_var.coordinate.wheight);
        cairo_line_to(cr, 
                g_var.coordinate.origin_vertex.x*g_var.coordinate.wwidth+5, 
                loop*g_var.coordinate.wheight);
    }

    cairo_stroke(cr);
    cairo_destroy (cr);
    gtk_widget_queue_draw (widget);
}

/// 重绘图像
/**
 *     当空间需要重绘时将会触发该信号，此时将 surface 中内容画出即可。
 */
static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{
    cairo_set_source_surface (cr, g_var.surface, 0, 0);
    cairo_paint (cr);
    return FALSE;
}

/// 创建一个新的合适尺寸的画布
/**
 *     该函数关联至 drawing_area 的 configure-event 事件。当 drawing_area 创建
 * 尺寸大小改变都将触发此事件。即当改变窗口大小时都将重新创建适合新窗口尺寸的
 * 画布。
 */
static gboolean configure_event_cb (GtkWidget *widget, 
                                    GdkEventConfigure *event, 
                                    gpointer data)
{
    if (g_var.surface)
        cairo_surface_destroy (g_var.surface);
    
    g_var.surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
                                    CAIRO_CONTENT_COLOR,
                                    gtk_widget_get_allocated_width (widget),
                                    gtk_widget_get_allocated_height (widget));
    
    /* Initialize the surface to white */
    clear_surface ();
    calculate_coordinate();    
    draw_coordinate(widget);
    draw_vectors(widget, g_var.kd->vectors, rgb_new(0,0,0));
    draw_vectors(widget, g_var.kd->init_gcenters, rgb_new(0,0,1));
    gtk_widget_queue_draw (widget);
  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

/// 向output_text 控件中追加文本
void append_output_text(char *s)
{
    GtkTextView *output_text = NULL;
    GtkTextBuffer *buffer = NULL;
    GtkTextIter start, end;
    char *str = NULL, *newstr = NULL;

    output_text = GTK_TEXT_VIEW(gtk_builder_get_object(g_var.builder,
                                                       "output_text"));
    buffer = gtk_text_view_get_buffer(output_text);
    
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    str = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    newstr = calloc(strlen(str)+strlen(s)+10, sizeof(char));
    strcpy(newstr, str);
    strcat(newstr, s);
    gtk_text_buffer_set_text(buffer, newstr, -1);
    

    free(str);
    free(newstr);
   
}

void scroll2end(GtkAdjustment *adjustment, gpointer user_data)
{
    gtk_adjustment_set_value(adjustment, 
            gtk_adjustment_get_upper(adjustment)
            - gtk_adjustment_get_page_size(adjustment));
}

/// 计算K均值聚类结果并绘图
void start_btn_calculate(GtkButton *button, gpointer user_data)
{
    int i;
    GList *loop = NULL;
    char s[512];
    GtkWidget *drawingarea = NULL; 

    k_means_solve(g_var.kd);
    append_output_text("\n计算结束\n");

    // 在 output_text 中输出文本分类结果并在 DrawingArea 中绘出分类结果
    clear_surface();
    drawingarea = GTK_WIDGET(gtk_builder_get_object(g_var.builder, 
                                                    "drawingarea"));
    draw_coordinate(drawingarea);
    for (i=0; i<g_var.kd->n; i++) {
        memset(s, 0, 512);
        sprintf(s, "Z%d:", i+1);
        append_output_text(s);
        for (loop = g_var.kd->gathers[i].list; 
                loop != NULL; 
                loop = loop->next) {

            memset(s, 0, 512);
            sprintf(s, " (%.4f,%.4f) ", 
                    ((coo_point_t *)(loop->data))->x,
                    ((coo_point_t *)(loop->data))->y);
            append_output_text(s);
        }
        append_output_text("\n");

        draw_vectors(drawingarea, g_var.kd->gathers[i].list, 
                colors[i%(sizeof(colors)/sizeof(rgb_t))]);
    }
    append_output_text("\n");

    // 清空kd
    k_means_data_free(g_var.kd);
    
    // 设置回调
    gtk_button_set_label(button, "重新开始");
    g_signal_handler_disconnect(drawingarea, g_var.drawingarea_handleid);
    g_signal_handler_disconnect(button, g_var.start_btn_handleid);
    g_var.start_btn_handleid = 
        g_signal_connect (GTK_WIDGET(button), "clicked",
                        G_CALLBACK (start_btn_begin), NULL);
}

/// 结束模式向量输入，开始聚类中心输入
void start_btn_end_input(GtkButton *button, gpointer user_data)
{
    GtkWidget *drawingarea = NULL; 

    gtk_button_set_label(button, "结束初始聚类中心输入并开始分类");
    drawingarea = GTK_WIDGET(gtk_builder_get_object(g_var.builder, 
                                                    "drawingarea"));
    
    g_signal_handler_disconnect(drawingarea, g_var.drawingarea_handleid);
    g_var.drawingarea_handleid = 
        g_signal_connect (drawingarea, "button-press-event",
                        G_CALLBACK (drawingarea_mouse_press_callbock2), NULL);
    g_signal_handler_disconnect(button, g_var.start_btn_handleid);
    g_var.start_btn_handleid = 
        g_signal_connect (GTK_WIDGET(button), "clicked",
                        G_CALLBACK (start_btn_calculate), NULL);
    append_output_text("\n请在左窗图中单击输入起始聚类中心输入点，输入结束后单击结束按钮。\n\n");

}

/// 开始模式向量输入
void start_btn_begin(GtkButton *button, gpointer user_data)
{

    GtkWidget *drawingarea = NULL; 

    gtk_button_set_label(button, "结束模式向量输入");
    drawingarea = GTK_WIDGET(gtk_builder_get_object(g_var.builder, 
                                                    "drawingarea"));

    g_var.drawingarea_handleid = 
        g_signal_connect (drawingarea, "button-press-event",
                        G_CALLBACK (drawingarea_mouse_press_callbock1), NULL);

    g_signal_handler_disconnect(button, g_var.start_btn_handleid);
    g_var.start_btn_handleid = 
        g_signal_connect (GTK_WIDGET(button), "clicked",
                        G_CALLBACK (start_btn_end_input), NULL);

    clear_surface();    
    append_output_text("请在左窗图中单击输入模式点，输入结束后单击“结束模式向量输入按钮。”\n\n");

}

/// 关闭窗口回调
static void close_window (void)
{
    if (g_var.surface)
        cairo_surface_destroy (g_var.surface);
}

/// 设置窗口布局，信号关联
static void activate (  GtkApplication *app, gpointer user_data)
{
    GtkWidget *window = NULL;
    GtkWidget *drawingarea = NULL; 
    GtkWidget *start_btn = NULL;
    GtkScrolledWindow *sw = NULL;
    GtkAdjustment *v = NULL;

    g_var.builder = gtk_builder_new();
    gtk_builder_add_from_file(g_var.builder, "kmeans.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(g_var.builder, "window"));
    gtk_application_add_window(app, GTK_WINDOW(window));
    drawingarea = GTK_WIDGET(gtk_builder_get_object(g_var.builder, 
                                                    "drawingarea"));
    gtk_widget_add_events(drawingarea, GDK_BUTTON_PRESS_MASK);
    gtk_widget_add_events(drawingarea, GDK_POINTER_MOTION_MASK);
    start_btn = GTK_WIDGET(gtk_builder_get_object(g_var.builder, "start_btn"));

    sw = GTK_SCROLLED_WINDOW(gtk_builder_get_object(g_var.builder, "scrolledwindow1"));
    v = gtk_scrolled_window_get_vadjustment(sw);

    g_var.kd = k_means_data_new(0);

    // 关联信号和回调函数
    g_signal_connect (window, "destroy", 
                        G_CALLBACK (close_window), NULL);
    g_signal_connect (drawingarea, "draw",
                        G_CALLBACK (draw_cb), NULL);
    g_signal_connect (drawingarea,"configure-event",
                        G_CALLBACK (configure_event_cb), NULL);
    g_signal_connect (v, "changed",
                        G_CALLBACK (scroll2end), NULL);
    g_var.start_btn_handleid = 
        g_signal_connect (start_btn, "clicked", 
                        G_CALLBACK (start_btn_begin), NULL);

    calculate_coordinate();
    gtk_widget_show_all (window);
    
}

/// 主函数
int main (int argc, char **argv)
{
    GtkApplication *app;
    int status;
    
    app = gtk_application_new ("org.gtk.funcdraw", G_APPLICATION_FLAGS_NONE);
    g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
    status = g_application_run (G_APPLICATION (app), argc, argv);
    g_object_unref (app);
    return status;
}

