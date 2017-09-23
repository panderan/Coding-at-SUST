#ifndef BIGNUM_OPERATION_H
#define BIGNUM_OPERATION_H

typedef struct snum {
	char * str;
	int len;
	int dot_idx;
	int sign;     // 0 为正， 非0 为负;
} snum_t;

// 由数字字符串构建一个字符数结构（struct snum）
snum_t *snum_new(char *str);

// 释放一个字符数结构
void snum_free(snum_t *snum);

// 将一个字符数转化为一个字符已分配数组（alloced）
char * snum_getstring(snum_t *snum);

// 将两个字符数结构进行加法运行得到一个新的已分配的字符数结构（alloced）
snum_t * snum_plus(snum_t *snum1, snum_t *snum2);

// 将两个字符数结构进行减法运行得到一个新的已分配的字符数结构（alloced）
snum_t * snum_minus(snum_t *snum1, snum_t *snum2);


// 测试宏
#define TEST_SNUM_PLUS(str1, str2, p1, p2, p3, ch) \
			printf("%s + %s = %s\n", (str1), (str2), \
			ch=snum_getstring(p3=snum_plus(p1=snum_new(str1), p2=snum_new(str2)))); \
			snum_free(p1), snum_free(p2), snum_free(p3), free(ch);
#define TEST_SNUM_MINUS(str1, str2, p1, p2, p3, ch) \
			printf("%s - %s = %s\n", (str1), (str2), \
			ch=snum_getstring(p3=snum_minus(p1=snum_new(str1), p2=snum_new(str2)))); \
			snum_free(p1), snum_free(p2), snum_free(p3), free(ch);

#endif

