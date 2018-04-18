#ifndef _DATAPROCESS1_
#define _DATAPROCESS1_

#include "dataprocess_interface.h"

class dataprocess1: public dataprocess_interface
{
    public:
        virtual void process(matrix_list &mlist, 
                                expr_list &elist,
                                matrix_list &olist);
    private:
        void process_display(matrix_list &mlist, matrix_list &olist);
        void process_spanningtree(matrix_list &mlist, matrix_list &olist);
        void get_basic_incidence_matrix(MatrixXi &inc_mat, MatrixXi adj_mat);
        void get_spanning_tree(const char * gname, MatrixXi inc_mat, matrix_list &olist);
        int  dectect_spanning_tree(int *idx, MatrixXi inc_mat, MatrixXi &st);
};

#endif 
