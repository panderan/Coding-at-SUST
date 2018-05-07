#ifndef _DATAPROCESS_INTERFACE_
#define _DATAPROCESS_INTERFACE_

#include "graph_struct.h"
#include "reflect_define.h"

class dataprocess_interface 
{
    REFLECT_INTERFACE(dataprocess_interface)

    public:
        virtual void process(matrix_list &mlist, 
                                matrix_list &olist) = 0;
};

#endif
