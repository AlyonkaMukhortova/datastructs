#ifndef DIALOG_H
#define DIALOG_H
#include "table.h"


//int menu();
int dialog(const char* msgs[], int N);
int get_int();
char* getstr();

int D_Add(Table*), D_Find(Table*), D_Delete(Table*), D_Show(Table*);
int D_Load(Table* ptab);

int (*fptr[])(Table*);
const char* msgs[];
const int NMsgs;



#endif
