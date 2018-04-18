#ifndef _DATAREADER_1_
#define _DATAREADER_1_

#include "datareader_interface.h"

class datareader1 : public datareader_interface {
    public:
        virtual void read(string filepath, 
                            matrix_list &mlist, 
                            expr_list &elist);
    private:
        void read_graph(FILE *fp, s_graph *graph);
};


#endif 
