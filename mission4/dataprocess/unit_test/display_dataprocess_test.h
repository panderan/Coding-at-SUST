#ifndef _DISPLAY_DATAPROCESS_TEST_
#define _DISPLAY_DATAPROCESS_TEST_

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "datareader_factory.h"
#include "../display_dataprocess.h"

class display_dataprocess_test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(display_dataprocess_test);
    CPPUNIT_TEST(process_test);
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

    public:
        void process_test();
};





#endif 
