#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "graph.h"

#define	WHITE 111
#define GREY 222
#define BLACK 333

int insert_node(Graph** graph, char* name, float x, float y) {
	Graph* ptr;
	int q = find_node(*graph, name, x, y);
	if (q != NOT_FOUND)
		return EXISTS;
	if (*graph == NULL) {
		ptr = (Graph*)malloc(1 * sizeof(Graph));
		ptr->size = 1;
		ptr->name = name;
		ptr->x = x;
		ptr->y = y;
		ptr->list = NULL;
		*graph = ptr;
		return SUCCESS;
	}
	*graph = (Graph*)realloc(*graph, ((*graph)->size + 1) * sizeof(Graph));
	ptr = *graph;
	ptr[ptr->size].list = NULL;
	ptr[ptr->size].name = name;
	ptr[ptr->size].x = x;
	ptr[ptr->size].y = y;
	ptr->size = ptr->size + 1;
	for (int i = 1; i < ptr->size; i++) {
		ptr[i].size = ptr->size - i;
	}
	return SUCCESS;
}

int find_node(Graph* graph, char* name, float x, float y) {
	if (graph == NULL)
		return NOT_FOUND;
	for (int i = 0; i < graph->size; i++) {
		if (strcmp(graph[i].name, name) == 0 || graph[i].x  == x && graph[i].y == y)
			return i;
	}
	return NOT_FOUND;
}

int find_node_only_name(Graph* graph, char* name) {
	if (graph == NULL)
		return NOT_FOUND;
	for (int i = 0; i < graph->size; i++) {
		if (strcmp(graph[i].name, name) == 0)
			return i;
	}
	return NOT_FOUND;
}

int insert_edge(Graph** graph, char* name1, char* name2, int weight) {
	Graph* ptr = *graph;
	int first = find_node_only_name(ptr, name1);
	int second = find_node_only_name(ptr, name2);
	int list = find_edge(ptr, name1, name2);
	if (first == NOT_FOUND || second == NOT_FOUND)
		return NOT_FOUND;
	if (list != NOT_FOUND)
		return EXISTS;
	List* list1 = (List*)malloc(sizeof(List));
	list1->index = second;
	list1->weight = weight;
	list1->next = NULL;
	if (ptr[first].list == NULL) {
		ptr[first].list = list1;
	}
	else {
		List* list = ptr[first].list;
		while (list->next != NULL)
			list = list->next;
		list->next = list1;
	}
	return SUCCESS;
}

int insert_edge_load(Graph** graph, char* name1, int index, int weight) {
	Graph* ptr = *graph;
	int first = find_node_only_name(ptr, name1);
	int second = index;
	List* list1 = (List*)malloc(sizeof(List));
	list1->index = second;
	list1->weight = weight;
	list1->next = NULL;
	if (ptr[first].list == NULL) {
		ptr[first].list = list1;
	}
	else {
		List* list = ptr[first].list;
		while (list->next != NULL)
			list = list->next;
		list->next = list1;
	}
	return SUCCESS;
}

int remove_edge(Graph** graph, char* name1, char* name2) {
	Graph* ptr = *graph;
	int first = find_node_only_name(ptr, name1);
	int second = find_node_only_name(ptr, name2);
	int i = find_edge(ptr, name1, name2);
	if (first == NOT_FOUND || second == NOT_FOUND || i == NOT_FOUND)
		return NOT_FOUND;
	List* list = ptr[first].list;
	
	if (ptr[first].list->index == second) {
		ptr[first].list = ptr[first].list->next;
	}
	else {
		while (ptr[first].list->next != NULL && ptr[first].list->next->index != second)
			ptr[first].list = ptr[first].list->next;
		list = ptr[first].list->next;
		ptr[first].list->next = ptr[first].list->next->next;
	}
	free(list);
	return SUCCESS;
}

