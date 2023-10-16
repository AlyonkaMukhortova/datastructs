#ifndef GRAPH_H
#define GRAPH_H

typedef struct List {
	int weight;
	struct List* next;
	int index;
}List;


typedef struct Graph {
	char* name;
	float x;
	float y;
	struct List* list;
	int size;
}Graph;

enum Errors_e {
	SUCCESS = 0,
	NOT_FOUND = -22,
	EXISTS = -3,
	BELOW_ZERO = -4
};

int insert_node(Graph** graph, char* name, float x, float y);
int find_node(Graph* graph, char* name, float x, float y);
int find_node_only_name(Graph* graph, char* name);
int insert_edge(Graph** graph, char* name1, char* name2, int weight);
int remove_edge(Graph** graph, char* name1, char* name2);
int remove_node(Graph** graph, char* name);
void show_graph(Graph** graph);
int find_edge(Graph* graph, char* name1, char* name2);
int Bellman_Ford(Graph* graph, char* name1, char* name2);
Graph* find_node_depth(Graph* graph, char* name1, char* name2);
void DFS_Visit(Graph* graph, int u, int* d, int* f, int* color, int* prev, int* time);
Graph** decomposition(Graph* graph, int * z);
int insert_edge_load(Graph** graph, char* name1, int index, int weight);
void show_graph_only_one(Graph** graph);
Graph* Bellman_Ford_Path(Graph* graph, char* name1, char* name2, int* d);
void show_graph_reverse(Graph* graph, Graph* batya);
char* from_int_to_str(int num);
void show_graph_graphviz(Graph* graph, FILE* fd);

#endif // !GRAPH_H

