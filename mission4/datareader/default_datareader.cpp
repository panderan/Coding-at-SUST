#include "default_datareader.h"
#include "datareader_factory.h"
#include <iostream>

REGISTER_TO_A_FACTORY(datareader_factory, default_datareader, "default", datareader_interface)
REFLECT_IMPLEMENT(datareader_interface, default_datareader)

using std::cout;

void default_datareader::read(string filepath, matrix_list &mlist, expr_list &elist)
{
    char line[1024] = {0};
    char tmp[512] = {0};
    char *chp = NULL, *chp2 = NULL;
    FILE *fp = NULL;

    fp = fopen(filepath.c_str(), "r");
    if (fp == NULL) {
        fprintf(stderr, "Failed to open file %s\n", filepath);
        exit(0);
    }

    for (fgets(line, 1024, fp); feof(fp) == 0; fgets(line, 1024, fp)) {
        if (line[0] == '#') {
            continue;
        }
        if (strlen(line) < 7) {
            continue;
        }
        strncpy(tmp, line, 6);
        line[6] = 0;
        if (strcasecmp(line, "grpadj") == 0 || strcasecmp(line, "grpinc") == 0) {
            s_graph *graph = (s_graph *)calloc(1, sizeof(s_graph));
            for (chp = line + 7; *chp == ' ' ; chp++);
            for (chp2 = chp; *chp2 != ' ' && *chp2 != '{' && *chp2 != 0; chp2++);
            *chp2 = 0;
            strcpy(graph->name, chp);
            read_graph(fp, graph);
            if (strcasecmp(line, "grpadj") == 0) {
                graph->grp_type = ADJACENCY_MATRIX;
                check_grpadj(graph);
            }
            else {
                graph->grp_type = BASIC_INCIDENCE_MATRIX;
                check_grpinc(graph);
            }
            mlist.push_back(graph);
        }
        if (strcasecmp(line, "commnd") == 0) {
            string cmd(line+7);
            if (!cmd.empty()) {
                int index = 0;
                while ((index = cmd.find(' ', index)) != string::npos) {
                    cmd.erase(index, 1);
                }
                index = 0;
                while ((index = cmd.find('\n', index)) != string::npos) {
                    cmd.erase(index, 1);
                }
            }
            elist.push_back(cmd);
        }
    }
    return ;
}


void default_datareader::read_graph(FILE *fp, s_graph *graph) 
{
    int ch = 0, row = 0, col = 0, col_prev = 0;
    int cutrowsize = 10, cutcolsize=10;
    int *num = NULL;

    graph->m.resize(cutrowsize, cutcolsize);
    for (ch = fgetc(fp); feof(fp) == 0; ch = fgetc(fp)) {
        if (ch == '}') {
            break;
        }
        if (ch == '\n') {
            row++;
            if (col_prev != 0 && col_prev != col) {
                fprintf(stderr, "the number of value in each row is not equal.\n");
                exit(0);
            }
            if (col_prev == 0) {
                col_prev = col;
            }
            else {
                if (col_prev != col) {
                    fprintf(stderr, "invalid matrix (%d != %d) \n", col_prev, col);
                    exit(0);
                }
            }
            if (row == cutrowsize) {
                cutrowsize += 10;
                graph->m.conservativeResize(cutrowsize, cutcolsize);
            }
            col = 0;
            continue;
        }
        if (ch == ' ') {
            continue;
        }
        if (ch != '0' && ch != '1') {
            fprintf(stderr, "Incorrect input format\n");
            exit(0);
        }
        graph->m(row, col) = ch - '0' ;
        col++;
        if (col == cutcolsize) {
            cutcolsize += 10;
            graph->m.conservativeResize(cutrowsize, cutcolsize);
        }
    }
    graph->m.conservativeResize(row, col_prev);
    return ;
}

void default_datareader::check_grpadj(const s_graph * const graph)
{
    int row= 0, col = 0;
    int i = 0, j = 0;
    
    row = graph->m.rows();
    col = graph->m.cols();

    if (row != col) {
        fprintf(stderr, "邻接矩阵必须是对称矩阵,行与列不相等\n");
        exit(1);
    }
    
    for (i=0; i<row; i++) {
        for (j=0; j<col; j++) {
            if (j<=i) {
                continue;
            }
            if (graph->m(i,j) != graph->m(j,i)) {
                fprintf(stderr, "邻接矩阵必须是对称矩阵(%d,%d) != (%d,%d)\n",
                        i,j,j,i);
                exit(1);
            }
        }
    }
}

void default_datareader::check_grpinc(const s_graph * const graph)
{
    int row= 0, col = 0;
    int i = 0, j = 0;
    int cnt = 0;

    row = graph->m.rows();
    col = graph->m.cols();

    for (j=0; j<col; j++) {
        cnt = 0;
        for (i=0; i<row; i++) {
            cnt += graph->m(i,j);
        }
        if (cnt>2) {
            fprintf(stderr, "关联矩阵的每一列的元素和应当小于等于2,列：%d\n",
                    j);
            exit(1);
        }
    }
}






















