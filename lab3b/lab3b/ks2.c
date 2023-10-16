#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "ks2.h"




int hash(int key) {
	int g = key % MSIZE2;
	return g;
}

int hash1(int key) {
	int g = (hash(key) + 1) % MSIZE2;
	return g;
}

int ks2_insert(KeySpace2* ks, int key, Item* newItem) {
	int h1 = hash(key);
	int release = 0;
	int n = ks2_find_max_release(ks, key);
	if (n != NOT_FOUND)
		release = n + 1;
	if (ks[h1].busy == NO || ks[h1].busy == DELETED) {
		ks[h1].key2 = key;
		ks[h1].item = newItem;
		ks[h1].busy = BUSY;
		ks[h1].release = release;
		newItem->index2 = h1;
		return SUCCESS;
	}
	else {
		int y = 0;
		while (ks[h1].busy != NO && ks[h1].busy != DELETED) {
			
				
			h1 = hash1(h1);
			y++;
			if (y == MSIZE2) {
				return KS_IS_FULL;
				break;
			}
			if (ks[h1].busy == NO || ks[h1].busy == DELETED) {
				ks[h1].key2 = key;
				ks[h1].item = newItem;
				ks[h1].busy = BUSY;
				ks[h1].release = release;
				if (newItem!=NULL)
					newItem->index2 = h1;
				return SUCCESS;
			}
		}
	}
}

int ks2_find_all(KeySpace2* ks, int key, int* array_of_result_index) {
	int* r = array_of_result_index;
	int h1 = hash(key);
	int n = 0, k = 0;
	while (n < MSIZE2 && ks[h1].busy != NO) {
		if (ks[h1].busy != NO && ks[h1].busy != DELETED && ks[h1].key2 == key) {
			*r = h1;
			r++;
			k++;
		}
		h1 = hash1(h1);
		n++;
	}
	if (/*n == MSIZE2 &&*/ k == 0)
		return NOT_FOUND;
	else if (k > 0);
	return SUCCESS;
}

int ks2_find_max_release(KeySpace2* ks, int key) {
	int h1 = hash(key);
	int n = 0, k = 0, release = 0;
	while (n < MSIZE2 && ks[h1].busy != NO) {
		if (ks[h1].busy != NO && ks[h1].key2 == key) {
			if (ks[h1].release > release) {
				release = ks[h1].release;
			}
			k++;
		}
		h1 = hash1(h1);
		n++;
	}
	if (/*n == MSIZE2 &&*/ k == 0)
		return NOT_FOUND;
	else if (k > 0);
	return release;
}

int ks2_find_release(KeySpace2* ks, int key, int release, int* index) {
	int h1 = hash(key);
	int n = 0, k = 0;
	while (n < MSIZE2 && ks[h1].key2 != NULL && ks[h1].key2 != DELETED) {
		if (ks[h1].busy == BUSY && ks[h1].key2 == key && ks[h1].release == release) {
			*index = ks[h1].item->index2;
			k++;
		}
		h1 = hash1(h1);
		n++;
	}
	if (n == MSIZE2 || k == 0)
		return NOT_FOUND;
	else
		return SUCCESS;
}

int ks2_remove(KeySpace2* ks, int key) {
	int* a, n = 0, * k;
	a = (int*)malloc(MSIZE2 * sizeof(int));
	/*while (n < msize) {
		if (ks2_find(ks, key, &a) != NOT_FOUND) {
			ks[ks2_find(ks, key, &a)].key2 = DELETED;
		}
		n++;
	}*/
	int y = ks2_find_all(ks, key, a);
	k = a;
	while (*k >= 0) {
		n++;
		k++;
	}
	if (y != NOT_FOUND) {
		while (n > 0) {
			ks[*a].busy = DELETED;
			Item* f = ks[*a].item;
			ks[*a].item = NULL;
			free(f);
			a++;
			n--;
		}
		return SUCCESS;
	}
	else {
		return NOT_FOUND;
	}
	free(a);
}

int ks2_remove_release(KeySpace2* ks, int key, int release) {
	int* a, n = 0, * k;
	a = (int*)malloc(MSIZE2 * sizeof(int));
	/*while (n < msize) {
		if (ks2_find(ks, key, &a) != NOT_FOUND) {
			ks[ks2_find(ks, key, &a)].key2 = DELETED;
		}
		n++;
	}*/
	int y = ks2_find_all(ks, key, a);
	k = a;
	while (*k >= 0) {
		n++;
		k++;
	}
	if (y != NOT_FOUND) {
		while (n > 0) {
			if (ks[*a].release == release) {
				ks[*a].busy = DELETED;
				Item* f = ks[*a].item;
				ks[*a].item = NULL;
				free(f);
				break;
			}
			a++;
			n--;
		}
		return SUCCESS;
	}
	else {
		return NOT_FOUND;
	}
	free(a);
}

int ks2_remove_index(KeySpace2* ks, int index) {
	ks[index].busy = DELETED;
	Item* f = ks[index].item;
	ks[index].item = NULL;
	free(f);
	return SUCCESS;
}