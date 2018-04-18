#ifndef _DATAPROCESS_INTERFACE_
#define _DATAPROCESS_INTERFACE_

#include "graph_struct.h"

class dataprocess_interface 
{
    public:
        virtual void process(matrix_list &mlist, 
                                expr_list &elist,
                                matrix_list &olist) = 0;
};

#endif
