#ifndef _GRAPH_STRUCT_
#define _GRAPH_STRUCT_

#include <list>
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

#define ADJACENCY_MATRIX 0
#define BASIC_INCIDENCE_MATRIX 1

typedef struct {
    char name[128];
    MatrixXi m;
    int grp_type;
} s_graph;

typedef list <s_graph *> matrix_list;
typedef list <string> expr_list;

#define REMOVE_MATRIX_LIST_ALL(m_list) \
    matrix_list::iterator iter; \
    for (iter = m_list.begin(); iter != m_list.end();) { \
        free(*(iter)); *(iter) = NULL; \
    } \
    m_list.clear();

#define REMOVE_EXPR_LIST_ALL(e_list) e_list.clear();

#endif
