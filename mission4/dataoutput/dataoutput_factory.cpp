#include "dataoutput_factory.h"
#include "neato_dataoutput.h"

DECLEARE_REGISTATION(dataoutput_factory, dataoutput_interface)
REFLECT_FACTORY_CONSTRUCTOR_IMPLEMENT(dataoutput_factory, dataoutput_interface)

dataoutput_interface * dataoutput_factory::get_dataoutput(const char *name)
{
    dataoutput_interface * ret = NULL;

    SEARCH_REGISTATION(dataoutput_factory, name, dataoutput_interface, ret)
    return ret;
}
