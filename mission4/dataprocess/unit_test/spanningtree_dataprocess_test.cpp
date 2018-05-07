#include "datareader_factory.h"
#include "spanningtree_dataprocess_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(spanningtree_dataprocess_test);

void spanningtree_dataprocess_test::setUp()
{
    return;
}

void spanningtree_dataprocess_test::tearDown()
{
    return;
}

void spanningtree_dataprocess_test::process_test()
{
    string cmd;
    matrix_list mlist;
    expr_list elist;
    matrix_list olist;
    s_graph *g = NULL;
    datareader_interface *dr = NULL;
    int cnt = 0, res = 0;

    // 读取邻接矩阵
    dr = datareader_factory::get_datareader();
    dr->read("../../../dataprocess/unit_test/spanningtree.test", mlist, elist);

    // 检查命令参数 
    cmd = elist.front();
    if (strcasecmp(cmd.c_str(), "spanningtree") != 0) {
        CPPUNIT_FAIL("To test display the command must be 'spanningtree'");
        return;
    }
    
    // 处理参数
    spanningtree_dataprocess sp;
    cnt = mlist.size();
    sp.process(mlist, olist);

    res = mlist.size();
    CPPUNIT_ASSERT_EQUAL(0, res);

    res = olist.size();
    CPPUNIT_ASSERT_EQUAL(9, res);
    

    cout<<"  spanningtree 测试通过"<<endl;
    return;
}

