#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "list.h"
#include "expression_eval.h"

#define TRUE 0
#define FALSE 1

#define NODE_TYPE_OPERATOR 1
#define NODE_TYPE_OPERATOR_PROCESSED 2
#define NODE_TYPE_NUM 3 
#define NODE_TYPE_UNKNOW 4

#define OPERATOR_PLUS 10
#define OPERATOR_MINUS 11
#define OPERATOR_MULTIPLY 12
#define OPERATOR_DIVIDE 13
#define OPERATOR_LEFT_BRACKET 14
#define OPERATOR_RIGHT_BRACKET 15
#define OPERATOR_POWER 16
#define OPERATOR_SIN 17
#define OPERATOR_COS 18
#define OPERATOR_TAN 19
#define OPERATOR_UNKOWN 20

typedef int node_type;

typedef struct bintree {
    struct list_head list;
    struct bintree *parent;
    struct bintree *lchild;
    struct bintree *rchild;
    node_type type;         /**< 节点类型 */
    double value;           /**< 数值     */
    int opt_type;           /**< 操作符类型 */
    char opt_str[6];        /**< 操作符字符 */
    
}bintree_t;

void init_bintree_node (bintree_t *node) 
{
    INIT_LIST_HEAD(&node->list);
    node->parent = NULL;
    node->lchild = NULL;
    node->rchild = NULL;
    node->type = NODE_TYPE_UNKNOW;
    node->opt_type = OPERATOR_UNKOWN;
    node->value = 0;
    node->opt_str[0] = '\0';
}

int is_operator(char *str) {
    switch (*str) {
        case '-':
            return OPERATOR_MINUS;
            break;
        case '+':
            return OPERATOR_PLUS;
            break;
        case '*':
            return OPERATOR_MULTIPLY;
            break;
        case '/':
            return OPERATOR_DIVIDE;
            break;
        case '^':
            return OPERATOR_POWER;
            break;
        case '(':
            return OPERATOR_LEFT_BRACKET;
            break;
        case ')':
            return OPERATOR_RIGHT_BRACKET;
            break;
        case 's':
            if (*(str+1) == 'i' && *(str+2) == 'n')
                return OPERATOR_SIN;
            else 
                return OPERATOR_UNKOWN;
            break;
        case 'c':
            if (*(str+1) == 'o' && *(str+2) == 's')
                return OPERATOR_COS;
            else 
                return OPERATOR_UNKOWN;
            break;
        case 't':
            if (*(str+1) == 'a' && *(str+2) == 'n')
                return OPERATOR_TAN;
            else 
                return OPERATOR_UNKOWN;
            break;
        default:
            return OPERATOR_UNKOWN;
    }
}

int is_num(char *str)
{
    if (*str == '.')
        return TRUE;
    if (*str < '0' || *str > '9')
        return FALSE;
    else 
        return TRUE;
}

bintree_t * parse_expression_string(char *string)
{
    int i = 0, j = 0;
    bintree_t *root = NULL;
    bintree_t *node = NULL;
    char buffer[100];
    node_type type;

    root = calloc(1, sizeof(bintree_t));
    init_bintree_node(root);

    for (i=0; *(string+i)!='\0'; i+=j) {
        j=0;
        if (is_num(string+i) == TRUE) {
            for (j++; is_num(string+i+j) == TRUE; j++);
            memcpy(buffer, string+i, j);
            buffer[j]='\0';
            node = calloc(1, sizeof(bintree_t));
            init_bintree_node(node);
            node->value = atof(buffer);
            node->type = NODE_TYPE_NUM;
            list_add_tail(&node->list, &root->list);
        }
        else if ((type=is_operator(string+i)) != OPERATOR_UNKOWN) {
            if (type == OPERATOR_SIN || type == OPERATOR_COS || type == OPERATOR_TAN)
                j+=3;
            else 
                j++;
            node = calloc(1, sizeof(bintree_t));
            init_bintree_node(node);

            // 处理负数
            if (*(string+i) == '-') {
                if (i == 0 || (i!=0 
                            && is_operator(string+i-1)!=OPERATOR_UNKOWN
                            && is_num(string+i+1) == TRUE)) {
                    for (; is_num(string+i+j) == TRUE; j++);
                    memcpy(buffer, string+i, j);
                    buffer[j]='\0';
                    node = calloc(1, sizeof(bintree_t));
                    init_bintree_node(node);
                    node->value = atof(buffer);
                    node->type = NODE_TYPE_NUM;
                    list_add_tail(&node->list, &root->list);
                    continue;
                }
                
            }

            node->type = NODE_TYPE_OPERATOR;
            node->opt_type = type;
            memcpy(node->opt_str, string+i, j);
            node->opt_str[j] = '\0';
            list_add_tail(&node->list, &root->list);
        }
        else {
            printf("ERROR in parse expression string\n");
            exit(0);
        }
    }

    return root;
}

