#ifndef _NEATO_DATAOUTPUT_ 
#define _NEATO_DATAOUTPUT_

#include "dataoutput_interface.h"

class neato_dataoutput: public dataoutput_interface
{
    REFLECT_DECLARE(dataoutput_interface)

    public:
        virtual void output_graph(matrix_list &glist);

    private:
        void output_graph_bsc_inc_mat(const char * cwd, s_graph *g);
        void output_graph_adj_mat(const char * cwd, s_graph *g);
        void output_mkdir(const char *tgt);
};

#endif 
