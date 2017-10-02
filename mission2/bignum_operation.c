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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum_operation.h"


/// 对齐两个数字
/**
 * @param snum1 指向 snum_t 结构的指针
 * @param snum2 指向 snum_t 结构的指针
 * @retval snum_t指针数组 下标分别为0和1，指向两个对齐的 snum_t 结构。
 */
static snum_t * snum_align(snum_t *snum1, snum_t *snum2)
{
    snum_t *ret = NULL, *ret1 = NULL, *ret2 = NULL;
    int fractional_alignlen = 0;    // 小数段长度（不含小数点）
    int integer_alignlen = 0;       // 整数段长度（不含小数点）
    int i = 0;
    
    // 分别计算小数段和整数段长度
    fractional_alignlen = snum1->dot_idx > snum2->dot_idx ? 
        snum1->dot_idx : snum2->dot_idx;
    integer_alignlen = snum1->len - snum1->dot_idx > snum2->len - snum2->dot_idx ?
        snum1->len - 1 - snum1->dot_idx : snum2->len - 1 - snum2->dot_idx;
    
    // 构建返回值并初始化其长度信息
    ret = calloc(2, sizeof(snum_t));
    if (ret == NULL) {
        exit(EXIT_FAILURE);
    }
    ret1 = ret;
    ret2 = ret+1;
    ret1->len = fractional_alignlen + integer_alignlen + 1;
    ret1->dot_idx = fractional_alignlen;
    ret2->len = fractional_alignlen + integer_alignlen + 1;
    ret2->dot_idx = fractional_alignlen;
    ret1->str = calloc(ret1->len+1, sizeof(char));
    ret2->str = calloc(ret2->len+1, sizeof(char));
    if (ret1->str == NULL || ret2->str == NULL) {
        exit(EXIT_FAILURE);
    }

    // 分别赋值
    for (i=0; i<ret1->len; i++) {
        // 处理ret1
        if (i-ret1->dot_idx+snum1->dot_idx >= 0 && 
                i< (ret1->dot_idx == snum1->dot_idx ? snum1->len : snum1->len + ret1->dot_idx - snum1->dot_idx)) {
            *(ret1->str+i) = *(snum1->str+i-ret1->dot_idx+snum1->dot_idx);
        }
        else {
            *(ret1->str+i) = '0';
        }

        // 处理ret2
        if (i-ret2->dot_idx+snum2->dot_idx >= 0 && 
                i< (ret1->dot_idx == snum2->dot_idx ? snum2->len : snum2->len + ret1->dot_idx - snum2->dot_idx)) {
            *(ret2->str+i) = *(snum2->str+i-ret2->dot_idx+snum2->dot_idx);
        }
        else {
            *(ret2->str+i) = '0';
        }
    }

    return ret;
}

/// 比较两个 snum_t 字符数的大小
/**
 * @param snum1 指向 snum_t 结构的指针
 * @param snum2 指向 snum_t 结构的指针
 * @retval  0  snum1 >= snum2
 * @retval  1  snum1 <  snum2
 */
static int snum_compare(snum_t *snum1, snum_t *snum2)
{
    int i = 0, j = 0;
    
    if (snum1->len - snum1->dot_idx > snum2->len - snum2->dot_idx) {
        // 比较整数部分长度，长着为大
        return 0;
    }
    else if (snum1->len - snum1->dot_idx == snum2->len - snum2->dot_idx) {
        // 整数等长，比较小数
        for (i=snum1->len-1, j=snum2->len-1; i>=0 && j>=0; i--, j--) {
            if (*(snum1->str+i) > *(snum2->str+j)) {
                return 0;
            }
            else if (*(snum1->str+i) < *(snum2->str+j)) {
                return 1;
            }
            else {
                continue;
            }
        }
        i++;
        j++;
        if (i==0 && j==0) {
            return 0;
        }
        else if (i==0 && j>0) {
            return 1;
        }
        else {
            return 0;
        }
    }
    else {
        return 1;
    }
}

/// 删除字符数的前导0
/**
 * @param n 指向 snum_t 结构的指针
 */
static void wipe_leading_zero(snum_t *n)
{
    int i = 0;
    for (i=n->len-1; i>=0; i--) {
        if (*(n->str+i) == '0') {
            if (i!=0 && *(n->str+i-1) != '.') {
                n->len--;
            }
            else {
                break;
            }
        }
        else {
            break;
        }
    }
}

/// 删除字符数的后缀0
/**
 * @param n 指向 snum_t 结构的指针
 */
