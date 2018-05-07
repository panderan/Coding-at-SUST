#ifndef _DATAREADER_FACTORY_
#define _DATAREADER_FACTORY_

#include "datareader_interface.h"

class datareader_factory
{
    REFLECT_FACTORY_CONSTRUCTOR(datareader_factory, datareader_interface)

    public:
        static datareader_interface * get_datareader(const char *name = "default");
};

#endif 
