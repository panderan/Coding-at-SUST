#ifndef _DATAPROCESS_FACTORY_
#define _DATAPROCESS_FACTORY_

#include "dataprocess_interface.h"

class dataprocess_factory 
{
    REFLECT_FACTORY_CONSTRUCTOR(dataprocess_factory, dataprocess_interface)

    public:
        static dataprocess_interface * get_dataprocess(const char *name = "display");
};

#endif
