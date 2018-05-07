#include "dataprocess_factory.h"
#include "display_dataprocess.h"
#include "spanningtree_dataprocess.h"

DECLEARE_REGISTATION(dataprocess_factory, dataprocess_interface)
REFLECT_FACTORY_CONSTRUCTOR_IMPLEMENT(dataprocess_factory, dataprocess_interface)

dataprocess_interface * dataprocess_factory::get_dataprocess(const char *name)
{
    dataprocess_interface * ret = NULL;

    SEARCH_REGISTATION(dataprocess_factory, name, dataprocess_interface, ret)
    return ret;
}
