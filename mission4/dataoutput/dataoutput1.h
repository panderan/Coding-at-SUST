#ifndef _DATAOUTPUT1_
#define _DATAOUTPUT1_

#include "dataoutput_interface.h"

class dataoutput1: public dataoutput_interface
{
    public:
        virtual void output_graph(matrix_list &glist);

    private:
        void output_graph_bsc_inc_mat(const char * cwd, s_graph *g);
        void output_graph_adj_mat(const char * cwd, s_graph *g);
        void output_mkdir(const char *tgt);
};

#endif 
