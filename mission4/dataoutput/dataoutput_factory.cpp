#include "dataoutput_factory.h"
#include "dataoutput1.h"

dataoutput_interface * dataoutput_factory::get_dataoutput()
{
    return (dataoutput_interface *)new dataoutput1();
}
