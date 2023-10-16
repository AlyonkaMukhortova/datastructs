#ifndef DIALOG_H
#define DIALOG_H
#include "tree.h"


//int menu();
int dialog(const char* msgs[], int N);
int get_int();
char* getstr();

int D_Add(Tree*), D_Find(Tree*), D_Delete(Tree*), D_Show(Tree*), D_Timing(Tree**), D_Count(Tree**);

int (*fptr[])(Tree*);
const char* msgs[];
const int NMsgs;
void read_b(Tree** tree);
int count(Tree** tree);


#endif
