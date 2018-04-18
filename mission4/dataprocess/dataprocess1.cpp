#ifdef _DEBUG_ON_
#include <iostream>
#endif

#include "dataprocess1.h"

void dataprocess1::process(matrix_list &mlist, 
                            expr_list &elist,
                            matrix_list &olist)
{
    string cmd;
    cmd = elist.front();

    if (strcasecmp(cmd.c_str(), "display") == 0) {
        process_display(mlist, olist);        
    }
    else if (strcasecmp(cmd.c_str(), "spanningtree") == 0) {
        process_spanningtree(mlist, olist);
    }
    else {
        fprintf(stderr, "Invalid command\n");
        exit(1);
    }

}

void dataprocess1::process_display(matrix_list &mlist, matrix_list &olist)
{
    s_graph *tmp = NULL;

    while (!mlist.empty()) {
        tmp = mlist.front();
        mlist.pop_front();
        olist.push_back(tmp);
    }
}

void dataprocess1::process_spanningtree(matrix_list &mlist, matrix_list &olist)
{
    s_graph *g = NULL;
    MatrixXi inc_mat;

    g = mlist.front();
    get_basic_incidence_matrix(inc_mat, g->m);
    get_spanning_tree(g->name, inc_mat, olist);
}

void dataprocess1::get_basic_incidence_matrix(MatrixXi &inc_mat, MatrixXi adj_mat)
{
    int row = 0, col = 0;
    int i = 0, j = 0;
    uint64_t cnt_edges = 0;

    // 统计边数
    for (i=0; i<adj_mat.rows(); i++) {
        for (j=0; j<adj_mat.cols(); j++) {
            if (i>j) {
                continue;
            }
            cnt_edges += adj_mat(i,j);
        }
    }

    // 构造基本关联矩阵
    inc_mat.resize(adj_mat.rows(), cnt_edges);
    cnt_edges = 0;
    for (i=0; i<adj_mat.rows(); i++) {
        for (j=0; j<adj_mat.cols(); j++) {
            if (i>j) {
                continue;
            }
            if (adj_mat(i,j)) {
                inc_mat(i, cnt_edges) = 1;
                inc_mat(j, cnt_edges) = 1;
                cnt_edges += adj_mat(i,j);
            }
        }
    }
    inc_mat.conservativeResize(adj_mat.rows()-1, cnt_edges);

#ifdef _DEBUG_ON_
    cout<<"inc mat:"<<endl<<inc_mat<<endl;
#endif

}

void dataprocess1::get_spanning_tree(const char * gname, MatrixXi inc_mat, matrix_list &olist)
{
    int rows = 0, cols = 0; // n中选m个，n>m
    int *n = NULL, *m = NULL;
    int *pckon = NULL;
    int i= 0, j = 0;
    int flag = 0;
    int tree_cnt = 0; 
    s_graph *s_tree = NULL;

    rows = inc_mat.rows();
    cols = inc_mat.cols();
    if (rows > cols) {
        n = &rows;
        m = &cols;
    }
    else {
        n = &cols;
        m = &rows;
    }

    // 初始化排列组合
    pckon = (int *) calloc(*m, sizeof(int));
    for (i=0; i<*m; i++) {
        *(pckon+i) = i;
    };


    // 循环枚举
    while (1) {
#ifdef _DEBUG_ON_
        // 打印输出枚举
        for(i=0; i<*m; i++) {
            printf("%d ", *(pckon+i));
        }
        printf("\n");
#endif
        
        // 检测是否是生成树
        if (s_tree == NULL) {
            s_tree = (s_graph *) calloc(1, sizeof(s_graph));
            if (s_tree == NULL) {
                fprintf(stderr, "Out of memory\n");
                exit(1);
            }
            s_tree->grp_type = BASIC_INCIDENCE_MATRIX;
        }
        if (dectect_spanning_tree(pckon, inc_mat, s_tree->m) != 0) {
            sprintf(s_tree->name, "%s_tree%d", gname, ++tree_cnt);
            olist.push_back(s_tree);
            s_tree = NULL;
        }

        // 计算下一个枚举
        flag = 0;
        for (i=*m-1; i>=0; i--) { if (*(pckon+i) > *n-*m+i-1) {
                if (i == 0) {
                    return ;
                }
                flag = 1;
                continue;
            } 
            else {
                if (flag == 0) {
                    break;
                }
                else {
                    (*(pckon+i))++;
                    for (j=i+1;j<*m;j++) {
                        *(pckon+j) = *(pckon+j-1) + 1;
                    }
                    break;
                }
            }
        }
        if (flag == 0) {
            (*(pckon+*m-1))++;
        }
    }
    return ;
}

int dataprocess1::dectect_spanning_tree(int *idx, MatrixXi inc_mat, MatrixXi &st)
{
    int flag_transposition = 0;
    int m = 0, n = 0;
    int i = 0, j = 0, p = 0;
    MatrixXd smtx;

    if (inc_mat.rows() > inc_mat.cols()) {
        inc_mat.transposeInPlace();
        flag_transposition = 1;
    }
    m = inc_mat.rows();

    // 找出自主阵
    smtx.resize(m, m);
    //fprintf(stderr, "inc_mat size rows:%d, cols:%d\n", inc_mat.rows(), inc_mat.cols());
    for (i=0, p=0; i<m; i++) {
        for (j=0; j<m; j++) {
            //fprintf(stderr, "stm(%d,%d) = inc_mat(%d, %d)\n", j, i, j, *(idx+p));
            smtx(j,i) = (double) inc_mat(j, *(idx+p));
        }
        p++;
    }

    // 判断自主阵是否非奇异
#ifdef _DEBUG_ON_
    cout<<"子主阵："<<endl<<smtx<<endl<<endl;
#endif
    if (smtx.determinant() != 0) {
        if (flag_transposition) {
            smtx.transposeInPlace();
        }
        st.resize(m, m);
        for (i=0; i<m; i++) {
            for (j=0; j<m; j++) {
                st(i,j) = int(smtx(i,j)+0.5);
            }
        }
        return 1;
    }
    return 0;
}























