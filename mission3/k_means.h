/******************************************************************************
 ******************************************************************************
 *
 * @file k_means.h
 * 
 * @brief 二维K-均值聚类算法
 *  
 *      二维k-均值聚类算法的C实现。
 *
 * @author Deran Pan 
 * @date 2017-10-15
 *
 ******************************************************************************
 ******************************************************************************
 */
#ifndef K_MEANS_H
#define K_MENAS_H

#include <glib.h>

/// 二维模式向量（坐标点）
typedef struct _coo_point {
    double x;
    double y;
} coo_point_t;

/// 聚类集
typedef struct k_gather {
    coo_point_t center;     /**< 聚类中心点 */
    GList *list;            /**< 属于该类的模式链表 */
}k_gather_t;

/// k均值算法数据集
typedef struct k_means_data {
    int n;                  /**< 聚类中心个数（init_gcenters链表长度） */
    GList *vectors;         /**< in: 聚类中心链表 */
    GList *init_gcenters;   /**< in: 模式链表   */
    k_gather_t *gathers;    /**< 所有聚类集     */
}k_means_data_t;

// 新建二维模式（点）
coo_point_t *coo_point_new(double x, double y);

// 新建k均值算法数据结构
k_means_data_t * k_means_data_new(int n);

// 运行k均值算法
void k_means_solve(k_means_data_t *kd);

// 释放k均值算法数据集
void k_means_data_free(k_means_data_t *kd);
#endif