static void wipe_tail_zero(snum_t *n)
{
    int i = 0, offset = 0;
    for (i=0; i<n->dot_idx; i++) {
        if (*(n->str+i) != '0') {
            break;
        }
    }

    for (offset=i,i=0; i+offset<n->len; i++) {
        *(n->str+i) = *(n->str+i+offset);
    }
    n->len = n->len-offset;
    *(n->str+n->len) = 0;
}

/// 个位数加法
/**
 * @param n1 加数字符，0~9
 * @param n2 被加数字符，0~9
 * @param is_carry 加法结果是否进位，0代表不进位，1代表进位
 * @param res 指向字符的指针，存储计算结果
 */
static int __snum_plus(char n1, char n2, int is_carry, char *res)
{
    *(res) = n1 + n2 - 48;
    if (is_carry) {
        *res = *res + 1;
    }

    if (*res > 57) {
        *res = *res - 10;
        return 1;
    }
    else {
        return 0;
    }
}

/// 字符数加法
/**
 * 这里假定 snum1 和 snum2 都是正数。
 *
 * @param snum1 指向 snum_t 结构的指针
 * @param snum2 指向 snum_t 结构的指针
 * @retval snum_t* 返回一个已分配内存存储了计算结果的 snum_t 结构
 */
static snum_t * _snum_plus(snum_t *snum1, snum_t *snum2)
{
    snum_t *optnum = NULL, *ret_snum = NULL;
    int i = 0, is_carry = 0;
    
    ret_snum = calloc(1, sizeof(snum_t));
    if (ret_snum == NULL) {
        exit(EXIT_FAILURE);
    }
    
    optnum = snum_align(snum1, snum2);
    ret_snum->str = calloc(optnum->len+1, sizeof(char));
    if (ret_snum->str == NULL) {
        free(optnum);
        exit(EXIT_FAILURE);
    }

    for (i=0; i<optnum->len; i++) {
        if (*(optnum->str+i) == '.') {
            *(ret_snum->str+i) = '.';
            continue;
        }
        is_carry = __snum_plus(*(optnum->str+i), *((optnum+1)->str+i), is_carry, ret_snum->str+i);
    }
    ret_snum->dot_idx = optnum->dot_idx;
    ret_snum->len = optnum->len;
    if (is_carry) {
        *(ret_snum->str+i) = '1';
        ret_snum->len++;
    }
    return ret_snum;
}

/// 个位数减加
/**
 * @param n1 减数字符，0~9
 * @param n2 被减数字符，0~9
 * @param is_borrow 是否需要借位，0代表不借位，1代表借位
 * @param res 指向字符的指针，存储计算结果
 */
static int __snum_minus(char n1, char n2, int is_borrow, char *res)
{
    *res = n1 - n2 + 48;
    if (is_borrow) {
        *res = *res - 1;
    }
    
    if (*res >= 48) {
        return 0;
    }
    else {
        *res = *res + 10;
        return 1;
    }
}

/// 字符数加法
/**
 * 这里假定 snum1 和 snum2 都是正数，且 snum1 要大于 snum2 。
 *
 * @param snum1 指向 snum_t 结构的指针
 * @param snum2 指向 snum_t 结构的指针
 * @retval snum_t* 返回一个已分配内存存储了计算结果的 snum_t 结构
 */
static snum_t * _snum_minus(snum_t *snum1, snum_t *snum2)
{
    snum_t *optnum = NULL, *ret_snum = NULL;
    int i = 0, is_borrow = 0;
    
    ret_snum = calloc(1, sizeof(snum_t));
    if (ret_snum == NULL) {
        exit(EXIT_FAILURE);
    }
    optnum = snum_align(snum1, snum2);
    ret_snum->str = calloc(optnum->len+1, sizeof(char));
    if (ret_snum->str == NULL) {
        free(optnum);
        exit(EXIT_FAILURE);
    }

    for (i=0; i<optnum->len; i++) {
        if (*(optnum->str+i) == '.') {
            *(ret_snum->str+i) = '.';
            continue;
        }
        is_borrow = __snum_minus(*(optnum->str+i), *((optnum+1)->str+i), is_borrow, ret_snum->str+i);
    }
    ret_snum->dot_idx = optnum->dot_idx;
    ret_snum->len = optnum->len;

    wipe_leading_zero(ret_snum);
    return ret_snum;
}

/// 移动字符数的小数点位置（乘10或除10操作）
/**
 * @param snum1 指向待操作字符数结构的指针
 * @param offset 移动方式，offset>0表示乘10操作，offset<0表示除10操作
 */
