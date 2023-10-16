#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "ks1.h"

int ks1_insert(KeySpace1* ks, char* key, Item* newItem, int csize) {
	int n = ks1_find(ks, key, csize);
	if (n == SUCCESS)
		return EXISTS;
	else {
		int i = MSIZE1 - 1;

		while (strcmp(ks[i].key1, "\0") == 0) {
			i--;
		}
		if (i == MSIZE1 - 1)
			return KS_IS_FULL;

		i++;
		while (i > 0 && strcmp(key, ks[i - 1].key1) < 0) {
			strcpy(ks[i].key1, ks[i - 1].key1);
			ks[i].item = ks[i - 1].item;
			i--;
		}
			strcpy(ks[i].key1, key);
			ks[i].item = newItem;
			if (newItem!=NULL)
				newItem->key1 = key;
			return SUCCESS;
	}
}

int ks1_find(KeySpace1* ks, char* key, int csize) {
	int i = 0, j = -1, u = 0;
	int m = csize - 1;
	while (i <= m) {
		j = (i + m) / 2;
		if (strcmp(key, ks[j].key1) == 0) {
			u = 1;
			break;

		}
		else if (strcmp(key, ks[j].key1) > 0) {
			i = j + 1;
		}
		else if (strcmp(key, ks[j].key1) < 0) {
			m = j - 1;
		}
	}
	if (i > m)
		j = -1;
	if (j == -1)
		return NOT_FOUND;
	else
		return SUCCESS;
	//return j;
}

int ks1_find_index(KeySpace1* ks, char* key, int csize) {
	int i = 0, j = -1, u = 0;
	int m = csize - 1;
	while (i <= m) {
		j = (i + m) / 2;
		if (strcmp(key, ks[j].key1) == 0) {
			u = 1;
			break;

		}
		else if (strcmp(key, ks[j].key1) > 0) {
			i = j + 1;
		}
		else if (strcmp(key, ks[j].key1) < 0) {
			m = j - 1;
		}
	}
	if (i > m)
		return NOT_FOUND;
	else
		return j;
}

int ks1_remove(KeySpace1* ks, char* key, int csize) {
	int ind_el = ks1_find_index(ks, key, csize);
	if (ks1_find(ks, key, csize) == -1) {
		return NOT_FOUND;
	}
	else {
		for (int i = ind_el; i < MSIZE1 - 1; i++) {
			strcpy(ks[i].key1, ks[i + 1].key1);
			//ks[i].key1 = ks[i+1].key1;
			ks[i].item = ks[i + 1].item;
		}
		strcpy(ks[MSIZE1 - 1].key1, "\0");
		ks[MSIZE1 - 1].item = NULL;
		return SUCCESS;
	}
}

int ks1_remove_all(KeySpace1* ks, char* key1, char* key2, int csize, int* index2) {
	int index_first = 0, index_last = 0, h = 0, index = 0, * k;
	k = index2;
	if (strcmp(key1, key2) > 0) {
		char* k = (char*)malloc(10 * sizeof(char));
		k = strcpy(k, key1);
		strcpy(key1, key2);
		strcpy(key2, k);
		free(k);
	}
	while (index < MSIZE1) {
		if (ks[index].item != NULL && strcmp(ks[index].key1, key1) >= 0 && strcmp(ks[index].key1, key2) <= 0) {
			*k = ks[index].item->index2;
			k++;
			ks1_remove(ks, ks[index].key1, csize);
			h++;

		}
		else
			index++;
	}
	if (h > 0)
		return SUCCESS;
	else
		return NOT_FOUND;
}