#include <iostream>
#include "datareader_factory.h"
#include "dataprocess1.h"

using namespace std;

int main(int argc, char **argv)
{
    matrix_list mlist;
    expr_list elist;
    matrix_list olist;
    s_graph *g = NULL;
    datareader_interface *dr = NULL;

    // 读取邻接矩阵
    dr = datareader_factory::get_datareader();
    dr->read("./data.txt", mlist, elist);

    // 处理矩阵
    dataprocess1 dp;
    dp.process(mlist, elist, olist);
    cout<<"in:"<<mlist.size()<<endl;
    cout<<"out:"<<olist.size()<<endl;
    if (!olist.empty()) {
        g = olist.front();
        cout<<g->name<<endl;
        cout<<g->m<<endl;
    }
}
