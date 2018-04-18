#include <iostream>
#include <list>
#include "datareader1.h"

using namespace std;

int main(int argc, char **argv) 
{
    matrix_list mlist;
    expr_list elist;
    datareader1 dr;

    if (argc < 1)
        exit(1);

    dr.read(argv[1], mlist, elist);
    cout<<"图数量："<<mlist.size()<<endl;
    s_graph *g = (s_graph *) mlist.front();
    cout<<"第一个图大小："<<g->m.size()<<endl;
    if (g->grp_type == ADJACENCY_MATRIX) {
        cout<<"Type:ADJACENCY_MATRIX"<<endl;
    }
    else if (g->grp_type == BASIC_INCIDENCE_MATRIX) {
        cout<<"Type:BASIC_INCIDENCE_MATRIX"<<endl;
    }
    else {
        cout<<"Type:Unknow"<<endl;
    }
    cout<<g->m<<endl;
    string cmd = elist.front();
    cout<<"command: "<<cmd<<endl;
    return 0;
}
