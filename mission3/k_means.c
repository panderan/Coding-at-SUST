/******************************************************************************
 ******************************************************************************
 *
 * @file k_means.c
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
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "k_means.h"

// 计算欧氏距离值
#define euclidean_distance(x1,y1,x2,y2) \
    sqrt(((x1)-(x2))*((x1)-(x2))+((y1)-(y2))*((y1)-(y2)))

/// 创建一个二维模式（坐标点）
coo_point_t *coo_point_new(double x, double y)
{
    coo_point_t *ret = NULL;

    ret = calloc(1, sizeof(coo_point_t));
    if (ret == NULL) 
        exit(EXIT_FAILURE);

    ret->x = x;
    ret->y = y;

    return ret;
}

/// 新建k均值算法数据结构
k_means_data_t * k_means_data_new(int n) 
{
    k_means_data_t *ret = NULL;

    ret = calloc(1, sizeof(k_means_data_t));
    if (ret == NULL) 
        exit(EXIT_FAILURE);

    ret->n = n;

    return ret;
}

/// 释放二维模式（坐标点）空间
void free_coo_point_t (gpointer data) {
    if (data != NULL)
        free(data);
}

/// 释放聚类集
void k_means_free_gather(k_gather_t *kg)
{
    g_list_free_full(kg->list, free_coo_point_t);
    kg->list = NULL;
}

/// 释放所有聚类集
void k_means_free_gathers(k_means_data_t *kd)
{
    int i = 0;

    for (i=0; i < kd->n; i++) {
        k_means_free_gather(kd->gathers+i);
    }
}

/// 释放k均值算法数据集
void k_means_data_free(k_means_data_t *kd)
{
    kd->n = 0;
    k_means_free_gathers(kd);
    free(kd->gathers);
    g_list_free_full(kd->init_gcenters, free_coo_point_t);
    g_list_free_full(kd->vectors, free_coo_point_t);
    kd->gathers = NULL;
    kd->init_gcenters = NULL;
    kd->vectors = NULL;
}

/// 向k均值算法数据集中添加新模式（二维点）
void k_means_data_add(k_means_data_t *kd, double x, double y)
{
    coo_point_t *tmp = coo_point_new(x, y);
    kd->vectors = g_list_append(kd->vectors, tmp); 
}

/// 将k均值算法数据集中模式按最短距离聚类
void sort_by_shortest_distance(k_means_data_t *kd) 
{
    GList *loop = NULL;
    double *dis = NULL;
    int i = 0, min = 0;

    dis = calloc(kd->n, sizeof(double));
    for (loop = kd->vectors; loop!= NULL; loop = loop->next) {
        for (i = 0; i < kd->n; i++) {
            *(dis+i) = euclidean_distance((kd->gathers+i)->center.x,
                                          (kd->gathers+i)->center.y,
                                          ((coo_point_t*)(loop->data))->x,
                                          ((coo_point_t*)(loop->data))->y);
        }
        for (min = 0, i = 1; i< kd->n; i++) {
            if (*(dis+min) > *(dis+i))
                min = i;
        }

        coo_point_t *tmp = coo_point_new(((coo_point_t*)(loop->data))->x,
                                         ((coo_point_t*)(loop->data))->y);
        kd->gathers[min].list = g_list_append(kd->gathers[min].list, tmp); 
    }
}

/// 验证聚类结果
/**
 * @param kd k均值聚类数据集
 * @retval 0 需要重新聚类
 * @retval 1 聚类完成
 */
int verify_result(k_means_data_t *kd)
{
    coo_point_t average;
    GList *loop = NULL;
    int i = 0, end = 1;

    for (i = 0; i< kd->n; i++) {
        average.x = 0;
        average.y = 0;
        for (loop = kd->gathers[i].list; loop != NULL; loop = loop->next) {
            average.x += ((coo_point_t *)(loop->data))->x;
            average.y += ((coo_point_t *)(loop->data))->y;
        }
        average.x /= g_list_length(kd->gathers[i].list);
        average.y /= g_list_length(kd->gathers[i].list);
        if (abs(average.x - kd->gathers[i].center.x) > 0.0001 ||
                abs(average.y - kd->gathers[i].center.y) > 0.0001)
        {
            // 需要重新聚类
            kd->gathers[i].center.x = average.x;
            kd->gathers[i].center.y = average.y;
            end = 0;
        }
    }
    return end;
}

/// 运行k均值聚类算法
void k_means_solve(k_means_data_t *kd)
{
    int i = 0, end = 0;
    GList *loop = NULL;

    kd->n = g_list_length(kd->init_gcenters);
    kd->gathers = calloc(kd->n, sizeof(k_gather_t));
    loop = kd->init_gcenters;
    for (i=0; i<kd->n; i++) {
       kd->gathers[i].center.x = ((coo_point_t *)(loop->data) )->x;
       kd->gathers[i].center.y = ((coo_point_t *)(loop->data) )->y;
       loop = loop->next;
    }

    while (!end) {
        k_means_free_gathers(kd);
        sort_by_shortest_distance(kd);
        end = verify_result(kd);
    }
}

