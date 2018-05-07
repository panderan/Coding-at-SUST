#ifndef _NEATO_DATAOUTPUT_TEST_ 
#define _NEATO_DATAOUTPUT_TEST_

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>

class neato_dataoutput_test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(neato_dataoutput_test);
    CPPUNIT_TEST(output_grpadj_test);
    CPPUNIT_TEST(output_grpinc_test);
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

    public:
        void output_grpadj_test();
        void output_grpinc_test();
};





#endif 
