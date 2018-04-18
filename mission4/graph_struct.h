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

#endif