static void __offset_dot(snum_t *snum1, int offset)
{
    int dif = snum1->dot_idx - offset;
    int i = 0;

    if (dif < 0) {
        // 乘以10且左补0
        
        // 将小数点移动最左端
        for (i=snum1->dot_idx; i>0; i--) {
            *(snum1->str+i) = *(snum1->str+i-1);
        }
        (*snum1->str) = '.';
        
        // 申请新的空间且补零
        snum1->len = snum1->len-dif;
        snum1->dot_idx = 0;
        snum1->str = realloc(snum1->str, snum1->len+1);
        *(snum1->str+snum1->len) = '\0';
        for (i=snum1->len-1; i>-dif; i--) {
            *(snum1->str+i) = *(snum1->str+i+dif);
        }
        for (;i>0;i--) {
            *(snum1->str+i) = '0';
        }
        *(snum1->str) = '.';
    }
    else if (dif > snum1->len-2) {
        // 除以10且右补零
        
        // 将小数点移动至最右端
        for (i=snum1->dot_idx; i<snum1->len-1; i++) {
            *(snum1->str+i) = *(snum1->str+i+1);
        }
        *(snum1->str+i) = '.';

        // 申请新的空间且补零
        snum1->str = realloc(snum1->str, dif+3);
        i++;
        *(snum1->str+i) = '0';
        for (i++; i<dif+2; i++) {
            *(snum1->str+i-2) = '0';
            *(snum1->str+i-1) = '.';
            *(snum1->str+i) = '0';
        }
        snum1->len = dif + 2;
        snum1->dot_idx = dif;
        *(snum1->str+snum1->len) = '\0';
    }
    else {
        // 左移或右移且不用心申请空间
        
        if (offset > 0) {
            // 乘以10，左移动
            for (i=0; i<offset; i++) {
                *(snum1->str+snum1->dot_idx) = *(snum1->str+snum1->dot_idx-1);
                *(snum1->str+snum1->dot_idx-1) = '.';
                snum1->dot_idx--;
            }
        }
        else {
            // 除以10，右移动
            for (i=0; i<-offset; i++) {
                *(snum1->str+snum1->dot_idx) = *(snum1->str+snum1->dot_idx+1);
                *(snum1->str+snum1->dot_idx+1) = '.';
                snum1->dot_idx++;
            }
        }
    }

    wipe_leading_zero(snum1);
}

/// 字符数和一位数的乘法
/**
 * @param snum1 指向待操作字符数结构的指针，乘数
 * @param n 被乘数的字符，0~9
 * @retval snum_t* 结果返回值，一个已分配内存的snum_t结构
 */
static snum_t *__snum_multiple(snum_t *snum1, char n)
{
    snum_t *ret = NULL;
    int i = 0, m = 0, carry = 0;

    ret = calloc(1, sizeof(snum_t));
    if (ret == NULL) {
        exit(EXIT_FAILURE);
    }

    ret->len = snum1->len+1;
    ret->dot_idx = snum1->dot_idx;
    ret->str = calloc(ret->len+1, sizeof(char));
    if (ret->str == NULL) {
        exit(EXIT_FAILURE);
    }

    for (i=0; i<snum1->len; i++) {
        if (*(snum1->str+i) == '.') {
            *(ret->str+i) = '.';
        }
        else {
            m = (*(snum1->str+i)-48) * (n-48) + carry;
            carry = m / 10;
            m = m % 10;
            *(ret->str+i) = m + 48;
        }
    }

    if (carry != 0) {
        *(ret->str+i) = carry+48;
    }
    else {
        ret->len--;
    }
    return ret;
}

/// 构造一个 snum_t 字符数结构
/**
 * @param str 数字初值
 * @param len 数字字符存储长度，如果 len=0 则 len 的长度为 初始的数字字符串长度
 * @retval snum_t 字符数结构
 */
snum_t *snum_new2(char *str, int len)
{
    snum_t *snum = NULL;
    char *numstr = NULL;

    int i = 0, j = 0;

    snum = calloc (1, sizeof(snum_t));
    if (snum == NULL) {
        return NULL;
    }
    
    // 处理正负号
    if (*str == '-') {
        snum->sign = 1;
        numstr = str+1;
    }
    else {
        numstr = str;
    }
    
    snum->len = strlen(numstr);
    if (len) {
        snum->str = calloc(len, sizeof(char));
    }
    else {
        snum->str = calloc(snum->len+2, sizeof(char));
    }
    if (snum->str == NULL) {
        free(snum);
        return NULL;
    }

    // 赋值
    //   检查是否有小数点
    for (i=0; i<snum->len; i++) {
        if (*(numstr+i) == '.') {
            break;
        }
    }
    if (i < snum->len) {
        // 有小数点
        for (i=0,j=snum->len-1;
                i<snum->len;
                *(snum->str+i++)=*(numstr+j--));
    }
    else {
        // 没有小数点
        *(snum->str+0) = '.';
        snum->len++;
        for (i=1,j=snum->len-1;
                i<snum->len;
                *(snum->str+i++)=*(numstr-1+j--));
    }

    // 计算小数点的位置
    for (i=0; i<snum->len; i++) {
        if (*(snum->str+i) == '.') {
            snum->dot_idx = i;
        }
    }
    wipe_leading_zero(snum);
    wipe_tail_zero(snum);
    return snum;
}

