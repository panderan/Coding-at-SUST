/******************************************************************************
 ******************************************************************************
 *
 * @file global.h
 *
 * @brief 定义一些全局变量
 *
 *
 * @author Deran Pan 
 * @date 2017-10-15
 *
 ******************************************************************************
 ******************************************************************************
 */
#ifndef GLOBALVAL_H
#define GLOBALVAL_H

#include <gtk/gtk.h>
#include "k_means.h"

/// 坐标系信息
struct coordinate_params {
    double startx;          /**< in: 起始 x 坐标值  */
    double endx;            /**< in: 结束 x 坐标值  */
    double middlex;         /**< x 中间值           */
    double stepx;           /**< x 轴步长           */
    double scalex;          /**< x 轴缩放倍数       */
    int x_axis_pos;         /**< x 轴在 y 方向上的位置  */
    int spacex;             /**< 多少像素表示一个单位   */
    double starty;          /**< in: 起始 y 坐标值  */
    double endy;            /**< in: 结束 y 坐标值  */
    double middley;         /**< y 中间值 */
    double stepy;           /**< y 轴步长 */
    double scaley;          /**< y 轴缩放倍数 */
    int y_axis_pos;         /**< y 轴在 x 方向上的位置  */
    int spacey;             /**< 多少个像素表示一个单位 */
    struct _coo_point origin_vertex; /**< 原点位置(窗口坐标) */
    int wwidth;             /**< 画布宽度 */
    int wheight;            /**< 画布高度 */
};

/// 全局变量
typedef struct global_var {
    GtkBuilder *builder;        /**< GtkBuilder */
    cairo_surface_t *surface;   /**< Surface to store current scribbles */
    struct coordinate_params coordinate; 
    int start_btn_handleid;     /**< start_btn 控件 clicked 回调id */
    int drawingarea_handleid;
    k_means_data_t *kd;         /**< k均值算法相关数据 */

} global_var_t;

extern global_var_t g_var;

#endif

