#ifndef _DEFAULT_DATAREADER_TEST_ 
#define _DEFAULT_DATAREADER_TEST_

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "../default_datareader.h"

class default_datareader_test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(default_datareader_test);
    CPPUNIT_TEST(read_grpadj_test);
    CPPUNIT_TEST(read_grpinc_test);
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

    public:
        void read_grpadj_test();
        void read_grpinc_test();
};





#endif 
