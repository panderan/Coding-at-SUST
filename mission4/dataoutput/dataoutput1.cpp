#include "dataoutput1.h"
#ifdef unix
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#else
#include <direct.h>
#endif

void dataoutput1::output_graph(matrix_list &glist)
{
    char cwd[1024] = {0};
    char dotfiledir[2048] = {0};
    list<s_graph *>::iterator it;
    s_graph *g = NULL;

    // 创建输出目录
    getcwd(cwd, 1024); 
    sprintf(dotfiledir, "%s/output", cwd);
    output_mkdir(dotfiledir);

    sprintf(dotfiledir, "%s/output/dotfiles", cwd);
    output_mkdir(dotfiledir);

    sprintf(dotfiledir, "%s/output/pictures", cwd);
    output_mkdir(dotfiledir);

    // 输出dot语言描述的Graph
    for (it=glist.begin(); it!=glist.end(); it++) {
        g = *it;
        if (g->grp_type == BASIC_INCIDENCE_MATRIX) {
            output_graph_bsc_inc_mat(cwd, g);
        }
        else {
            output_graph_adj_mat(cwd, g);
        }
    } 
}

void dataoutput1::output_graph_adj_mat(const char * cwd, s_graph *g)
{
    char dotfilename[2048] = {0};
    char pngfilename[2048] = {0};
    char cmd[2048] = {0};
    FILE *fp = NULL;
    int i = 0, j = 0;
    int flag_isolated = 0;

    // 输出dot语言描述的Graph,并生成png图片
    sprintf(dotfilename, "%s/output/dotfiles/%s.dot", cwd, g->name);
    sprintf(pngfilename, "%s/output/pictures/%s.png", cwd, g->name);
    fp = fopen(dotfilename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to create output file\n");
        exit(1);
    }
    fprintf(fp, "graph %s {\n", g->name);
    fprintf(fp, "node [shape=circle]\n");
    fprintf(fp, "edge [len=2.0]\n");
    for (i=0; i<g->m.rows(); i++) {
        flag_isolated = 0;
        for (j=0; j<g->m.cols(); j++) {
            flag_isolated +=g->m(i,j);
            if (i>j) {
                continue;
            }
            if (g->m(i,j) == 1) {
                fprintf(fp, "n%d -- n%d\n", i+1, j+1);
            }
        }
        if (flag_isolated == 0) {
            fprintf(fp, "n%d\n", i+1);
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
    sprintf(cmd, "neato -Tpng %s -o %s", dotfilename, pngfilename);
    system(cmd);
    fp = NULL;
}

void dataoutput1::output_graph_bsc_inc_mat(const char * cwd, s_graph *g)
{
    char dotfilename[2048] = {0};
    char pngfilename[2048] = {0};
    char cmd[2048] = {0};
    FILE *fp = NULL;
    int n1 = 0, n2 = 0;
    int i = 0, j = 0;

    // 输出dot语言描述的Graph,并生成png图片
    sprintf(dotfilename, "%s/output/dotfiles/%s.dot", cwd, g->name);
    sprintf(pngfilename, "%s/output/pictures/%s.png", cwd, g->name);
    fp = fopen(dotfilename, "w");
    if (fp == NULL) {
        fprintf(stderr, "Failed to create output file\n");
        exit(1);
    }
    fprintf(fp, "graph %s {\n", g->name);
    fprintf(fp, "node [shape=circle]\n");
    fprintf(fp, "edge [len=2.0]\n");

    // 绘制每条边
    for (i=0; i<g->m.cols(); i++) {
        n1 = -1;
        n2 = -1;
        for (j=0; j<g->m.rows(); j++) {
            if (g->m(j,i)) {
                if (n1 < 0) {
                    n1 = j;
                }
                else {
                    n2 = j;
                    break;
                }
            }
        }
        if (n2 == -1) {
            n2 = j;
        }
        fprintf(fp, "n%d -- n%d\n", n1+1, n2+1 );
    }

    // 绘制孤立点
    for (i=0; i<g->m.rows(); i++) {
        int total = 0;
        for (j=0; j<g->m.cols(); j++) {
            total += g->m(i,j);
        }
        if (total == 0) {
            fprintf(fp, "n%d\n", i+1);
        }
    }
    fprintf(fp, "}\n");
    fclose(fp);
    sprintf(cmd, "neato -Tpng %s -o %s", dotfilename, pngfilename);
    system(cmd);
    fp = NULL;
}

void dataoutput1::output_mkdir(const char *tgt)
{
    // 创建输出目录
    if (access(tgt, F_OK) != 0) {
#ifdef unix
        if (mkdir(tgt, S_IRWXU|S_IRGRP|S_IXGRP|S_IROTH|S_IXOTH) != 0) {
#else
        if (mkdir(tgt) != 0) {
#endif
            fprintf(stderr, "Unable to create dir %s\n", tgt);
            exit(0);
        }
    }
}





