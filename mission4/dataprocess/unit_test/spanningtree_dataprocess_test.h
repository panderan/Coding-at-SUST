#ifndef _SPANNINGTREE_DATAPROCESS_TEST_ 
#define _SPANNINGTREE_DATAPROCESS_TEST_

#include <iostream>
#include <cppunit/extensions/HelperMacros.h>
#include "../spanningtree_dataprocess.h"

class spanningtree_dataprocess_test : public CppUnit::TestFixture
{
    CPPUNIT_TEST_SUITE(spanningtree_dataprocess_test);
    CPPUNIT_TEST(process_test);
    CPPUNIT_TEST_SUITE_END();

    public:
        void setUp();
        void tearDown();

    public:
        void process_test();
};





#endif 
