#include "datareader_factory.h"
#include "datareader1.h"

datareader_interface * datareader_factory::get_datareader()
{
    return (datareader_interface *) new datareader1();
}