/// 释放 snum_t 的内存空间
/**
 * @param snum 待释放的指向 snum_t 结构的指针
 */
void snum_free(snum_t *snum)
{
    if (snum!= NULL && snum->str != NULL) {
        free(snum->str);
    }
    if (snum!=NULL) {
        free(snum);
    }
    return;
}

/// 为 snum_t 字符数赋值
/**
 * @param n 待赋值的指向 snum_t 结构的指针
 * @param str 新值
 */
void snum_set(snum_t *n, char *str)
{
    snum_t *tmp = NULL;
    tmp = snum_new1(str);
    
    free(n->str);
    memcpy(n, tmp, sizeof(snum_t));
    free(tmp);
}

/// 将存储在 snum_t 中的数字转换为字符串
/**
 * @param snum 带转换的指向 snum_t 结构的指针
 * @retval string 返回一个已分配内存的字符串（需额外free）
 */
char * snum_getstring(snum_t *snum)
{
    int i = 0;
    char *ret = NULL, *retnum = NULL;
    
    ret = calloc(snum->len+2, sizeof(char));
    if (ret == NULL) {
        exit(EXIT_FAILURE);
    }

    if (snum->sign != 0) {
        *ret = '-';
        retnum = ret+1;
    }
    else {
        retnum = ret;
    }
    
    for (i=0; i<(snum->dot_idx==0?snum->len-1:snum->len); i++) {
        *(retnum+i) = *(snum->str+snum->len-1-i);
    }
    
    return ret;
}

/// 除法的一次运算，得一位商值
/**
 * 0 < snum1 且 snum1 < snum2 * 10。snum1/snum2 得一位商值和本次余数。
 *
 * @param snum1 本次除数（运行结束后存储余数）
 * @param snum2 本次被除数
 * @retval char 商值（数字字符）
 */
static int  _snum_divide(snum_t *snum1, snum_t *snum2)
{
    int i = 0;
    snum_t *tmp = NULL, *tmp2 = NULL;

    if (snum_compare(snum1, snum2) == 0) {
        for (i=2; i<11; i++) {
            tmp = __snum_multiple(snum2, i+48);
            if (snum_compare(snum1, tmp) == 0) {
                snum_free(tmp);
                tmp = NULL;
                continue;
            }
            else {
                snum_free(tmp);
                tmp = NULL;
                tmp = __snum_multiple(snum2, i-1+48);
                tmp2 = _snum_minus(snum1, tmp);
                memcpy(snum1, tmp2, sizeof(snum_t));
                free(tmp2);   // 仅仅free snum_t 结构，保留snum_t->str内存空间
                tmp2 = NULL;
                return i-1;
            }
        }
    }
    else {
        return 0;
    }
    exit(EXIT_FAILURE);
}

/// 字符数除法
/**
 * @param snum1 指向待操作字符数结构的指针，除数
 * @param snum2 指向待操作字符数结构的指针，被除数
 * @retval snum_t* 结果返回值，一个已分配内存的snum_t结构
 */
