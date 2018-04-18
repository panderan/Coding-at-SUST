#ifndef _DATAPROCESS_FACTORY_
#define _DATAPROCESS_FACTORY_

#include "dataprocess_interface.h"

class dataprocess_factory 
{
    public:
        static dataprocess_interface * get_dataprocess();
};

#endif
