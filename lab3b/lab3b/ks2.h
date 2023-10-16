#ifndef KS2_H
#define KS2_H
#include "item.h"

#define DELETED -1
#define NO 0
#define BUSY 1


typedef struct KeySpace2 {
	int busy;
	int key2;
	int release;
	Item* item;
}KeySpace2;

int ks2_insert(KeySpace2* ks, int key, Item* newItem);
int ks2_remove(KeySpace2* ks, int key);
//int ks2_find(KeySpace2* ks, int key);
int ks2_find_all(KeySpace2* ks, int key, int* array_of_result_index);
int ks2_find_release(KeySpace2* ks, int key, int release, int* index);
int ks2_remove_release(KeySpace2* ks, int key, int release);
int ks2_remove_index(KeySpace2* ks, int index);
int hash(int key);
int hash1(int key);

#endif