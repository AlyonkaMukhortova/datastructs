#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "table.h"

int initTable_zeroes(Table* tab) {
	int i = 0;
	tab->ks1 = (KeySpace1*)malloc(tab->msize1 * sizeof(KeySpace1));
	tab->ks2 = (KeySpace2*)malloc(tab->msize2 * sizeof(KeySpace2));
	tab->csize1 = 0;
	while (i < tab->msize2) {
		tab->ks2[i].key2 = 0;
		tab->ks2[i].busy = NO;
		tab->ks2[i].release = 0;
		tab->ks2[i].item = NULL;
		i++;
	}
	i = 0;
	while (i < tab->msize1) {
		char* u = "\0";
		strcpy(tab->ks1[i].key1, u);
		tab->ks1[i].item = NULL;
		i++;
	}
}



int insert(Table* tab, const char* key1, int key2, Item* item, InfoType* info) {
	int k = ks1_insert(tab->ks1, key1, item, tab->csize1);
	int y = EXISTS;
	if (k == SUCCESS)
		y = ks2_insert(tab->ks2, key2, item);
	if (k == SUCCESS && y == SUCCESS) {
		if (strcmp(tab->ks1[1].key1, "\0") == 0)
			fseek(tab->fd, 92 * 4, SEEK_SET);
		else
			fseek(tab->fd, 0, SEEK_END);
		int i = ks1_find_index(tab->ks1, key1, tab->csize1 + 1);
		tab->ks1[i].item->offset = ftell(tab->fd);
		fwrite(&(info->num1), sizeof(float), 1, tab->fd);
		fwrite(&(info->num2), sizeof(float), 1, tab->fd);
		//printf("%s", info->str);
		//printf("%d", tab->ks1[i].item->len - 8);
		fwrite(info->str, sizeof(char), tab->ks1[i].item->len - 8, tab->fd);
		tab->csize1 += 1;
		return SUCCESS;
	}
	if (k == KS_IS_FULL || y == KS_IS_FULL)
		return KS_IS_FULL;
	if (k == EXISTS)
		return EXISTS;
}

int insert_whithout_info(Table* tab, const char* key1, int key2, Item* item) {
	int k = ks1_insert(tab->ks1, key1, item, tab->csize1);
	int y = EXISTS;
	if (k == SUCCESS)
		y = ks2_insert(tab->ks2, key2, item);
	if (k == SUCCESS && y == SUCCESS) {
		return SUCCESS;
	}
	if (k == KS_IS_FULL || y == KS_IS_FULL)
		return KS_IS_FULL;
	if (k == EXISTS)
		return EXISTS;
}

int find_both_keys(Table* tab, char* key1, int key2, Table* foundtab) {
	int* a = (int*)malloc(tab->msize2 * sizeof(int));
	int y = ks2_find_all(tab->ks2, key2, a);
	
	if (y == NOT_FOUND)
		return NOT_FOUND;
	else {
		while (*a >= 0) {
			if (strcmp(tab->ks2[*a].item->key1, key1) == 0) {
				Item* newone = copy_item(tab->ks2[*a].item);
				newone->index2 = 0;
				strcpy(foundtab->ks1->key1, key1);
				foundtab->ks1->item = newone;
				foundtab->ks2->key2 = key2;
				foundtab->ks2->item = newone;
				foundtab->ks2->item->offset = tab->ks2[*a].item->offset;
				foundtab->ks2->release = tab->ks2[*a].release;
				foundtab->fd = tab->fd;
				return SUCCESS;
			}
			a++;
		}
	}
	//free(a);
	return NOT_FOUND;
}

int find_only_key1(Table* tab, char* key1, Table* foundtab) {
	if (ks1_find(tab->ks1, key1, tab->csize1) == NOT_FOUND)
		return NOT_FOUND;
	else {
		int a = ks1_find_index(tab->ks1, key1, tab->csize1);
		Item* newone = copy_item(tab->ks1[a].item);
		newone->index2 = 0;
		strcpy(foundtab->ks1->key1, key1);
		foundtab->ks1->item = newone;
		foundtab->ks2->key2 = tab->ks2[tab->ks1[a].item->index2].key2;
		foundtab->ks2->item = newone;
		foundtab->ks2->item->offset = tab->ks1[a].item->offset;
		foundtab->ks2->release = tab->ks2[tab->ks1[a].item->index2].release;
		foundtab->fd = tab->fd;
		return SUCCESS;
	}
}

