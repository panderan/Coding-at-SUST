#ifndef _DATAOUTPUT_INTERFACE_
#define _DATAOUTPUT_INTERFACE_

#include "graph_struct.h"
#include "reflect_define.h"

class dataoutput_interface
{
    REFLECT_INTERFACE(dataoutput_interface)

    public:
        virtual void output_graph(matrix_list &glist) = 0;
};

#endif