int remove_node(Graph** graph, char* name) {
	Graph* ptr = *graph;
	int index = find_node_only_name(ptr, name);
	if (index == NOT_FOUND)
		return NOT_FOUND;
	List* list = ptr[index].list;
	while (list != NULL) {
		List* l = list;
		ptr[index].list = ptr[index].list->next;
		list = list->next;
		free(l);
	}
	Graph** r = &ptr;
	int  n = ptr[index].size - 1;
	for (int i = 0; i < n; i++) {
		ptr[index +i] = ptr[index + 1+i];
	}
	//free(r);
	for (int i = 0; i < index ; i++) {
		ptr[i].size = ptr[i].size - 1;
	}
	ptr = (Graph*)realloc(ptr, ptr->size*sizeof(Graph));
	for (int i = 0; i < ptr[0].size; i++) {
		List* q = ptr[i].list;
		if (ptr[i].list != NULL && ptr[i].list->index == index) {
			//List* w = q;
			ptr[i].list = ptr[i].list->next;
			q = q->next;
			while (ptr[i].list != NULL) {
				if (ptr[i].list->index > index)
					ptr[i].list->index = ptr[i].list->index - 1;
				ptr[i].list = ptr[i].list->next;
				//q = q->next;
			}
			ptr[i].list = q;
			//free(w);
		}
		else if (ptr[i].list != NULL) {
				while (ptr[i].list->next != NULL && ptr[i].list->next->index != index) {
					if (ptr[i].list->index > index) {
						ptr[i].list->index = ptr[i].list->index - 1;
					}
					ptr[i].list = ptr[i].list->next;
					//q = q->next;
				}
				if (ptr[i].list->next != NULL) {
					//List* w = q->next->next;
					ptr[i].list->next = ptr[i].list->next->next;
					//q->next = w;
					//free(w);
				}
				while (ptr[i].list != NULL) {
					if (ptr[i].list->index> index)
						ptr[i].list->index = ptr[i].list->index - 1;
					ptr[i].list = ptr[i].list->next;
					//q = q->next;
				}
				ptr[i].list = q;
		}
		
	}
	return SUCCESS;
}

void show_graph(Graph** graph) {
	Graph* ptr = *graph;
	if (ptr != NULL) {
		for (int i = 0; i < ptr[0].size; i++) {
			printf("%d. {%s, %f, %f}:    ", i, ptr[i].name, ptr[i].x, ptr[i].y);
			List* list = ptr[i].list;
			while (list != NULL) {
				printf("(%d, %d)  ", list->index, list->weight);
				list = list->next;
			}
			printf("\n");
		}
	}
	else
		printf("Graph is EMPTY\n");
}

int find_edge(Graph* graph, char* name1, char* name2) {
	Graph* ptr = graph;
	int first = find_node_only_name(ptr, name1);
	int second = find_node_only_name(ptr, name2);
	if (first == NOT_FOUND || second == NOT_FOUND)
		return NOT_FOUND;
	List* l = ptr[first].list;
	while (l != NULL) {
		if (l->index == second)
			return SUCCESS;
		l = l->next;
	}
	return NOT_FOUND;
}

int Bellman_Ford(Graph* graph, char* name1, char* name2) {
	Graph* ptr = graph;
	int size = ptr->size;
	int first = find_node_only_name(graph, name1);
	int second = find_node_only_name(graph, name2);
	if (first == NOT_FOUND || second == NOT_FOUND)
		return NOT_FOUND;
	int* d = (int*)malloc(size * sizeof(int));
	int* prev = (int*)malloc(size * sizeof(int));
	
	for (int i = 0; i < size; i++) {
		d[i] = 2147483647;
		prev[i] = -1;
	}
	d[first] = 0;
	
	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			int index, u = i;
			
			if (u + first + 1 > size) {
				index = u + first - size;
			}
			else {
				index = u + first;
			}
			
			List* list = ptr[index].list;
			while (list != NULL) {
				if (d[list->index] > d[index] + list->weight) {
					if (k == size - 1) {
						free(d);
						free(prev);
						return BELOW_ZERO;
					}

					if (/*d[list->index] == 2147483647 && */d[index] == 2147483647 /*&& list->weight > 0*/)
						d[list->index] == 2147483647;
					//else if (d[list->index] == 2147483647 && d[index] == 2147483647 && list->weight < 0)
						//d[list->index] = 2147483647;// d[index] + list->weight;
					else if (/*d[list->index] == -2147483647 && */d[index] == -2147483647 /*&& list->weight < 0*/)
						d[list->index] == -2147483647;
					//else if (d[list->index] == -2147483647 && d[index] == -2147483647 && list->weight > 0)
						//d[list->index] = -2147483647;// d[index] + list->weight;
					else
						d[list->index] = d[index] + list->weight;
					prev[list->index] = index;

				}
				
				list = list->next;
			}
		}
	}
	int g = d[second];
	free(d);
	free(prev);
	return g;
}

