#include <gtk/gtk.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "expression_eval.h"

const char * theme = "\
#draw_btn {\
    font-size: 18px;\
}\
#funcs_input_text_area {\
    font-size: 18px;\
}";

/* Surface to store current scribbles */
static cairo_surface_t *surface = NULL;

struct _coo_point {
	double x;
	double y;
};

typedef struct coo_point {
	int total;
	struct _coo_point *vertexs;
} coo_point_t;

coo_point_t *coo_point_new(int total_points)
{
	coo_point_t *data = calloc(1, sizeof(coo_point_t));
	data->total = total_points;
	data->vertexs = calloc(data->total, sizeof(struct _coo_point));
	return data;
}

// 获取函数坐标数据
coo_point_t *calculate(char *expr_str, double start_x, double end_x, int total_vertex, int wwidth, int wheight)
{
	double start = 0;
	double step = 0;
	int i = 0, j = 0, str_len = 0;
	char buffer[1024];
	int buf_idx = 0;
	coo_point_t *data = NULL;
	double scales = wwidth / (end_x - start_x);

	start = start_x;
	step = (end_x - start_x)/total_vertex;
	data = coo_point_new(total_vertex+1);

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






// 将画布置为空白
static void clear_surface (void)
{
	cairo_t *cr;
	cr = cairo_create (surface);
	cairo_set_source_rgb (cr, 1, 1, 1);
	cairo_paint (cr);
	cairo_destroy (cr);
}

// 画坐标系
static void draw_coordinate(GtkWidget *widget, int start_x, int end_x, int step)
{
	cairo_t *cr;
	int width, height, i;

	width = gtk_widget_get_allocated_width (widget);
	height = gtk_widget_get_allocated_height (widget);

	/* Paint to the surface, where we store our state */
	cr = cairo_create (surface);
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

	cairo_stroke(cr);

	cairo_destroy (cr);

	gtk_widget_queue_draw (widget);
}

// 画函数图像
static void draw_functional_image(GtkWidget *widget, coo_point_t *data) 
{
	int i = 0;
	cairo_t *cr;
	cr = cairo_create (surface);

	cairo_set_source_rgb(cr, 0, 0, 0);
	for (i=0; i<data->total-1; i++) {
		cairo_move_to(cr, (data->vertexs+i)->x, (data->vertexs+i)->y);
		cairo_line_to(cr, (data->vertexs+i+1)->x, (data->vertexs+i+1)->y);
	}
	cairo_stroke(cr);
	
	cairo_destroy (cr);
	gtk_widget_queue_draw (widget);
}

static void start_draw(GtkButton *button, gpointer data) 
{
	GtkWidget *container, *container_v_box;
	GtkWidget *text_view;
	GtkWidget *drawing_area;
	GtkTextBuffer *buffer;
	GtkTextIter start, end;
	char *funcs_strs = NULL;
	coo_point_t *vertexs;


	// printf("%s\n", gtk_button_get_label(button));
	container = gtk_widget_get_parent(GTK_WIDGET(button));
	container_v_box = gtk_widget_get_parent(GTK_WIDGET(container));
	// printf("parent:%s pparent:%s\n", gtk_widget_get_name(container), gtk_widget_get_name(container_v_box));
	

	// 寻找 TextView
	GList *children = gtk_container_get_children(GTK_CONTAINER(container));
	do {
		// printf("child:%s\n", gtk_widget_get_name(GTK_WIDGET(children->data)));
		if (strcmp(gtk_widget_get_name(GTK_WIDGET(children->data)), "scrolled_window") == 0) {
			container = children->data;
			break;
		}
	} while ((children=g_list_next(children)) != NULL);

	children = gtk_container_get_children(GTK_CONTAINER(container));
	text_view = gtk_bin_get_child(GTK_BIN(container));
	// printf("%s\n", gtk_widget_get_name(GTK_WIDGET(text_view)));
	
	// 寻找 DrawArea
	container = container_v_box;
	children = gtk_container_get_children(GTK_CONTAINER(container));
	do {
		if (strcmp(gtk_widget_get_name(GTK_WIDGET(children->data)), "frame") == 0) {
			container = children->data;
			break;
		}
	} while ((children=g_list_next(children)) != NULL);
	children = gtk_container_get_children(GTK_CONTAINER(container));
	drawing_area = gtk_bin_get_child(GTK_BIN(container));

	// 获取函数输入
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(text_view));
	gtk_text_buffer_get_start_iter(buffer, &start);
	gtk_text_buffer_get_end_iter(buffer, &end);
	funcs_strs = gtk_text_buffer_get_text(buffer, &start, &end, FALSE);
	printf("%s\n", funcs_strs);

	vertexs = calculate(funcs_strs, -5, 5, 50, 
			gtk_widget_get_allocated_width (drawing_area),
			gtk_widget_get_allocated_height (drawing_area));
	free(funcs_strs);


	// 绘制图像
	clear_surface();
	draw_coordinate(drawing_area, -5, 5, 1);
	draw_functional_image(drawing_area, vertexs);
}

