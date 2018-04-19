#include <iostream>
#include "graph_struct.h"
#include "datareader_factory.h"
#include "dataprocess_factory.h"
#include "dataoutput_factory.h"

void print_help();

int main(int argc, char *argv[])
{
    matrix_list mlist;
    expr_list elist;
    matrix_list olist;
    datareader_interface *dr = NULL;
    dataprocess_interface *dp = NULL;
    dataoutput_interface *dop = NULL;

    if (argc < 1) {
        print_help();
        exit(1);
    }

    // 读取邻接矩阵
    dr = datareader_factory::get_datareader();
    dr->read(argv[1], mlist, elist);

    // 处理矩阵
    dp = dataprocess_factory::get_dataprocess();
    dp->process(mlist, elist, olist);

    // 图形输出
    dop = dataoutput_factory::get_dataoutput();
    dop->output_graph(olist);
    return 0;
}

void print_help()
{
    cout<<"grptools help:"<<endl;
    cout<<"  grptools <filename>"<<endl;
}
