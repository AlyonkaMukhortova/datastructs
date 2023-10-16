#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "dialog.h"

(*fptr[])(Graph**) = { 0, D_Add, D_Find, D_Delete, D_Show, D_Timing, D_Generate, D_Map };

const char* msgs[] = { "0. Quit", "1. Add", "2. Find", "3. Delete", "4. Show", "5. Timing", "6.Generate", "7. Map" };

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

int D_Add(Graph** graph) {
	int n;
	printf("1. Add Node\n2. Add edge\n");
	get_int(&n);
	if (n == 1) {
		float x, y;
		char* name = "\0", * s;
		//char* key1 = key;
		printf("Enter x \n");
		get_float(&x);
		printf("Enter y\n");
		get_float(&y);
		printf("Enter name\n");
		scanf("%*c");
		name = getstr();
		int res = insert_node(graph, name, x, y);
		if (res == SUCCESS)
			printf("Item has been added successfully\n");
		else
			printf("Key exists in table.\n");
	}
	else if (n == 2) {
		char* name1 = "\0";
		char* name2 = "\0";
		int weight;
		printf("Enter name 1\n");
		scanf("%*c");
		name1 = getstr();
		printf("Enter name2\n");
		//scanf("%*c");
		name2 = getstr();
		printf("Enter weight \n");
		get_int(&weight);
		int a = insert_edge(graph, name1, name2, weight);
		if (a == SUCCESS)
			printf("Item has been added successfully\n");
		else if (a == EXISTS)
			printf("Edge exists in table.\n");
		else
			printf("No such name\n");
		
	}
}

int D_Find(Graph** graph) {
	Graph* result = NULL;
	int n = 0, p;
	printf("1. Find node\n2. Find min path\n3. Decomposition\n");
	get_int(&p);
	if (p == 1) {
		printf("Enter name1\n");
		char* name1;
		scanf("%*c");
		name1 = getstr();
		printf("Enter name2\n");
		char* name2;
		name2 = getstr();
		result = find_node_depth(*graph, name1, name2);
		show_graph_only_one(&result);
	}
	else if (p == 2) {
		printf("Enter name1\n");
		char* name1;
		scanf("%*c");
		name1 = getstr();
		printf("Enter name2\n");
		char* name2;
		name2 = getstr();
		//p = Bellman_Ford(*graph, name1, name2);
		Graph* res = NULL;
		res = Bellman_Ford_Path(*graph, name1, name2, &p);
		if (p == BELOW_ZERO)
			printf("Below Zero cycle\n");
		else if (p == NOT_FOUND)
			printf("Not Found\n");
		else {
			show_graph_reverse(res, *graph);
			printf("Path Length:	%d\n", p);
		}
		free(res);
	}
	else if (p == 3) {
		int s = 0, i = 0;
		Graph** o = decomposition(*graph, &s);
		i = s;
		while (s >= 0) {
			printf("\nGRAPH#%d\n", i - s + 1);
			D_Show(o);
			o++;
			s--;
		}
	}
	return 1;
}

int D_Show(Graph** graph) {
	show_graph(graph);
	printf("Enter file name -->  ");
	//scanf("%*c");
	char* fname = "";
	while(strcmp(fname, "") == 0)
		fname = getstr();
	FILE* fd;
	fopen_s(&fd, fname, "w");
	fputs("digraph G{ \n", fd);
	//fputs("\tgraph [ordering=\"out\"];\n", fd);
	int n = 0;
	show_graph_graphviz(* graph, fd);
	fputs("}", fd);
	fclose(fd);
	printf("\n");
	/*show_tree(tree, 0);
	printf("\n");*/
	return 1;
}

int D_Delete(Graph** graph) {
	printf("1. Delete node.\n2. Delete egde.\n");
	int a;
	get_int(&a);
	if (a == 1) {
		printf("Enter name\n");
		char* name;
		scanf("%*c");
		name = getstr();
		int j = remove_node(graph, name);
		if (j == SUCCESS)
			printf("Elemet has been deleted successfully\n");
		else if (j == NOT_FOUND)
		printf("No such element in table");
	}
	else if (a == 2) {
		printf("Enter name1\n");
		scanf("%*c");
		char* name1 = getstr();
		
		printf("Enter name2\n");
		char* name2 = getstr();
		int j = remove_edge(graph, name1, name2);
		if (j == SUCCESS)
			printf("Elemet has been deleted successfully\n");
		else if (j == NOT_FOUND)
			printf("No such element in table");
	}
	return 1;
}

