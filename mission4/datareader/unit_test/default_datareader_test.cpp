#include "default_datareader_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(default_datareader_test);

void default_datareader_test::setUp()
{
    return;
}

void default_datareader_test::tearDown()
{
    return;
}

void default_datareader_test::read_grpadj_test()
{
    matrix_list mlist;
    expr_list elist;
    default_datareader dr;
    int cnt = 0, row = 0, col = 0;

    // 图的数量
    cout<<endl<<"测试读取邻接矩阵图"<<endl;
    dr.read("../../../datareader/unit_test/read_grpadj.test", mlist, elist);
    cnt = mlist.size();
    cout<<"  图数量："<<cnt<<endl;

    // 图大小
    s_graph *g = (s_graph *) mlist.front();
    cout<<"  图大小："<<g->m.rows()<<"x"<<g->m.cols()<<endl;
    
    // 图类型
    CPPUNIT_ASSERT_EQUAL(g->grp_type, ADJACENCY_MATRIX);

    // 图数据
    cout<<"  图数据："<<endl<<g->m<<endl;

    // 命令
    cout<<"  命令："<<elist.front()<<endl;
    return;
}

void default_datareader_test::read_grpinc_test()
{
    matrix_list mlist;
    expr_list elist;
    default_datareader dr;
    int cnt = 0, row = 0, col = 0;

    // 图的数量
    cout<<endl<<"测试读取关联矩阵图"<<endl;
    dr.read("../../../datareader/unit_test/read_grpinc.test", mlist, elist);
    cnt = mlist.size();
    cout<<"  图数量："<<cnt<<endl;

    // 图大小
    s_graph *g = (s_graph *) mlist.front();
    cout<<"  图大小："<<g->m.rows()<<"x"<<g->m.cols()<<endl;
    
    // 图类型
    CPPUNIT_ASSERT_EQUAL(g->grp_type, BASIC_INCIDENCE_MATRIX);

    // 图数据
    cout<<"  图数据："<<endl<<g->m<<endl;

    // 命令
    cout<<"  命令："<<elist.front()<<endl;
    return;
}

