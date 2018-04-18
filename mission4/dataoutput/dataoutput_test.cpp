#include <iostream>
#include "datareader_factory.h"
#include "dataoutput1.h"

using namespace std;

int main(int argc, char **argv)
{
    matrix_list mlist;
    expr_list elist;
    matrix_list olist;
    s_graph *g = NULL;
    datareader_interface *dr = NULL;
    
    if (argc < 1) {
        exit(1);
    }

    // 读取邻接矩阵
    dr = datareader_factory::get_datareader();
    dr->read(argv[1], mlist, elist);

    // 输出矩阵
    dataoutput1 dop;
    dop.output_graph(mlist);
    return 0;
}
