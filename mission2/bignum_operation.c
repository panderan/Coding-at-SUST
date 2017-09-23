#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "bignum_operation.h"

snum_t *snum_new(char *str)
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
	snum->str = calloc(snum->len+2, sizeof(char));
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

	return snum;
}

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

char * snum_getstring(snum_t *snum)
{
	int i = 0, numlen = 0;
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

static snum_t * snum_align(snum_t *snum1, snum_t *snum2)
{
	snum_t *ret = NULL, *ret1 = NULL, *ret2 = NULL;
	int fractional_alignlen = 0, integer_alignlen = 0;
	int i = 0;

	fractional_alignlen = snum1->dot_idx > snum2->dot_idx ? 
		snum1->dot_idx : snum2->dot_idx;
	integer_alignlen = snum1->len - snum1->dot_idx > snum2->len - snum2->dot_idx ?
		snum1->len - 1 - snum1->dot_idx : snum2->len - 1 - snum2->dot_idx;

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

// @retval  0  snum1 >= snum2
// @retval  1  snum1 <  snum2
static int snum_compare(snum_t *snum1, snum_t *snum2)
{
	int i = 0, j = 0;

	if (snum1->len - snum1->dot_idx > snum2->len - snum2->dot_idx) {
		return 0;
	}
	else if (snum1->len - snum1->dot_idx == snum2->len - snum2->dot_idx) {
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

static int __snum_plus(char n1, char n2, int is_carry, char *res)
{
	*res = n1 + n2 - 48;
	if (is_carry) {
		(*res)++;
	}

	if (*res > 57) {
		*res -= 10;
		return 1;
	}
	else {
		return 0;
	}
}

static snum_t * _snum_plus(snum_t *snum1, snum_t *snum2)
{
	snum_t *optnum = NULL, *ret_snum = NULL;
	int i = 0, is_carry = 0;
	
	ret_snum = calloc(1, sizeof(snum_t));
	if (ret_snum == NULL) {
		exit(EXIT_FAILURE);
	}
	ret_snum->str = calloc(ret_snum->len+1, sizeof(char));
	if (ret_snum->str == NULL) {
		free(optnum);
		exit(EXIT_FAILURE);
	}
	
	optnum = snum_align(snum1, snum2);
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

static int __snum_minus(char n1, char n2, int is_borrow, char *res)
{
	*res = n1 - n2 + 48;
	if (is_borrow) {
		(*res)--;
	}
	
	if (*res >= 48) {
		return 0;
	}
	else {
		(*res) += 10;
		return 1;
	}
}

// 必须是大减小 且都是正数
static snum_t * _snum_minus(snum_t *snum1, snum_t *snum2)
{
	snum_t *optnum = NULL, *ret_snum = NULL;
	int i = 0, is_borrow = 0;
	
	ret_snum = calloc(1, sizeof(snum_t));
	if (ret_snum == NULL) {
		exit(EXIT_FAILURE);
	}
	ret_snum->str = calloc(ret_snum->len+1, sizeof(char));
	if (ret_snum->str == NULL) {
		free(optnum);
		exit(EXIT_FAILURE);
	}

	optnum = snum_align(snum1, snum2);
	for (i=0; i<optnum->len; i++) {
		if (*(optnum->str+i) == '.') {
			*(ret_snum->str+i) = '.';
			continue;
		}
		is_borrow = __snum_minus(*(optnum->str+i), *((optnum+1)->str+i), is_borrow, ret_snum->str+i);
	}
	ret_snum->dot_idx = optnum->dot_idx;
	ret_snum->len = optnum->len;

	// 去除前导0 
	for (i=ret_snum->len-1; i>=0; i--) {
		if (*(ret_snum->str+i) == '0') {
			if (i!=0 && *(ret_snum->str+i-1) != '.') {
				ret_snum->len--;
			}
			else {
				break;
			}
		}
		else {
			break;
		}
	}
	return ret_snum;
}

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