snum_t *snum_divide3(snum_t *snum1, snum_t *snum2, int precision) 
{
    int i = 0, j = 0, idx = 0, is_enter_frac = 0;
    snum_t *reminder = NULL, *retval = NULL;
    char *tmp = NULL, *ch = NULL, *retch = NULL;

    tmp = calloc(snum2->len+11, sizeof(char));
    retch = calloc(5000, sizeof(char));
    if (tmp == NULL || retch == NULL) {
        exit(EXIT_FAILURE);
    }
    reminder = snum_new2("0", snum2->len+1);

    __offset_dot(snum1, snum2->dot_idx);
    __offset_dot(snum2, snum2->dot_idx);
    
    is_enter_frac = 0;
    if (precision == 0)
        precision = snum1->dot_idx+10;
    for (j=0,idx=snum1->len-1,i=0; i<precision; i++,idx--) {
        if (!is_enter_frac && i > 0)
            i--;
        if (i>0 && strcmp(ch = snum_getstring(reminder), "0") == 0) {
            free(ch);
            break;
        }
        if (i>0)
            free(ch);

        if (idx >=0 && *(snum1->str+idx) == '.') {
            *(retch+j++) = '.';
            is_enter_frac = 1;
        }
        else {
            ch = snum_getstring(reminder);
            strcpy(tmp, ch);
            free(ch);
            *(tmp + strlen(tmp) + 1) = '\0';
            if (idx >=0) {
                *(tmp + strlen(tmp)) = *(snum1->str+idx);
            }
            else {
                *(tmp + strlen(tmp)) = '0';
            }
            snum_set(reminder, tmp);
            *(retch+j++) = _snum_divide(reminder, snum2) + 48;
        }
    }
    retval = snum_new1(retch);
    retval->sign = snum1->sign + snum2->sign;
    retval->sign = retval->sign==2?0:retval->sign;
    snum_free(reminder);
    free(tmp);
    free(retch);
    return retval;
}

/// 字符数乘法
/**
 * @param snum1 指向待操作字符数结构的指针，乘数
 * @param snum2 指向待操作字符数结构的指针，被乘数
 * @retval snum_t* 结果返回值，一个已分配内存的snum_t结构
 */
snum_t *snum_multiple(snum_t *snum1, snum_t *snum2)
{
    snum_t *loop = NULL;
    snum_t *temp_storage = NULL, *temp = NULL;
    int i = 0, offset = 0;

    temp_storage = snum_new1("0");
    for (i=0,offset=0; i<snum2->len; i++) {
        if (*(snum2->str+i) == '.') {
            continue;
        }
        else {
            loop = __snum_multiple(snum1, *(snum2->str+i));
            __offset_dot(loop, offset++);
            temp = snum_plus(loop, temp_storage);
            snum_free(loop);
            snum_free(temp_storage);
            temp_storage = temp;
            temp = NULL;
            loop = NULL;
        }
    }

    temp_storage->sign = snum1->sign + snum2->sign;
    temp_storage->sign = temp_storage->sign==2?0:temp_storage->sign;
    __offset_dot(temp_storage, -snum2->dot_idx);
    wipe_tail_zero(temp_storage);
    return temp_storage;
}

/// 字符数加法
/**
 * @param snum1 指向待操作字符数结构的指针，加数
 * @param snum2 指向待操作字符数结构的指针，被加数
 * @retval snum_t* 结果返回值，一个已分配内存的snum_t结构
 */
snum_t * snum_plus(snum_t *snum1, snum_t *snum2)
{
    snum_t *ret = NULL;
    
    if (snum1->sign == 0 && snum2->sign == 0) {
        ret = _snum_plus(snum1, snum2);
    }
    else if (snum1->sign == 0 && snum2->sign != 0) {
        if (snum_compare(snum1, snum2) == 0) {
            ret = _snum_minus(snum1, snum2);
        }
        else {
            ret = _snum_minus(snum2, snum1);
            ret->sign = 1;
        }
    }
    else if(snum1->sign != 0 && snum2->sign == 0) {
        if (snum_compare(snum1, snum2) == 0) {
            ret = _snum_minus(snum1, snum2);
            ret->sign = 1;
        }
        else {
            ret = _snum_minus(snum2, snum1);
        }
    }
    else {
        ret = _snum_plus(snum1, snum2);
        ret->sign = 1;
    }
    
    return ret;
}

/// 字符数减法
/**
 * @param snum1 指向待操作字符数结构的指针，减数
 * @param snum2 指向待操作字符数结构的指针，被减数
 * @retval snum_t* 结果返回值，一个已分配内存的snum_t结构
 */
snum_t * snum_minus(snum_t *snum1, snum_t *snum2)
{
    snum_t *ret = NULL;
    
    if (snum1->sign == 0 && snum2->sign == 0) {
        if (snum_compare(snum1, snum2) == 0) {
            ret = _snum_minus(snum1, snum2);
        }
        else {
            ret = _snum_minus(snum2, snum1);
            ret->sign = 1;
        }
    }
    else if (snum1->sign == 0 && snum2->sign != 0) {
        ret = _snum_plus(snum1, snum2);
    }
    else if(snum1->sign != 0 && snum2->sign == 0) {
        ret = _snum_plus(snum1, snum2);
        ret->sign = 1;
    }
    else {
        if (snum_compare(snum1, snum2) == 0) {
            ret = _snum_minus(snum1, snum2);
            ret->sign = 1;
        }
        else {
            ret = _snum_minus(snum2, snum1);
        }
    }
    
    return ret;
}

