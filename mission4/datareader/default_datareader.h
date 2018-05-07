#ifndef _DEFAULT_DATAREADER_
#define _DEFAULT_DATAREADER_

#include "datareader_interface.h"

class default_datareader : public datareader_interface {

    REFLECT_DECLARE(datareader_interface)

    public:
        virtual void read(string filepath, 
                            matrix_list &mlist, 
                            expr_list &elist);
    private:
        void read_graph(FILE *fp, s_graph *graph);
        void check_grpadj(const s_graph * const graph);
        void check_grpinc(const s_graph * const graph);
};


#endif 