char* D_Load(Graph** graph)
{
	char* fname = NULL;
	printf("Enter file name: --> ");
	fname = getstr();
	if (fname == NULL)
		return 0;
	load(graph, fname);
	//free(fname);
	return fname;
}

int D_Save(Graph** graph, char* name)
{
	char* fname = name;
	printf("1. Save\n2. Do Not Save\n");
	int p = 2;
	get_int(&p);
	if (p ==1)
		save(graph, fname);
	free(fname);
	return 1;
}

int D_Generate(Graph** graph) {
	printf("Enter number of Nodes\n");
	int n;
	get_int(&n);
	*graph = Generate_Graph(n);
	return 1;
}

save(Graph** graph, char* fname) {
	FILE* fd;
	int t = 0;
	fopen_s(&fd, fname, "w");
	if (fd == NULL)
		return 0;
	fprintf(fd, "name\n");
	fprintf(fd, "x        y    List of edges\n");
	int size = (*graph)->size;
	for (int i = 0; i < size; i++) {
		fprintf(fd, "%s\n", (*graph)[i].name);
		fprintf(fd, "%f %f", (*graph)[i].x, (*graph)[i].y);
		List* list = (*graph)[i].list;
		while (list != NULL) {
			fprintf(fd, "	%d %d", list->index, list->weight);
			list = list->next;
		}
		fprintf(fd, "\n");
	}
	fclose(fd);
	return 1;
}

int fget_int_map(int* a, FILE* fd) {
	int n;
	do {
		char b;
		//fscanf(fd, "%c", &b);
		//if (b == '\n')
			//return 0;
		n = fscanf(fd, "%d", a);
		if (n == 0) {
			//scanf("%*c");
			//scanf("%*c");
			return n;
		}
	} while (n == 0);
	return n;
}

int fget_int(int* a, FILE* fd) {
	int n;
	do {
		char b;
		fscanf(fd, "%c", &b);
		if (b == '\n')
			return 0;
		n = fscanf(fd, "%d", a);
		if (n == 0) {
			//scanf("%*c");
			//scanf("%*c");
			return n;
		}
	} while (n == 0);
	return n;
}

