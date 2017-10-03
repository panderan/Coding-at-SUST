/******************************************************************************
 ******************************************************************************
 *
 * @file mission1.c
 *
 * @brief 绘制简单的一元函数图像 
 *
 * 　　绘制简单的一元函数图像，支持＋、－、×、＼、^、sin、cos、tan、（） 这九种
 * 运算的混合表达式。括号不支持多层嵌套。
 *
 * @author Deran Pan 
 * @date 2017-09-29
 *
 ******************************************************************************
 ******************************************************************************
 */

#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "global.h"
#include "expression_eval.h"

global_var_t g_var;

/// 释放 coo_points_t 结构
void coo_points_free(coo_points_t *n)
{
    if (n && n->vertexs)
        free(n->vertexs);
    if (n)
        free(n);
}

/// 创建一个坐标点集合
coo_points_t *coo_points_new(int total_points)
{
    coo_points_t *data = calloc(1, sizeof(coo_points_t));
    data->total = total_points;
    data->vertexs = calloc(data->total, sizeof(struct _coo_point));
    return data;
}

/// 获取函数坐标数据
/**
 *     计算函数的离散点好用以绘制函数图像。expr_str 指向函数的字符表达式，计算
 * 计算方法在 expression_eval.c 中实现。
 *
 * @param expr_str      指向函数字符串的指针
 * @param start_x       起始计算的x值
 * @param end_x         结束计算的x值
 * @param total_vertex  一共取多少个离散点
 * @param wwidth        当前窗口的宽度
 * @param wheight       当前窗口的高度
 *
 * @retval coo_points_t* 指向坐标点数组的指针
 *
 */
