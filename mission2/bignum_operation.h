/******************************************************************************
 ******************************************************************************
 *
 * @file bignum_operation.c
 *
 * @brief 实现字符数的加减乘除运行 
 *
 * 　　数字采用字符串的形式进行存储和运行，对数字大小和精度均无限制。
 *
 * @author Deran Pan 
 * @date 2017-09-29
 *
 ******************************************************************************
 ******************************************************************************
 */

#ifndef BIGNUM_OPERATION_H
#define BIGNUM_OPERATION_H

/// 字符数存储结构
typedef struct snum {
    char * str;       /**< 数字字符串 */
    int len;          /**< 字符长长度（包括小数点）*/
    int dot_idx;      /**< 小数点的下标位置 */
    int sign;         /**< 0 为正， 非0 为负 */
} snum_t;

// 由数字字符串构建一个字符数结构（struct snum）
snum_t *snum_new2(char *str, int len);
#define snum_new1(str) snum_new2(str, 0)

// 释放一个字符数结构
void snum_free(snum_t *snum);

// 将一个字符数转化为一个字符已分配数组（alloced）
char * snum_getstring(snum_t *snum);

// 将两个字符数结构进行加法运行得到一个新的已分配的字符数结构（alloced）
snum_t * snum_plus(snum_t *snum1, snum_t *snum2);

// 将两个字符数结构进行减法运行得到一个新的已分配的字符数结构（alloced）
snum_t * snum_minus(snum_t *snum1, snum_t *snum2);

// 将两个字符数结构进行乘法运行得到一个新的已分配的字符数结构（alloced）
snum_t *snum_multiple(snum_t *snum1, snum_t *snum2);

// 将两个字符数结构进行乘法运行得到一个新的已分配的字符数结构（alloced）
snum_t *snum_divide3(snum_t *snum1, snum_t *snum2, int precision);
#define snum_divide(snum1, snum2) snum_divide3(snum1, snum2, 0)

// 测试宏
#define TEST_SNUM_PLUS(str1, str2, p1, p2, p3, ch) \
            printf("%s + %s = %s\n", (str1), (str2), \
            ch=snum_getstring(p3=snum_plus(p1=snum_new1(str1), p2=snum_new1(str2)))); \
            snum_free(p1), snum_free(p2), snum_free(p3), free(ch); \
            p1 = NULL, p2 = NULL, p3 = NULL, ch = NULL;
#define TEST_SNUM_MINUS(str1, str2, p1, p2, p3, ch) \
            printf("%s - %s = %s\n", (str1), (str2), \
            ch=snum_getstring(p3=snum_minus(p1=snum_new1(str1), p2=snum_new1(str2)))); \
            snum_free(p1), snum_free(p2), snum_free(p3), free(ch); \
            p1 = NULL, p2 = NULL, p3 = NULL, ch = NULL;
#define TEST_SNUM_MULTIPLE(str1, str2, p1, p2, p3, ch) \
            printf("%s * %s = %s\n", (str1), (str2), \
            ch=snum_getstring(p3=snum_multiple(p1=snum_new1(str1), p2=snum_new1(str2)))); \
            snum_free(p1), snum_free(p2), snum_free(p3), free(ch); \
            p1 = NULL, p2 = NULL, p3 = NULL, ch = NULL;
#define TEST_SNUM_DIVIDE(str1, str2, p1, p2, p3, ch) \
            printf("%s / %s = %s\n", (str1), (str2), \
            ch=snum_getstring(p3=snum_divide(p1=snum_new1(str1), p2=snum_new1(str2)))); \
            snum_free(p1), snum_free(p2), snum_free(p3), free(ch); \
            p1 = NULL, p2 = NULL, p3 = NULL, ch = NULL;
#define TEST_OFFSET_DOT(n1, ch, numstr, offset) \
            n1 = snum_new1(numstr); __offset_dot(n1, offset); \
            printf("orig:%s  -  offset:%d  -  out:%s\n", numstr, offset, ch=snum_getstring(n1)); \
            snum_free(n1); free(ch), n1 = NULL, ch = NULL;
#endif