int fget_float(int* a, FILE* fd) {
	int n;
	do {
		n = fscanf(fd, "%f", a);
		if (n == 0) {
			printf("Error\n");
			fscanf(fd, "%*c", 0);
		}
	} while (n == 0);
	return n;
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
			fscanf(fd,"%*c");
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

int load(Graph** graph, char* fname)
{
	FILE* fd;
	int t = 0;
	char* s = (char*)malloc(100);
	fopen_s(&fd, fname, "r");
	
	if (fd == NULL)
		return 0;
	fscanf(fd, "%[^\n]\n", s);
	fscanf(fd, "%[^\n]\n", s);
	free(s);
	while (t != EOF) {
		float x, y;
		char* name = "\0";// (char*)malloc(99);
		int index, weight;
		//if (t>0)
			//fscanf(fd, "%*c", 0);
		//fscanf(fd, "%[^\n]\n", name);
		name = fgetstr(fd);
		if (name == NULL)
			break;
		name[strlen(name)] = '\0';
		t = fget_float(&x, fd);
		t = fget_float(&y, fd);
		if (t != EOF) {
			int res = insert_node(graph, name, x, y);
			
			printf("\n");
		}
		while (t > 0) {
			t = fget_int(&index, fd);
			//t = fget_float(fd, "%d", &weight);
			if (t > 0)
				t = fget_int(&weight, fd);
			if (t>0)
				insert_edge_load(graph, name, index, weight);
		}
		
		if (t > 0)
			t++;
	}
	fclose(fd);
	D_Show(graph);
	return 1;
}

Graph* Generate_Graph(int n) {
	Graph* graph = NULL;
	for (int i = 0; i < n; i++) {
		char* name = rand_str();
		float x = rand() * rand();
		float y = rand() * rand();
		int a = insert_node(&graph, name, x, y);
		if (a != SUCCESS)
			i--;
	}
	for (int i = 0; i < n * 10; i++) {
		int weight = rand() * rand();
		int index1 = (rand() * rand())% n;
		int index2 = (rand()*rand()) % n;
		char* name1 = graph[index1].name;
		char* name2 = graph[index2].name;
		int a = insert_edge(&graph, name1, name2, weight);
		if (a != SUCCESS)
			i--;
	}
	return graph;
}

int D_Timing(Graph** nnn)
{
	
	int n = 10, k, cnt = 10, i, m, z;
	clock_t first, last;
	srand(time(NULL));
	while (n-- > 0) {
		Graph* graph = NULL;
		m = 0;
		graph = Generate_Graph((10- n) *cnt);
		first = clock();
		for (i = 0; i < 10000; ++i) {
			int index1 = rand()*rand() % cnt;
			int index2 = rand()*rand() % cnt;
			char* name1 = graph[index1].name;
			char* name2 = graph[index2].name;
			if (find_node_depth(graph, name1, name2)!= NULL)
			//if (a != NOT_FOUND)
				m++;	
		}
		
		last = clock();
		printf("%d items was found\n", m);
		printf("test #%d, number of nodes = %d, time = %d\n", 10 - n, (10 -
			n) * cnt, last - first);
		delGraph(&graph);
	}
	
	return 1;
}

char* rand_str() {
	//srand((unsigned int)time(NULL));
	int size = rand() % 19 + 2;
	char* str = (char*)malloc(size * sizeof(char));
	if (str == NULL)
	{
		printf("can't allocate memory");
		return NULL;
	}
	for (int i = 0; i < size - 1; i++)
	{
		str[i] = (rand()*rand()) % 83 + 40;
	}
	str[size] = '\0';
	return str;
}

int delGraph(Graph** graph) {
	if (*graph != NULL) {
		int size = (*graph)->size;
		for (int i = 0; i < size; i++) {
			Graph* ptr = &(*graph)[i];
			List* list = ptr->list;
			while (list != NULL) {
				List* l = list;
				list = list->next;
				free(l);
			}
			//free(ptr->name);
		}
		free(*graph);
	}
}

int D_Map(Graph** nnn) {
	int p = 1; 
	Graph* graph = NULL;
	printf("Enter file name for nodes\n");
	scanf("%*c");
	char* fname_nodes = getstr();
	printf("Enter file name for edges\n");
	char* fname_edges = getstr();
	load_map(&graph, fname_nodes, fname_edges);
	while (p != 0) {
		printf("0. Quit\n1. Find path\n");
		get_int(&p);
		if (p == 1) {
			printf("Enter name1\n");
			scanf("%*c");
			char* name1 = getstr();
			printf("Enter name2\n");
			//scanf("%*c");
			char* name2 = getstr();
			Graph* res = NULL;
			res = Bellman_Ford_Path(graph, name1, name2, &p);
			if (p == BELOW_ZERO)
				printf("Below Zero cycle\n");
			else if (p == NOT_FOUND)
				printf("Not Found\n");
			else {
				show_graph_reverse(res, graph);
				printf("Path Length:	%d\n", p);
			}
			free(res);
		}
	}
	delGraph(&graph);
	return 1;
}

void load_map(Graph** graph, char* fname_nodes, char* fname_edges) {
	FILE* fd, *fd1;
	int t = 0;
	char* s = (char*)malloc(100);
	fopen_s(&fd, fname_nodes, "r");

	if (fd == NULL)
		return 0;

	while (t != EOF) {
		float x, y;
		char* name = "\0";
		int index, weight;
		int iname;
		t = fget_int_map(&iname, fd);
		name = from_int_to_str(iname);
		if (name == NULL)
			break;
		name[strlen(name)] = '\0';
		t = fget_float(&x, fd);
		t = fget_float(&y, fd);
		if (t != EOF) {
			int res = insert_node(graph, name, x, y);
		}
		if (t > 0)
			t++;
	}
	fclose(fd); 
	t = 0;
	fopen_s(&fd1, fname_edges, "r");
	while (t != EOF) {
		float x, y;
		char* name1 = "\0",* name2 = "\0";
		int index, weight;
		int iname1, iname2;
		t = fget_int_map(&index, fd1);
		t = fget_int_map(&iname1, fd1);
		name1 = from_int_to_str(iname1);
		if (name1 == NULL)
			break;
		name1[strlen(name1)] = '\0';
		t = fget_int_map(&iname2, fd1);
		name2 = from_int_to_str(iname2);
		if (name2 == NULL)
			break;
		name2[strlen(name2)] = '\0';
		t = fget_int_map(&weight, fd1);
		char* s = (char*)malloc(20);
		fscanf(fd1, "%[^\n]", s);
		free(s);
		if (t != EOF) {
			int res = insert_edge(graph, name1, name2, weight);
			res = insert_edge(graph, name2, name1, weight);
		}
		if (t > 0)
			t++;
	}
	fclose(fd);
	D_Show(graph);
	return 1;
}