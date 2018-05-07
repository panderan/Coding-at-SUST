#ifdef _DEBUG_ON_
#include <iostream>
#endif

#include "dataprocess_factory.h"
#include "display_dataprocess.h"

REGISTER_TO_A_FACTORY(dataprocess_factory, display_dataprocess, "display", dataprocess_interface)
REFLECT_IMPLEMENT(dataprocess_interface, display_dataprocess)
    
void display_dataprocess::process(matrix_list &mlist, 
                            matrix_list &olist)
{
    process_display(mlist, olist);        
}

void display_dataprocess::process_display(matrix_list &mlist, matrix_list &olist)
{
    s_graph *tmp = NULL;

    while (!mlist.empty()) {
        tmp = mlist.front();
        mlist.pop_front();
        olist.push_back(tmp);
    }
}

