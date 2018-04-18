#ifndef _DATAOUTPUT_FACTORY_
#define _DATAOUTPUT_FACTORY_

#include "dataoutput_interface.h"

class dataoutput_factory
{
    public:
        static dataoutput_interface * get_dataoutput();
};

#endif
