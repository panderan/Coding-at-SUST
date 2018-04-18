#ifndef _DATAREADER_FACTORY_
#define _DATAREADER_FACTORY_

#include "datareader_interface.h"

class datareader_factory
{
    public:
        static datareader_interface * get_datareader();
};

#endif 