int find_only_key2(Table* tab, int key2, Table* foundtab) {
	int* s = (int*)malloc(tab->msize2 * sizeof(int));
	int a = ks2_find_all(tab->ks2, key2, s);
	if (a == NOT_FOUND)
		return NOT_FOUND;
	else {
		int n = 0, * r = s;
		while (*r >= 0) {
			n++;
			r++;
		}
		foundtab->msize2 = n;
		foundtab->msize1 = n;
		foundtab->ks1 = (KeySpace1*)realloc(foundtab->ks1, foundtab->msize1 * sizeof(KeySpace1));
		foundtab->ks2 = (KeySpace1*)realloc(foundtab->ks2, foundtab->msize2 * sizeof(KeySpace1));
		n = 0;
		while (*s >= 0) {
			Item* newone = copy_item(tab->ks2[*s].item);
			newone->index2 = n;
			strcpy(foundtab->ks1->key1, tab->ks2[*s].item->key1);
			foundtab->ks1->item = newone;
			foundtab->ks2->key2 = key2;
			foundtab->ks2->item = newone;
			foundtab->ks2->item->offset = tab->ks2[*s].item->offset;
			foundtab->ks2->release = tab->ks2[*s].release;
			foundtab->fd = tab->fd;
			n++;
			s++;
		}
		return SUCCESS;
	}
	free(s);
}

int find_both_keys_whithout_res(Table* tab, char* key1, int key2) {
	int* a = (int*)malloc(tab->msize2 * sizeof(int));
	int y = ks2_find_all(tab->ks2, key2, a);
	if (y == NOT_FOUND)
		return NOT_FOUND;
	else {
		while (*a > 0) {
			if (strcmp(tab->ks2[*a].item->key1, key1) == 0) {
				return SUCCESS;
			}
			a++;
		}
	}
	free(a);
}

int remove_both_keys(Table* tab, const char* key1, int key2) {
	if (find_both_keys_whithout_res(tab, key1, key2) == SUCCESS) {
		int f = ks1_find_index(tab->ks1, key1, tab->csize1);
		int y = ks2_remove_release(tab->ks2, key2, tab->ks2[tab->ks1[f].item->index2].release);
		int k = ks1_remove(tab->ks1, key1, tab->csize1);
		if (y == SUCCESS && k == SUCCESS) {
			tab->csize1--;
			return SUCCESS;
		}
		else
			return NOT_FOUND;
	}
	else return NOT_FOUND;
}

int remove_only_key1(Table* tab, const char* key1) {
	int i = ks1_find_index(tab->ks1, key1, tab->csize1);
	if (i != NOT_FOUND) {
		int y = ks2_remove_release(tab->ks2, tab->ks2[tab->ks1[i].item->index2].key2, tab->ks2[tab->ks1[i].item->index2].release);
		int k = ks1_remove(tab->ks1, key1, tab->csize1);
		if (y == SUCCESS && k == SUCCESS) {
			tab->csize1--;
			return SUCCESS;
		}
		else
			return NOT_FOUND;
	}
	else
		return NOT_FOUND;
}

int remove_only_key2(Table* tab, int key2) {
	int* a = (int*)malloc(tab->msize2 * sizeof(int));
	int b = ks2_find_all(tab->ks2, key2, a);
	if (b != NOT_FOUND) {
		while (*a >= 0) {
			int i = tab->ks2[*a].item->key1;
			int w = ks1_remove(tab->ks1, i, tab->csize1);
			a++;
		}
		int u = ks2_remove(tab->ks2, key2);
		return SUCCESS;
	}
	else
		return NOT_FOUND;
	free(a);
}

int delTable(Table* tab) {
	int i = 0;
	while (i < tab->msize2-1) {
		tab->ks2[i].busy = DELETED;
		Item* f = tab->ks2[i].item;
		if (f != NULL) {
			tab->ks2[i].item = NULL;
		}
		free(f);
		i++;
	}
	KeySpace1* q = tab->ks1;
	KeySpace2* r = tab->ks2;
	tab->ks1 = NULL;
	tab->ks2 = NULL;
	free(q);
	free(r);
	//return SUCCESS;
}

int find_only_key2_release(Table* tab, int key2, int release, Table* foundtab) {
	int a;
	int p = ks2_find_release(tab->ks2, key2, release, &a);
	if (p == SUCCESS) {
		Item* newone = copy_item(tab->ks2[a].item);
		newone->index2 = 0;
		strcpy(foundtab->ks1->key1, tab->ks2[a].item->key1);
		foundtab->ks1->item = newone;
		foundtab->ks2->key2 = key2;
		foundtab->ks2->item = newone;
		foundtab->ks2->item->offset = tab->ks2[a].item->offset;
		foundtab->ks2->release = tab->ks2[a].release;
		foundtab->fd = tab->fd;//нужно определить дескриптор файла, чтобы инфа могла считаться
		return SUCCESS;
	}
	else
		return NOT_FOUND;
}

