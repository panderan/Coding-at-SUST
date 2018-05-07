#ifndef _DISPLAY_DATAPROCESS_
#define _DISPLAY_DATAPROCESS_

#include "dataprocess_interface.h"

class display_dataprocess: public dataprocess_interface
{
    REFLECT_DECLARE(dataprocess_interface)

    public:
        virtual void process(matrix_list &mlist, 
                                matrix_list &olist);
    protected:
        void process_display(matrix_list &mlist, matrix_list &olist);
};

#endif 