Graph* Bellman_Ford_Path(Graph* graph, char* name1, char* name2, int* z) {
	Graph* ptr = graph;
	
	int size = ptr->size;
	int first = find_node_only_name(graph, name1);
	int second = find_node_only_name(graph, name2);
	if (first == NOT_FOUND || second == NOT_FOUND) {
		*z = NOT_FOUND;
		return NULL;
	}
	int* d = (int*)malloc(size * sizeof(int));
	int* prev = (int*)malloc(size * sizeof(int));

	for (int i = 0; i < size; i++) {
		d[i] = 2147483647;
		prev[i] = -1;
	}
	d[first] = 0;

	for (int k = 0; k < size; k++) {
		for (int i = 0; i < size; i++) {
			int index, u = i;

			if (u + first + 1 > size) {
				index = u + first - size;
			}
			else {
				index = u + first;
			}

			List* list = ptr[index].list;
			while (list != NULL) {
				if (d[index] != 2147483647 && d[list->index] > d[index] + list->weight) {
					if (k == size - 1) {
						free(d);
						free(prev);
						*z = BELOW_ZERO;
						return NULL;
					}

					if (/*d[list->index] == 2147483647 && */d[index] == 2147483647 /*&& list->weight > 0*/)
						d[list->index] == 2147483647;
					//else if (d[list->index] == 2147483647 && d[index] == 2147483647 && list->weight < 0)
						//d[list->index] = 2147483647;// d[index] + list->weight;
					else if (/*d[list->index] == -2147483647 && */d[index] == -2147483647 /*&& list->weight < 0*/)
						d[list->index] == -2147483647;
					//else if (d[list->index] == -2147483647 && d[index] == -2147483647 && list->weight > 0)
						//d[list->index] = -2147483647;// d[index] + list->weight;
					else
						d[list->index] = d[index] + list->weight;
					prev[list->index] = index;

				}

				list = list->next;
			}
		}
	}
	/*for (int i = 0; i < size; i++) {

		List* list = ptr[i].list;
		while (list != NULL) {
			if (d[list->index] > d[i] + list->weight) {
				free(d);
				free(prev);
				*z = BELOW_ZERO;
				return BELOW_ZERO;
			}
			list = list->next;
		}
	}*/
	int r = second;
	if (prev[r] == -1) {
		*z = NOT_FOUND;
		return NULL;
	}
	int e = 2;
	Graph* res = (Graph*)malloc(sizeof(Graph));
	res[0] = graph[second];
	while (prev[r] != first) {
		res = (Graph*)realloc(res, e * sizeof(Graph));
		res[e - 1] = graph[prev[r]];
		r = prev[r];
		e++;
	}
	res = (Graph*)realloc(res, e * sizeof(Graph));
	res[e - 1] = graph[first];
	for (int j = 0; j < e; j++) {
		res[j].size = e - j;
	}
	int g = d[second];
	*z = g;
	free(d);
	free(prev);
	return res;
}

Graph* find_node_depth(Graph* graph, char* name1, char* name2) {
	int size = graph[0].size;
	int* color = (int*)malloc(size * sizeof(int));
	int* prev = (int*)malloc(size * sizeof(int));
	int* d = (int*)malloc(size * sizeof(int));
	int* f = (int*)malloc(size * sizeof(int));
	int first = find_node_only_name(graph, name1);
	int second = find_node_only_name(graph, name2);
	int time = 0;
	for (int i = 0; i < size; i++) {
		d[i] = 0;
		f[i] = 0;
		color[i] = WHITE;
		prev[i] = -1;
	}
	
		
	DFS_Visit(graph, first, d, f, color, prev, &time);
	int n = color[second];
	free(d);
	free(f);
	free(prev);
	free(color);
	if (n != WHITE)
		return &(graph[second]);
	else
		return NULL;
}

void DFS_Visit(Graph* graph, int u, int* d, int* f, int* color, int* prev, int* time) {
	color[u] = GREY;
	*time = *time + 1;
	d[u] = *time;
	List* list = graph[u].list;
	while (list != NULL) {
		if (color[list->index] == WHITE) {
			prev[list->index] = u;
			DFS_Visit(graph, list->index, d, f, color, prev, time);
		}
		list = list->next;
	}
	color[u] = BLACK;
	*time = *time + 1;
	f[u] = *time;
}

int* find_node_depth_all(Graph* graph) {
	int size = graph[0].size;
	int* color = (int*)malloc(size * sizeof(int));
	int* prev = (int*)malloc(size * sizeof(int));
	int* d = (int*)malloc(size * sizeof(int));
	int* f = (int*)malloc(size * sizeof(int));
	int time = 0;
	for (int i = 0; i < size; i++) {
		d[i] = 0;
		f[i] = 0;
		color[i] = WHITE;
		prev[i] = -1;
	}
	
	for (int i = 0; i < size; i++) {
		if (color[i] == WHITE) {
			
			DFS_Visit(graph, i, d, f, color, prev, &time);
			
		}
	}
	return f;
}

int* find_node_depth_decomposition(Graph* graph, int* t, int* d) {
	int size = graph[0].size;
	int* color = (int*)malloc(size * sizeof(int));
	int* prev = (int*)malloc(size * sizeof(int));
	//d = (int*)malloc(size * sizeof(int));
	int* f = (int*)malloc(size * sizeof(int));
	int time = 0;
	for (int i = 0; i < size; i++) {
		d[i] = 0;
		f[i] = 0;
		color[i] = WHITE;
		prev[i] = -1;
	}
	for (int i = 0; i < size; i++) {
		if (color[t[i]] == WHITE) {
			DFS_Visit(graph, t[i], d, f, color, prev, &time);
		}
	}
	return f;
}

