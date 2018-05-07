#ifndef _SPANNINGTREE_DATAPROCESS_ 
#define _SPANNINGTREE_DATAPROCESS_

#include "display_dataprocess.h"

class spanningtree_dataprocess: public display_dataprocess 
{
    REFLECT_DECLARE(dataprocess_interface)

    public:
        virtual void process(matrix_list &mlist, 
                                matrix_list &olist);
    protected:
        void process_spanningtree(matrix_list &mlist, matrix_list &olist);
        void get_basic_incidence_matrix(MatrixXi &inc_mat, MatrixXi adj_mat);
        void get_spanning_tree(const char * gname, MatrixXi inc_mat, matrix_list &olist);
        int  dectect_spanning_tree(int *idx, MatrixXi inc_mat, MatrixXi &st);
};

#endif 
