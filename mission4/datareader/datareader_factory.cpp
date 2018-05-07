#include "datareader_factory.h"
#include "default_datareader.h"

DECLEARE_REGISTATION(datareader_factory, datareader_interface)
REFLECT_FACTORY_CONSTRUCTOR_IMPLEMENT(datareader_factory, datareader_interface)

datareader_interface * datareader_factory::get_datareader(const char *name)
{
    datareader_interface *ret = NULL;

    SEARCH_REGISTATION(datareader_factory, name, datareader_interface, ret)
    return ret;
}

