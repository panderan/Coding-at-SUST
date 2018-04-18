#ifndef _DATAOUTPUT_INTERFACE_
#define _DATAOUTPUT_INTERFACE_

#include "graph_struct.h"

class dataoutput_interface
{
    public:
        virtual void output_graph(matrix_list &glist) = 0;
};

#endif
