#ifndef TABLE_H
#define TABLE_H

#include "ks1.h"
#include "ks2.h"

typedef struct Table {
	KeySpace1* ks1;
	KeySpace2* ks2;
	int msize1;
	int msize2;
	int csize1;
}Table;


int initTable_random(Table* tab);
int initTable_zeroes(Table* tab);
//int initTable(Table* tab);
int delTable(Table* tab);
int insert(Table* tab, const char* key1, int key2, Item* info);
int remove(Table* tab, const char* key1, int* key2);
int find_both_keys(Table* tab, const char* key1, int key2, Table** foundtab);
int remove_both_keys(Table* tab, const char* key1, int key2);
int find_both_keys_whithout_res(Table* tab, char* key1, int key2);
int find_only_key1(Table* tab, char* key1, Table* foundtab);
int find_only_key2(Table* tab, int key2, Table* foundtab);
int remove_only_key1(Table* tab, const char* key1);
int remove_only_key2(Table* tab, const char* key2);
int find_only_key2_release(Table* tab, int key2, int release, Table* foundtab);
int remove_key1_range(Table* tab, char* key1_1, char* key1_2);

#endif
