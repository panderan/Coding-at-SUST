#ifndef _DATAOUTPUT_FACTORY_
#define _DATAOUTPUT_FACTORY_

#include "dataoutput_interface.h"

class dataoutput_factory
{
    REFLECT_FACTORY_CONSTRUCTOR(dataoutput_factory, dataoutput_interface)

    public:
        static dataoutput_interface * get_dataoutput(const char *name = "neato");
};

#endif
