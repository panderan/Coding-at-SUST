#include "dataprocess_factory.h"
#include "dataprocess1.h"

dataprocess_interface * dataprocess_factory::get_dataprocess()
{
    return (dataprocess_interface *) new dataprocess1();
}
