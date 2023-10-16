#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dialog.h"

(*fptr[])(Tree**) = { 0, D_Add, D_Find, D_Delete, D_Show, D_Timing, D_Count };

const char* msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Timing", "6. Count" };

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
			scanf("%*c");
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

int D_Add(Tree** tree) {
	int key;
	char* strinfo1 = "\0", * strinfo2 = "\0", * s;
	//char* key1 = key;
	printf("Enter key \n");
	get_int(&key);
	printf("Enter string 1 for item\n");
	scanf("%*c");
	strinfo1 = getstr();
	printf("Enter string 2 for item\n");
	//scanf("%*c");
	strinfo2 = getstr();
	InfoType* info = create_info(strinfo1, strinfo2);
	int res = tree_insert(tree, key, info);
	if (res == SUCCESS)
		printf("Item has been added successfully\n");
}

int D_Find(Tree** tree) {
	Tree* result = NULL;
	int n = 0, p;
	printf("To find min enter 1.\nTo find using key enter 2.\n");
	get_int(&p);
	if (p == 1) {
		result = find_tree_min_find(tree, &n);
		if (result != NULL)
			show_tree_only_one(&result);
		else
			printf("No such element in table");
		if (n > 1) {
			printf("There are %d min elements.\nPlease, enter which one you want to find.\n", n);
			get_int(&n);
			//result = version(result, n);
			show_tree_version(&result, n);
		}
	}
	else if (p == 2) {
		printf("Enter int key\n");
		int key = 0;
		get_int(&key);
		result = tree_find_for_remove(tree, key, &n);
		if (result != NULL)
			show_tree_only_one(&result);
		else
			printf("No such element in table");
		if (n > 1) {
			printf("There are %d elements whith key %d.\nPlease, enter which one you want to find.\n", n, key);
			get_int(&n);
			//result = version(result, n);
			show_tree_version(&result, n);
		}
	}
}

int D_Show(Tree** tree) {
	//show_tree(tree);
	//show_tree_nexts(tree);
	show_tree_tree(tree, 0);
	printf("Enter file name -->  ");
	//scanf("%*c");
	char* fname = "";
	while(strcmp(fname, "") == 0)
		fname = getstr();
	FILE* fd;
	fopen_s(&fd, fname, "w");
	fputs("digraph G{ \n", fd);
	fputs("\tgraph [ordering=\"out\"];\n", fd);
	int n = 0;
	show_tree_graphviz(*tree, fd, &n);
	fputs("}", fd);
	fclose(fd);
	printf("\n");
	//show_tree_reverse(tree);
	return 1;
}

int D_Count(Tree** nnn) {
	Tree* tree = NULL;
	read_b(&tree);
	printf("Enter file name -->");
	char* fname = getstr();
	FILE* fd = NULL; 
	fopen_s(&fd, fname, "w");
	count(&tree, fd);
	fclose(fd);
	return 1;
}

int count(Tree** tree, FILE* fd) {
	if (tree == NULL)
		return 0;
	if (*tree == NULL)
		return 0;
	Tree* ptr = *tree;
	count(&(ptr->right), fd);
	int n = 0;
	tree_find_for_remove(&ptr, ptr->key, &n);
	fprintf(fd, "%d --- %d\n", ptr->key, n);
	count(&(ptr->left), fd);
	return 1;
}

int D_Delete(Tree** tree) {
		printf("Enter key\n");
		int key;
		int j = get_int(&key);
		j = tree_find_without_res(tree, key);
		if (j < 2)
			j = remove_tree(tree, key);
		else {
			printf("There are %d elements with key %d.\nPlease, enter which one you want to remove.\n", j, key);
			get_int(&j);
			j = remove_tree_version(tree, key, j);
		}
		//j = remove_tree(tree, key);
		if (j == SUCCESS)
			printf("Elemet has been deleted successfully\n");
		else if (j == NOT_FOUND)
			printf("No such element in table"); 
		return 1;
}

int D_Load(Tree** tree)
{
	char* fname = NULL;
	printf("Enter file name: --> ");
	fname = getstr();
	if (fname == NULL)
		return 0;
	load(tree, fname);
	free(fname);
	return 1;
}

char* fgetstr(FILE* fd)
{
	char* ptr = (char*)malloc(1);
	char buf[81];
	int n, len = 0;
	*ptr = '\0';
	do {
		n = fscanf(fd, "%80[^\n]", buf);
		if (n < 0) {
			free(ptr);
			ptr = NULL;
			continue;
			//break;
		}
		if (n == 0) {
			scanf("%*c");
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

int fget_int(int* a, FILE* fd) {
	int n;
	do {
		n = fscanf(fd, "%d", a);
		if (n == 0) {
			printf("Error\n");
			fscanf(fd, "%*c", 0);
		}
	} while (n == 0);
	return n;
}

int load(Tree** tree, char* fname)
{
	FILE* fd;
	int t = 0;
	fopen_s(&fd, fname, "r");
	if (fd == NULL)
		return 0;

	while (t != EOF) {
		int key = 0;
		t = fget_int(&key, fd);
		printf("%d", key);
		char* str1 = (char*)malloc(9999);
		char* str2 = (char*)malloc(9999);
		fscanf(fd, "%*c", 0);
		fscanf(fd, "%[^\n]\n", str1);
		fscanf(fd, "%[^\n]\n", str2);
		if (t != EOF) {
			InfoType* info = create_info(str1, str2);
			int res = tree_insert(tree, key, info);
			//D_Show(tree);
			printf("\n");
		}
	}
	fclose(fd);
	return 1;
}

int D_Timing(Tree** nnn)
{
	Tree* root = NULL;
	int n = 10, key[50000], k, cnt = 1000, i, m;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0) {
		for (i = 0; i < 50000; ++i)
			key[i] = rand()*rand();
		for (i = 0; i < cnt; ) {
			k = rand()*rand();
			if (tree_insert(&root, k, NULL) == SUCCESS) {
				++i;
				
			}
		}
		m = 0;
		first = clock();
		for (i = 0; i < 50000; ++i)
			if (find(&root, key[i]))
				++m;
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 -
			n) * cnt, last - first);
	}
	delTree(&root);
	return 1;
}

void read_b(Tree** tree) {
	printf("Enter file name -->\n");
	char* file_name = NULL;
	scanf("%*c");
	file_name = getstr();
	FILE* fd = NULL;
	fd = fopen(file_name, "r+b");
	int t = 0;
	while (1) {
		int key;
		fread(&key, sizeof(int), 1, fd);
		if (feof(fd))
			break;
		InfoType* info = create_info("\0", "\0");
		tree_insert(tree, key, info);
	}
	D_Show(tree);
	fclose(fd);
}