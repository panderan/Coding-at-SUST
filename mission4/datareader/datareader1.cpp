#include "datareader1.h"
#include <iostream>

using std::cout;

void datareader1::read(string filepath, matrix_list &mlist, expr_list &elist)
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
            if (strcasecmp(line, "grpadj") == 0) {
                graph->grp_type = ADJACENCY_MATRIX;
            }
            else {
                graph->grp_type = BASIC_INCIDENCE_MATRIX;
            }
            read_graph(fp, graph);
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


void datareader1::read_graph(FILE *fp, s_graph *graph) 
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

