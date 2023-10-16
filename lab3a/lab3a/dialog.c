#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "dialog.h"

(*fptr[])(Table*) = { 0, D_Add, D_Find, D_Delete, D_Show};

const char* msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show" };

const int NMsgs = sizeof(msgs) / sizeof(msgs[0]);

int get_int(int* a) {
	int n;
	do {
		n = scanf("%d", a);
		if (n == 0) {
			printf("Error\n");
			scanf("%*c", 0);
		}
	} while (n == 0);
	return n < 0 ? 0 : 1;
}

int get_float(float* a) {
	int n;
	do {
		n = scanf("%f", a);
		if (n == 0) {
			printf("Error\n");
			scanf("%*c", 0);
		}
	} while (n == 0);
	return n < 0 ? 0 : 1;
}

char* getstr()
{
	char* ptr = (char*)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = scanf("%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
			//break;
		}
		if (n == 0) {
			scanf_s("%*c");
			//break;
		}
		else {
			len += strlen(buf);
			ptr = (char*)realloc(ptr, len + 1);
			int k = strlen(buf);
			int l = strlen(ptr) + k + 1;
			strncat_s(ptr, l, buf, k);
		}
	} while (n > 0);
	return ptr;
}

int dialog(const char* msgs[], int N)
{
	char* errmsg = "";
	int rc;
	int i, n;
	do {
		puts(errmsg);
		errmsg = "You are wrong. Repeate, please!";
		for (i = 0; i < N; ++i)
			puts(msgs[i]);
		puts("Make your choice: --> ");
		n = get_int(&rc); 
		if (n == 0) 
			rc = 0;
	} while (rc < 0 || rc >= N);
	return rc;
}

int D_Add(Table* tab) {
	int key2;
	char* key1,* strinfo = "\0",*s;
	//char* key1 = key;
	key1 = (char*)malloc(11);
	float num1, num2;
	//InfoType* info;
	printf("Enter key 1\n");
	scanf("%*c");
	s = getstr();
	//scanf("%s",&key1);
	printf("Enter key 2\n");
	get_int(&key2);
	printf("Enter float num 1 for item\n");
	get_float(&num1);
	//scanf("%f",&num1); 
	printf("Enter float num 2 for item\n");
	get_float(&num2);
	//scanf("%f",&num2);
	printf("Enter string for item\n");
	scanf("%*c");
	strinfo = getstr();
	int p = strlen(s);
	if (p > 10)
		p = 10;
	strncpy(key1, s, p);
	key1[p] = '\0';
	//scanf("%s",&strinfo);
	Item* inf = create_item(num1, num2, strinfo);
	int res = insert(tab, key1, key2, inf);
	if (res == SUCCESS)
		printf("Item has been added successfully\n");
	else if (res == KS_IS_FULL)
		printf("Table is full, can't add Item");
	else
		printf(" Key1 exists in Table");
}

int D_Find(Table* tab) {
	printf("If you want to find element using both keys enter 1\nIf you want to find element only with key1 enter 2\nIf you want to fint all elements using only key2 enter 3\n");
	printf("If you want to find version of element using key2 enter 4\n");
	int a, b;
	int u = NOT_FOUND;
	b = get_int(&a);
	Table result = { NULL,NULL,1,1 };
	int f = initTable_zeroes(&result);
	Table* res = NULL;
	if (a == 1) {
		printf("Enter char key no more than 10 symbols for key 1\n");
		char* key1 = "";
		scanf("%*c");
		key1 = getstr();
		printf("Enter int key\n");
		int key2 = 0;
		get_int(&key2);
		//Table result = { NULL,NULL,1,1 };
		//int f = initTable_zeroes(&result);
		u = find_both_keys(tab, key1, key2, &result);
		res = &result;
	}
	else if (a == 2) {
		printf("Enter char key no more than 10 symbols for key 1\n");
		char* key1 = "";
		scanf("%*c");
		key1 = getstr();
		//Table result = { NULL,NULL,1,1 };
		//int f = initTable_zeroes(&result);
		u = find_only_key1(tab, key1, &result);
		res = &result;
	}
	else if (a == 3) {
		printf("Enter int key 2\n");
		int key2 = 0;
		get_int(&key2);
		//Table result = { NULL,NULL,1,1 };
		//int f = initTable_zeroes(&result);
		u = find_only_key2(tab, key2, &result);
		res = &result;

	}
	else if (a == 4) {
		printf("Enter int key 2\n");
		int key2 = 0, release = 0;;
		get_int(&key2);
		printf("Enter release\n");
		get_int(&release);
		//Table result = { NULL,NULL,1,1 };
		//int f = initTable_zeroes(&result);
		u = find_only_key2_release(tab, key2, release, &result);
		res = &result;
	}
	else
		printf("You are wrong. Repeate, please!");
	if (u == SUCCESS) {
		D_Show(res);
		
	}
	else
		printf("No such element in table");
	delTable(&result);
}

int D_Show(Table* tab) {
	printf("KeySpace1:\n{key1, key2(release), num1, num2, string}\n");
	for (int i = 0; i < tab->msize1; i++) {
		if (tab->ks1[i].item == NULL)
			printf("{0, 0, ");
		else
			printf("{%s, %d(%d), ", tab->ks1[i].key1, tab->ks2[tab->ks1[i].item->index2].key2, tab->ks2[tab->ks1[i].item->index2].release);
		print_item(tab->ks1[i].item);
	}
	printf("\n\n");
	printf("KeySpace2:\n{key1, key2(release), num1, num2, string}\n");

	for (int i = 0; i < tab->msize2; i++) {
		if (tab->ks2[i].item == NULL)
			printf("{0, 0, ");
		else
			printf("{%s, %d(%d), ", tab->ks2[i].item->key1, tab->ks2[i].key2, tab->ks2[i].release);
		print_item(tab->ks2[i].item);
	}

	printf( "\n" );
}

int D_Delete(Table* tab) {
	printf("Enter 1 if you want to delete element using both keys\nEnter 2 if you want to delete element using key1\nEnter 3 if you want to delete all elements using key2\n");
	printf("Enter 4 if you want to delete elements in range of keys(only for keyspace1)\n");
	int a,j,q = 1;
	j = get_int(&a);
	if (a == 1) {
		int key2;
		char* key1;
		printf("Enter key1\n");
		scanf("%*c");
		key1 = getstr();
		printf("Enter key2\n");
		j = get_int(&key2);
		j = remove_both_keys(tab, key1, key2);
		
	}
	else if (a == 2) {
		int q = 0;
		char* key1;
		printf("Enter key1\n");
		scanf("%*c");
		key1 = getstr();
		j = remove_only_key1(tab, key1);
	}
	else if (a == 3) {
		printf("Enter key2\n");
		int key2;
		j = get_int(&key2);
		j = remove_only_key2(tab, key2);
	}
	else if (a == 4) {
		char* key1_1,* key1_2;
		printf("Enter first key1\n");
		scanf("%*c");
		key1_1 = getstr();
		printf("Enter second key1\n");
		//scanf("%*c");
		key1_2 = getstr();
		j = remove_key1_range(tab, key1_1, key1_2);
	}
	else {
		printf("You are wrong. Repeate, please!");
		q = 0;
	}
	if (j == SUCCESS && q == 1)
		printf("Elemet has been deleted successfully\n");
	else if (j == NOT_FOUND && q == 1)
		printf("No such element in table");
}