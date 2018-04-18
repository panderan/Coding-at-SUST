#ifndef _ABSTRACT_DATAREADER_
#define _ABSTRACT_DATAREADER_

#include "graph_struct.h"

class datareader_interface {
    public:
        virtual void read(string filepath, 
                            matrix_list &mlist, 
                            expr_list &elist) = 0;
};

#endif
