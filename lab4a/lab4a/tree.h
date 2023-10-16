#ifndef TREE_H
#define TREE_H
#include "info.h"


typedef struct Tree {
	int key;
	InfoType* info;
	struct Tree* next;
	struct Tree* left;
	struct Tree* right;
	struct Tree* parent;
}Tree;

int init_tree(Tree* tree);
int tree_insert(Tree* tree, int key, InfoType* info);
Tree* tree_find(Tree* tree, int key);
int show_tree(Tree* tree);
int remove_tree(Tree* tree, int key);
Tree* tree_find_version(Tree* tree, int key, int version);
int show_tree_version(Tree* tree, int version);
int tree_find_without_res(Tree* tree, int key);
int remove_tree_version(Tree* tree, int key, int version);
Tree* tree_find_for_remove(Tree* tree, int key, int* f);
int show_tree_only_one(Tree* tree);
Tree* find_tree_min_find(Tree* tree);
Tree* find_tree_min_remove(Tree* tree);
int show_tree_reverse(Tree* tree);
Tree* find_tree_min_find(Tree** tree, int* n);
int show_tree_tree(Tree** tree, int n);
int show_tree_nexts(Tree** tree);
Tree* find_tree_next(Tree** tree, Tree* x);
int find(Tree** tree, int key);
void show_tree_graphviz(Tree* tree, FILE* fd, int *n);


#endif // !TREE_H