int remove_key1_range(Table* tab, char* key1_1, char* key1_2) {
	int* t = (int*)malloc(tab->msize1 * sizeof(int));
	int r = ks1_remove_all(tab->ks1, key1_1, key1_2, tab->csize1, t);
	if (r == SUCCESS) {
		while (*t >= 0) {
			tab->csize1--;
			int f = ks2_remove_index(tab->ks2, *t);
			t++;
		}
		return SUCCESS;
	}
	else
		return NOT_FOUND;
	free(t);
}

int load(Table* tab, char* fname)
{
	fopen_s(&(tab->fd), fname, "r+b");
	if (tab->fd == NULL)
		return 0;
	fread(&tab->msize1, sizeof(int), 1, tab->fd);
	fread(&tab->msize2, sizeof(int), 1, tab->fd);
	fread(&tab->csize1, sizeof(int), 1, tab->fd);
	
	
	for (int i = 0; i < MSIZE1; i++) {
		char* key1 = (char*)malloc(10*sizeof(char));
		int key2;
		
		int y = ftell(tab->fd);
		fread(key1, sizeof(char), 10, tab->fd);
		y = ftell(tab->fd);
		fread(&key2, sizeof(int), 1, tab->fd);
		y = ftell(tab->fd);
		Item* newitem = (Item*)malloc(sizeof(Item));
		fread(&newitem->len, sizeof(int), 1, tab->fd);
		y = ftell(tab->fd);
		fread(&newitem->offset, sizeof(int), 1, tab->fd);
		if (strcmp(key1, "\0") == 0)
			newitem = NULL;
		else
			int n = insert_whithout_info(tab, key1, key2, newitem);
		//free(key1);
	}

	return 1;
}

int save(Table* tab)
{
	fseek(tab->fd, 2* sizeof(int), SEEK_SET);
	//fseek(tab->fd, sizeof(int), SEEK_SET);
	fwrite(&tab->csize1, sizeof(int), 1, tab->fd);
	for (int i = 0; i < tab->msize1; i++) {
		if (tab->ks1[i].item == NULL) {
			char* d = "\0";
			fwrite(d, sizeof(char), 10, tab->fd);
			int key2 = 0;
			fwrite(&key2, sizeof(int), 1, tab->fd);
			int len = 0;
			fwrite(&len, sizeof(int), 1, tab->fd);
			int offset = 0;
			fwrite(&offset, sizeof(int), 1, tab->fd);
		}
		else{
		fwrite(tab->ks1[i].key1, sizeof(char), 10, tab->fd);
		int key2 = tab->ks2[tab->ks1[i].item->index2].key2;
		fwrite(&key2, sizeof(int), 1, tab->fd);
		fwrite(&tab->ks1[i].item->len, sizeof(int), 1, tab->fd);
		fwrite(&tab->ks1[i].item->offset, sizeof(int), 1, tab->fd);
		}
	}
	fclose(tab->fd);
	tab->fd = NULL;
	return 1;
}

int create(Table* tab, char* fname)
{
	tab->csize1 = 0;
	if (fopen_s(&(tab->fd), fname, "w+b") != 0) {
		tab->ks1 = NULL;
		tab->ks2 = NULL;
		return 0;
	}
	fwrite(&tab->msize1, sizeof(int), 1, tab->fd);
	fwrite(&tab->msize2, sizeof(int), 1, tab->fd);
	fwrite(&tab->csize1, sizeof(int), 1, tab->fd);
	for (int i = 0; i < tab->csize1; i++) {
		fwrite("0", sizeof(char), 10, tab->fd);
		fwrite(0, sizeof(int), 1, tab->fd);
		fwrite(0, sizeof(int), 1, tab->fd);
		fwrite(0, sizeof(int), 1, tab->fd);
	}
	return 1;
}

void print_info(Table* tab, int offset, int len) {
	float num1, num2;
	char* string = (char*)malloc((len - 8)* sizeof(char));
	fseek(tab->fd, offset, SEEK_SET);
	fread(&num1, sizeof(float), 1, tab->fd);
	fread(&num2, sizeof(float), 1, tab->fd);
	int q = ftell(tab->fd);
	fread(string, sizeof(char), len - 8, tab->fd);
	string[len - 8] = '\0';
	printf("%f, %f, %s, ", num1, num2, string);
	return 0;
}