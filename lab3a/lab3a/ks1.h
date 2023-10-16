#ifndef KS1_H
#define KS1_H
#include "item.h"

#define NK 10

typedef struct KeySpace1 {
	char key1[NK];
	Item* item;
}KeySpace1;

int ks1_insert(KeySpace1* ks, char* key, Item* newItem, int csize);
int ks1_remove(KeySpace1* ks, char* key, int csize);
int ks1_find(KeySpace1* ks, char* key, int csize);
int ks1_find_index(KeySpace1* ks, char* key, int csize);
//int ks1_remove_key_range(KeySpace1* ks, char* key_from, char* key_to);
int ks1_remove_all(KeySpace1* ks, char* key1, char* key2, int csize, int* keys);

#endif
