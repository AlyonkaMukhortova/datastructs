#ifndef DIALOG_H
#define DIALOG_H
#include "graph.h"


//int menu();
int dialog(const char* msgs[], int N);
int get_int();
char* getstr();

int D_Add(Graph**), D_Find(Graph**), D_Delete(Graph**), D_Show(Graph**), D_Timing(Graph**), D_Save(Graph**), D_Generate(Graph**), D_Map(Graph**);

int (*fptr[])(Graph**);
const char* msgs[];
const int NMsgs;
char* rand_str();
int delGraph(Graph** graph);
Graph* Generate_Graph(int n);
void load_map(Graph** graph, char* fname_nodes, char* fname_edges);



#endif