bintree_t *build_bintree(bintree_t *root) {

    bintree_t *loop = NULL, *safe = NULL;
    bintree_t *sub_expression = NULL;
    int intered_backet = FALSE;

    // 去括号
    // 遍历所有元素节点，寻找括号节点
    list_for_each_entry_safe(loop, safe, &root->list, list) {

        // 发现右括号，子表达式完毕，递归处理。
        if (loop->type == NODE_TYPE_OPERATOR && 
                loop->opt_type == OPERATOR_RIGHT_BRACKET) {
            intered_backet = FALSE;
            build_bintree(sub_expression);
            list_add_tail(sub_expression->list.next, &loop->list);
            list_del(&loop->list);
            free(loop);
            free(sub_expression);
            loop = NULL;
            sub_expression = NULL;
        }

        // 处于子串中，将该节点子表达式链表中
        else if (intered_backet == TRUE) {
            list_del(&loop->list);
            list_add_tail(&loop->list, &sub_expression->list);
        }
        

        // 发现右括号节点，将该节点作为子表达式的root节点。
        else if (loop->type == NODE_TYPE_OPERATOR && 
                loop->opt_type == OPERATOR_LEFT_BRACKET) {
            intered_backet = TRUE;
            list_del(&loop->list);
            sub_expression = loop;
            init_bintree_node(sub_expression);
        }
        else {
            continue;
        }
    }

    // 处理乘除，幂方，三角函数
    list_for_each_entry(loop, &root->list, list) {
        if (loop->type == NODE_TYPE_OPERATOR && 
                (loop->opt_type == OPERATOR_MULTIPLY 
                 || loop->opt_type == OPERATOR_DIVIDE
                 || loop->opt_type == OPERATOR_POWER)) {
            loop->lchild = list_entry(loop->list.prev, bintree_t, list);
            loop->rchild = list_entry(loop->list.next, bintree_t, list);
            list_del(loop->list.prev);
            list_del(loop->list.next);
            loop->type = NODE_TYPE_OPERATOR_PROCESSED;
        }
    }

    // 处理三角函数
    list_for_each_entry(loop, &root->list, list) {
        if (loop->type == NODE_TYPE_OPERATOR && 
                (loop->opt_type == OPERATOR_COS 
                 || loop->opt_type == OPERATOR_SIN
                 || loop->opt_type == OPERATOR_TAN)) {
            loop->lchild = list_entry(loop->list.next, bintree_t, list);
            loop->rchild = NULL;
            list_del(loop->list.next);
            loop->type = NODE_TYPE_OPERATOR_PROCESSED;
        }
    }

    // 处理加减
    list_for_each_entry(loop, &root->list, list) {
        if (loop->type == NODE_TYPE_OPERATOR && 
                (loop->opt_type == OPERATOR_PLUS 
                 || loop->opt_type == OPERATOR_MINUS)) {
            loop->lchild = list_entry(loop->list.prev, bintree_t, list);
            loop->rchild = list_entry(loop->list.next, bintree_t, list);
            list_del(loop->list.prev);
            list_del(loop->list.next);
            loop->type = NODE_TYPE_OPERATOR_PROCESSED;
        }
    }

    return root;

}

double _expression_eval(bintree_t *node) 
{
    double left, right;
    if (node->type != NODE_TYPE_NUM) {

        if (node->opt_type == OPERATOR_COS 
                || node->opt_type == OPERATOR_SIN
                || node->opt_type == OPERATOR_TAN) {
            if (node->lchild->type != NODE_TYPE_NUM) {
                left = _expression_eval(node->lchild);
            }
            else 
                left = node->lchild->value;

            switch (node->opt_type) {
                case OPERATOR_COS:
                    return cos(left);
                case OPERATOR_SIN:
                    return sin(left);
                case OPERATOR_TAN:
                    return tan(left);
                default:
                    exit(EXIT_FAILURE);
            }
        }
        else {
            if (node->lchild->type != NODE_TYPE_NUM) {
                left = _expression_eval(node->lchild);
            }
            else 
                left = node->lchild->value;
    
            if (node->rchild->type != NODE_TYPE_NUM) {
                right = _expression_eval(node->rchild);
            }
            else
                right = node->rchild->value;

            switch(node->opt_type) {
                case OPERATOR_PLUS:
                    return left+right;
                case OPERATOR_POWER:
                    return pow(left, right);
                case OPERATOR_DIVIDE:
                    return left / right;
                case OPERATOR_MINUS:
                    return left - right;
                case OPERATOR_MULTIPLY:
                    return left *right;
                default:
                    exit(EXIT_FAILURE);
            }
        }

    }
    else
        return node->value;

}

void free_bintree(bintree_t *node)
{
    if (node->lchild != NULL)
        free_bintree(node->lchild);

    if (node->rchild != NULL)
        free_bintree(node->rchild);

    free(node);
}

double expression_eval(char *str) 
{
    bintree_t *root = NULL;
    double value = 0;

    root = parse_expression_string(str);
    build_bintree(root);
    value = _expression_eval(list_entry(root->list.next, bintree_t, list));
    free_bintree(list_entry(root->list.next, bintree_t, list));
    free(root);

    return value;
}

#if 0 
// TEST
void middle_order(bintree_t *node)
{
    if (node->lchild != NULL)
        middle_order(node->lchild);

    if (node->type != NODE_TYPE_NUM)
        printf("(%s),", node->opt_str);
    else
        printf("(%.2f),", node->value);
    
    if (node->rchild != NULL)
        middle_order(node->rchild);
}


int main(int argc, char ** argv)
{
    bintree_t *root = NULL;
    bintree_t *loop = NULL;
    double value = 0;

    root = parse_expression_string("2^2");

#if 0
    list_for_each_entry(loop, &root->list, list) {
        if (loop->type == NODE_TYPE_OPERATOR)
            printf("%s ", loop->opt_str);
        else
            printf("%f ", loop->value);
    }
#endif
    
#if 0
    build_bintree(root);
    middle_order(list_entry(root->list.next, bintree_t, list));
    printf("\n");
#endif

#if 1
    build_bintree(root);
    value = _expression_eval(list_entry(root->list.next, bintree_t, list));
    printf("Value:%f\n", value);
    free_bintree(list_entry(root->list.next, bintree_t, list));
    free(root);
#endif

    return 0;

}
#endif

