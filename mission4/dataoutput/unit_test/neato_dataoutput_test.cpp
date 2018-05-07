#include "../neato_dataoutput.h"
#include "datareader_factory.h"
#include "default_datareader_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(neato_dataoutput_test);

void neato_dataoutput_test::setUp()
{
    return;
}

void neato_dataoutput_test::tearDown()
{
    return;
}

void neato_dataoutput_test::output_grpadj_test()
{
    matrix_list mlist;
    expr_list elist;
    datareader_interface *dr = NULL;
    int cnt = 0, row = 0, col = 0;

    // 读取邻接矩阵
    dr = datareader_factory::get_datareader();
    dr->read("../../../datareader/unit_test/read_grpadj.test", mlist, elist);

    // 测试输出邻接矩阵
    neato_dataoutput nd;
    cout<<endl<<"  测试输出邻接矩阵图"<<endl;
    nd.output_graph(mlist);
    return;
}

void neato_dataoutput_test::output_grpinc_test()
{
    matrix_list mlist;
    expr_list elist;
    datareader_interface *dr = NULL;
    int cnt = 0, row = 0, col = 0;

    // 读取邻接矩阵
    dr = datareader_factory::get_datareader();
    dr->read("../../../datareader/unit_test/read_grpinc.test", mlist, elist);

    // 测试输出邻接矩阵
    neato_dataoutput nd;
    cout<<endl<<"  测试输出关联矩阵图"<<endl;
    nd.output_graph(mlist);
    return;
}