coo_points_t *calculate(char *expr_str, double start_x, double end_x, int total_vertex, int wwidth, int wheight)
{
    double start = 0;
    double step = 0;
    int i = 0, j = 0, str_len = 0;
    char buffer[1024];
    int buf_idx = 0;
    coo_points_t *data = NULL;
    double scales = wwidth / (end_x - start_x);
    
    start = start_x;
    step = (end_x - start_x)/total_vertex;
    data = coo_points_new(total_vertex+1);
    
    for (i=0; i < data->total; i++) {
        // 将x替换为数字
        memset(buffer, 0, 1024);
        for (j = 0, buf_idx = 0, str_len = strlen(expr_str); 
                j < str_len; j++) {
            if (expr_str[j] != 'x') {
                buffer[buf_idx++] = expr_str[j];
            }
            else {
                sprintf(buffer+buf_idx, "%f", start+step*i);
                buf_idx = strlen(buffer);
            }
        }
     
        (data->vertexs+i)->x = start+step*i;
        (data->vertexs+i)->y = expression_eval(buffer);
        printf("%s = %f\n", buffer, (data->vertexs+i)->y);
        (data->vertexs+i)->x = (data->vertexs+i)->x*scales+wwidth/2;
        (data->vertexs+i)->y = -(data->vertexs+i)->y*scales+wheight/2;
    }
    
    return data;
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

/// 画坐标系
/**
 *     绘制坐标系，参数给出 x 轴的起始点和结束点.
 */
static void draw_coordinate(GtkWidget *widget, double start_x, double end_x,
                                                double start_y, double end_y)
{
    cairo_t *cr;
    int i;

    // 计算坐标系参数
    g_var.coordinate.spacex = 50;
    g_var.coordinate.spacey = 50;
    g_var.coordinate.wwidth = gtk_widget_get_allocated_width (widget);
    g_var.coordinate.wheight = gtk_widget_get_allocated_height (widget);

    g_var.coordinate.startx = start_x;
    g_var.coordinate.endx = end_x;
    g_var.coordinate.middlex = start_x + (end_x - start_x) * 0.5;
    g_var.coordinate.stepx = (end_x - start_x)/(g_var.coordinate.wwidth / g_var.coordinate.spacex);
    g_var.coordinate.scalex = g_var.coordinate.wwidth /(g_var.coordinate.endx - g_var.coordinate.startx);
    if (end_y < 0)
        g_var.coordinate.x_axis_pos = 0;
    else if (start_y > 0)
        g_var.coordinate.x_axis_pos = g_var.coordinate.wheight;
    else {
        g_var.coordinate.x_axis_pos = (int)(-start_y / (end_y - start_y) * g_var.coordinate.wheight);
    }
    g_var.coordinate.x_axis_pos = g_var.coordinate.wheight - g_var.coordinate.x_axis_pos;

    g_var.coordinate.starty = start_y;
    g_var.coordinate.endy = end_y;
    g_var.coordinate.middley = start_y + (end_y - start_y) * 0.5;
    g_var.coordinate.stepy = (end_y - start_y)/(g_var.coordinate.wheight / g_var.coordinate.spacey);
    g_var.coordinate.scaley = g_var.coordinate.wheight / (g_var.coordinate.endy - g_var.coordinate.starty);
    if (end_x < 0)
        g_var.coordinate.y_axis_pos = 0;
    else if (start_x > 0)
        g_var.coordinate.y_axis_pos = g_var.coordinate.wwidth;
    else {
        g_var.coordinate.y_axis_pos = (int)(-start_x / (end_x - start_x) * g_var.coordinate.wwidth);
    }

    // 绘制
    cr = cairo_create (g_var.surface);
    cairo_set_source_rgb(cr, 0, 0, 0);

    // 绘制x轴
    cairo_move_to(cr, 0, g_var.coordinate.x_axis_pos);
    cairo_line_to(cr, g_var.coordinate.wwidth, g_var.coordinate.x_axis_pos);
    for (i=0; 
            g_var.coordinate.middlex + i*g_var.coordinate.stepx 
              < g_var.coordinate.endx; 
            i++) {
        cairo_move_to(cr, 
                g_var.coordinate.wwidth/2 + i*g_var.coordinate.stepx*g_var.coordinate.scalex, 
                g_var.coordinate.x_axis_pos+3);
        cairo_line_to(cr, 
                g_var.coordinate.wwidth/2 + i*g_var.coordinate.stepx*g_var.coordinate.scalex, 
                g_var.coordinate.x_axis_pos-5);
    }
    for (i=0; 
            g_var.coordinate.middlex - i*g_var.coordinate.stepx 
              > g_var.coordinate.startx; 
            i++) {
        cairo_move_to(cr, 
                g_var.coordinate.wwidth/2 - i*g_var.coordinate.stepx*g_var.coordinate.scalex, 
                g_var.coordinate.x_axis_pos+3);
        cairo_line_to(cr, 
                g_var.coordinate.wwidth/2 - i*g_var.coordinate.stepx*g_var.coordinate.scalex, 
                g_var.coordinate.x_axis_pos-5);
    }

    // 绘制y轴
    cairo_move_to(cr, g_var.coordinate.y_axis_pos, 0);
    cairo_line_to(cr, g_var.coordinate.y_axis_pos, g_var.coordinate.wheight);
    for (i=0;
            g_var.coordinate.middley + i*g_var.coordinate.stepy
            < g_var.coordinate.endy;
            i++) {
        cairo_move_to(cr,
                g_var.coordinate.y_axis_pos-3,
                g_var.coordinate.wheight/2 + i*g_var.coordinate.stepy*g_var.coordinate.scaley);
        cairo_line_to(cr,
                g_var.coordinate.y_axis_pos+5,
                g_var.coordinate.wheight/2 + i*g_var.coordinate.stepy*g_var.coordinate.scaley);
    }
    for (i=0;
            g_var.coordinate.middley - i*g_var.coordinate.stepy
            > g_var.coordinate.starty;
            i++) {
        cairo_move_to(cr,
                g_var.coordinate.y_axis_pos-3,
                g_var.coordinate.wheight/2 - i*g_var.coordinate.stepy*g_var.coordinate.scaley);
        cairo_line_to(cr,
                g_var.coordinate.y_axis_pos+5,
                g_var.coordinate.wheight/2 - i*g_var.coordinate.stepy*g_var.coordinate.scaley);
    }


#if 0
    /* Paint to the surface, where we store our state */
    cr = cairo_create (g_var.surface);
    cairo_set_source_rgb(cr, 0, 0, 0);
    cairo_move_to(cr, 0, height/2);
    cairo_line_to(cr, width, height/2);
    cairo_move_to(cr, width, height/2);
    cairo_line_to(cr, width-10, height/2-10);
    
    cairo_move_to(cr, width/2, height);
    cairo_line_to(cr, width/2, 0);
    cairo_move_to(cr, width/2, 0);
    cairo_line_to(cr, width/2-10, 10);
    cairo_set_line_width(cr, 1);
#endif

    cairo_stroke(cr);
    cairo_destroy (cr);
    gtk_widget_queue_draw (widget);
}

/// 画函数图像
/**
 *     该函数绘制函数线条，参数 data 即为指向 coo_points_t 结构数组的指针。该数
 * 组中存储了一系列已计算好的函数曲线上的采样点。
 */
static void draw_functional_image(GtkWidget *widget, coo_points_t *data) 
{
    int i = 0;
    cairo_t *cr;
    cr = cairo_create (g_var.surface);

    cairo_set_source_rgb(cr, 0, 0, 0);
    for (i=0; i<data->total-1; i++) {
        cairo_move_to(cr, (data->vertexs+i)->x, (data->vertexs+i)->y);
        cairo_line_to(cr, (data->vertexs+i+1)->x, (data->vertexs+i+1)->y);
    }
    cairo_stroke(cr);
    
    cairo_destroy (cr);
    gtk_widget_queue_draw (widget);
}

/// 开始绘图
/**
 *     该函数关联至 “绘图” 按钮的 click 事件。当 “绘图” 按钮被按下时。该函数从
 * 文本框读取函数字符并解析绘图。
 */
static void start_draw(GtkButton *button, gpointer data) 
{
    GtkWidget *text_view;
    GtkWidget *drawing_area;
    GtkTextBuffer *buffer;
    GtkTextIter start, end;
    char *funcs_strs = NULL;
    coo_points_t *vertexs;
    
    // 寻找 TextView 和 DrawArea
    text_view = GTK_WIDGET(gtk_builder_get_object(g_var.builder, "func_input"));
    drawing_area = GTK_WIDGET(gtk_builder_get_object(g_var.builder, "drawingarea"));
    
    // 获取函数输入
    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
    gtk_text_buffer_get_start_iter(buffer, &start);
    gtk_text_buffer_get_end_iter(buffer, &end);
    funcs_strs = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
    printf("%s\n", funcs_strs);

    clear_surface();
    draw_coordinate(drawing_area, -3,5,-3,5);
#if 0    
    vertexs = calculate(funcs_strs, -5, 5, 50, 
            gtk_widget_get_allocated_width (drawing_area),
            gtk_widget_get_allocated_height (drawing_area));
    
    // 绘制图像
    clear_surface();
    draw_coordinate(drawing_area, -5, 5, 1);
    draw_functional_image(drawing_area, vertexs);
#endif

    // 释放内存资源
    free(funcs_strs);
    //coo_points_free(vertexs);
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
    
  /* We've handled the configure event, no need for further processing. */
  return TRUE;
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
    GtkWidget *draw_btn = NULL;

    g_var.builder = gtk_builder_new();
    gtk_builder_add_from_file(g_var.builder, "ui.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(g_var.builder, "window"));
    gtk_application_add_window(app, GTK_WINDOW(window));
    drawingarea = GTK_WIDGET(gtk_builder_get_object(g_var.builder, "drawingarea"));
    draw_btn = GTK_WIDGET(gtk_builder_get_object(g_var.builder, "draw_btn"));

    // 创建窗口并设置属性
    gtk_window_set_title (GTK_WINDOW (window), "函数绘图");
    g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);
    g_var.window = window;
    
    
    /* 设置主题样式
    GdkScreen *screen;
    GtkCssProvider *css_provider;
    screen = gdk_screen_get_default();
    css_provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(css_provider, theme, strlen(theme), NULL);
    gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);
    // 释放资源
    g_object_unref(css_provider);
    */

    // 关联信号和回调函数
    g_signal_connect (drawingarea, "draw",
                        G_CALLBACK (draw_cb), NULL);
    g_signal_connect (drawingarea,"configure-event",
                        G_CALLBACK (configure_event_cb), NULL);
    g_signal_connect(draw_btn, "clicked",
                        G_CALLBACK (start_draw), NULL);
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