int find_value(int* array, int value) {
	int i = 0;
	while (*array) {
		if (*array == value)
			return i;
		i++;
		array++;
	}
	return NOT_FOUND;
}

Graph** decomposition(Graph* graph, int* z) {
	int size = graph->size;
	Graph* gt = NULL;
	for (int i = 0; i < size; i++) {
		insert_node(&gt, graph[i].name, graph[i].x, graph[i].y);
	}
	for (int i = 0; i < size; i++) {
		List* list = graph[i].list;
		while (list != NULL) {
			insert_edge(&gt, graph[list->index].name, graph[i].name, list->weight);
			list = list->next;
		}
	}
	//show_graph(&graph);
	//show_graph(&gt);
	int* f = find_node_depth_all(graph);
	int* t = (int*)calloc(size, sizeof(int));// массив индексов по убыванию времени
	for (int i = 0; i < size; i++) {
		int max = 0;
		for (int j = 0; j < size; j++) {
			if (f[j] > f[max])
				max = j;
		}
		t[i] = max;
		f[max] = 0;
	}
	int* d = (int*)malloc(size*sizeof(int));
	f = find_node_depth_decomposition(gt, t, d);
	int count = 0;
	int q = 0;
	int s = 0;
	Graph** res = (Graph**)malloc(sizeof(Graph*));
	while (q < size) {
		res = (Graph**)realloc(res, (s + 1) * sizeof(Graph*));
		res[s] = NULL;
		int index = find_value(d, count + 1);
		insert_node(&(res[s]), graph[index].name, graph[index].x, graph[index].y);
		List* list1 = graph[index].list;
		while (list1 != NULL) {
			insert_edge_load(&(res[s]), graph[index].name, list1->index, list1->weight);
			list1 = list1->next;
		}
		int num = (f[index] - d[index] + 1) / 2;
		int c = count + 1;
		for (int j = 1; j <= num; j++) {
			
			int r = find_value(d, c/*ount + j + 1*/);
			while (r<0 || r > size) {
				c++;
				r = find_value(d, c);
				if (c == size * 2)
					r = -1;
			}
			if (r == -1)
				break;
			insert_node(&(res[s]), graph[r].name, graph[r].x, graph[r].y);
			List* list = graph[r].list;
			while (list != NULL) {
				insert_edge_load(&(res[s]), graph[r].name, list->index, list->weight);
				list = list->next;
			}
			c++;
		}
		count = f[index];
		q += num;
		s++;
	}
	*z = s - 1;
	return res;
}

void show_graph_only_one(Graph** graph) {
	Graph* ptr = *graph;
	if (ptr != NULL) {
		
		printf("{%s, %f, %f}:    ", ptr->name, ptr->x, ptr->y);
		List* list = ptr->list;
		while (list != NULL) {
			printf("(%d, %d)  ", list->index, list->weight);
			list = list->next;
		}
		printf("\n");
		
	}
	else
		printf("NO WAY");
}

void show_graph_reverse(Graph* graph, Graph* batya) {
	if (graph == NULL) {
		printf("Graph is EMPTY");
	}
	int size = graph->size;
	for (int i = 0; i < size; i++) {
		printf("%d. {%s, %f, %f}:	", i, graph[size-i-1].name, graph[size - i-1].x, graph[size - i-1].y);
		List* list = graph[size - i - 1].list;
		while (list != NULL) {
			if (i != size - 1)
				if (strcmp(batya[list->index].name, graph[size - i - 2].name) == 0)
					printf("(%d, %d)  ", i + 1, list->weight);
			list = list->next;
		}
		printf("\n\n");
	}
}

char* from_int_to_str(int num) {
	char* str = NULL;
	int size = 0;
	int num1 = num;
	while (num1 > 0) {
		num1 = num1 / 10;
		size++;
	}
	if (num == 0)
		size++;
	str = (char*)malloc((size + 1) * sizeof(char));
	if (size < 2) {
		str[0] = num + '0';
		str[1] = '\0';
	}

	else {
		int count = size - 1;
		while (num > 0) {
			str[count] = (num % 10) + '0';
			num = num / 10;
			count--;
		}
	}
	str[size] = '\0';
	return str;
}

void show_graph_graphviz(Graph* graph, FILE* fd) {
	
	Graph* ptr = graph;
	if (ptr != NULL) {
		int size = graph->size;
		for (int i = 0; i < size; i++) {
			List* list = ptr[i].list;
			while (list != NULL) {
				fprintf(fd, "\t\"%s\" -> \"%s\" [label = %d];\n", ptr[i].name, ptr[list->index].name, list->weight);
				list = list->next;
			}
			//printf("\n");
		}
	}
	else
		fprintf(fd, "\t\"Graph is EMPTY\";\n");
}