// 重绘图像
static gboolean draw_cb(GtkWidget *widget, cairo_t *cr, gpointer data)
{
	cairo_set_source_surface (cr, surface, 0, 0);
	cairo_paint (cr);
	return FALSE;
}

// 创建一个新的合适尺寸的画布
static gboolean configure_event_cb (GtkWidget *widget, 
									GdkEventConfigure *event, 
									gpointer data)
{
	if (surface)
		cairo_surface_destroy (surface);

	surface = gdk_window_create_similar_surface (gtk_widget_get_window (widget),
									CAIRO_CONTENT_COLOR,
									gtk_widget_get_allocated_width (widget),
									gtk_widget_get_allocated_height (widget));

	/* Initialize the surface to white */
	clear_surface ();

  /* We've handled the configure event, no need for further processing. */
  return TRUE;
}

static void close_window (void)
{
	if (surface)
		cairo_surface_destroy (surface);
}

static void activate (  GtkApplication *app, gpointer user_data)
{
	GtkWidget *window;
	GtkWidget *frame;
	GtkWidget *drawing_area;
	GtkWidget *v_box, *h_box;
	GtkWidget *draw_button;
	GtkWidget *view;
	GtkTextBuffer *buffer;
	GtkWidget *scrolled_window;
	GdkScreen *screen;
	GtkCssProvider *css_provider;
	GtkStyleContext *context;

	// 创建窗口并设置属性
	window = gtk_application_window_new (app);
	gtk_window_set_title (GTK_WINDOW (window), "函数绘图");
	g_signal_connect (window, "destroy", G_CALLBACK (close_window), NULL);
	gtk_container_set_border_width (GTK_CONTAINER (window), 8);

	v_box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 10);
	gtk_widget_set_name(v_box, "v_box");
	gtk_container_add(GTK_CONTAINER(window), v_box);

	// 创建frame
	frame = gtk_frame_new (NULL);
	gtk_widget_set_name(frame, "frame");
	gtk_frame_set_shadow_type (GTK_FRAME (frame), GTK_SHADOW_IN);
	gtk_box_pack_start(GTK_BOX(v_box), frame, TRUE, TRUE, 0);

	// 创建绘画控件并设置属性
	drawing_area = gtk_drawing_area_new ();
	gtk_widget_set_name(drawing_area, "drawing_area");
	gtk_widget_set_size_request (drawing_area, 1000, 800);
	gtk_container_add (GTK_CONTAINER (frame), drawing_area);
	
	h_box = gtk_box_new(GTK_ORIENTATION_VERTICAL, 10);
	gtk_widget_set_name(h_box, "h_box");
	gtk_box_pack_start(GTK_BOX(v_box), h_box, FALSE, FALSE, 0);

	draw_button = gtk_button_new_with_label("绘图");
	gtk_widget_set_name(draw_button, "draw_btn");
	gtk_box_pack_start(GTK_BOX(h_box), draw_button, FALSE, FALSE, 0);

	scrolled_window = gtk_scrolled_window_new(NULL, NULL);
	gtk_widget_set_name(scrolled_window, "scrolled_window");
	gtk_widget_set_size_request (scrolled_window, 300, 200);
	gtk_box_pack_start(GTK_BOX(h_box), scrolled_window, TRUE, TRUE, 0);

	view = gtk_text_view_new();
	gtk_widget_set_name(view, "funcs_input_text_area");
	buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
	gtk_text_buffer_set_text(buffer, "sin(x)", -1);
	gtk_text_view_set_wrap_mode(GTK_TEXT_VIEW(view), GTK_WRAP_WORD);
	gtk_container_add(GTK_CONTAINER(scrolled_window), view);

	// 设置主题样式
	screen = gdk_screen_get_default();
	context = gtk_widget_get_style_context(v_box);
	css_provider = gtk_css_provider_new();
	gtk_css_provider_load_from_data(css_provider, theme, strlen(theme), NULL);
	gtk_style_context_add_provider_for_screen(screen, GTK_STYLE_PROVIDER(css_provider), GTK_STYLE_PROVIDER_PRIORITY_USER);

	// 关联信号和回调函数
	g_signal_connect (drawing_area, "draw",
						G_CALLBACK (draw_cb), NULL);
	g_signal_connect (drawing_area,"configure-event",
						G_CALLBACK (configure_event_cb), NULL);
	g_signal_connect(draw_button, "clicked",
						G_CALLBACK (start_draw), NULL);
	gtk_widget_show_all (window);
	
	// 释放不需要的资源
	g_object_unref(css_provider);
}

int main (int argc, char **argv)
{
	GtkApplication *app;
	int status;

	app = gtk_application_new ("org.gtk.mission1", G_APPLICATION_FLAGS_NONE);
	g_signal_connect (app, "activate", G_CALLBACK (activate), NULL);
	status = g_application_run (G_APPLICATION (app), argc, argv);
	g_object_unref (app);

	return status;
}

