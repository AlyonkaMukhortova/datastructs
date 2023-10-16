#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "tree.h"


int tree_insert(Tree** tree, int key, InfoType* info) {
	Tree* new_tree = (Tree*)malloc(sizeof(Tree));
	new_tree->info = info;
	new_tree->key = key;
	new_tree->left = NULL;
	new_tree->right = NULL;
	if (*tree == NULL) {
		new_tree->next = NULL;
		new_tree->parent = NULL;
		*tree = new_tree;
	}
	else {
		Tree* par = NULL;
		Tree *ptr = *tree;
		while (ptr != NULL) {
			par = ptr;
			if (new_tree->key < ptr->key) {
				ptr = ptr->left;
			}
			else if (new_tree->key == par->key) {
				InfoType* info = par->info;
				par->info = new_tree->info;
				if(par->info!=NULL)
					par->info->next = info;
				return SUCCESS;
			}
			else {
				ptr = ptr->right;
			}
		}
		if (new_tree->key < par->key) {
			par->left = new_tree;
		}
		else if (new_tree->key == par->key) {
			InfoType* info = par->info;
			par->info = new_tree->info;
			par->info->next = info;
		}
		else {
			par->right = new_tree;
		}
		new_tree->parent = par;
		Tree* head = find_tree_min_remove(tree);
		if (head == new_tree) {
			Tree* p = find_tree_next(tree, new_tree);
			new_tree->next = p;
		}
		else {
			Tree* q = head->next;
			while (head != NULL && q != NULL) {
				if (head->next->key < key) {
					head = head->next;
					//break;
				}
				q = q->next;
			}
			new_tree->next = head->next;
			head->next = new_tree;
		}
	}
	return SUCCESS;
}

Tree* tree_find(Tree** tree, int key, int *f) {
	if (*tree == NULL)
		return NULL;
	Tree* ptr = *tree;
	Tree* res = (Tree*)malloc(sizeof(Tree));
	int y;
	if (ptr == NULL) {
		res = NULL;
	}
	else {
		if (ptr->key == key) {
			res = ptr;
		}
		else {
			if (key < ptr->key) {
				res = tree_find(&ptr->left, key, f);
			}
			else
				res = tree_find(&ptr->right, key, f);
		}
	}
	res->left = NULL;
	res->right = NULL;
	*f = check_num(res->info);
	return res;
}

Tree* tree_find_for_remove(Tree** tree, int key, int* f) {
	if (*tree == NULL)
		return NULL;
	Tree* ptr = *tree;
	Tree* res;// = (Tree*)malloc(sizeof(Tree));
	int y;
	if (ptr == NULL) {
		res = NULL;
	}
	else {
		if (ptr!= NULL && ptr->key == key) {
			res = ptr;
		}
		else {
			if (key < ptr->key) {
				res = tree_find_for_remove(&ptr->left, key, f);
			}
			else
				res = tree_find_for_remove(&ptr->right, key, f);
		}
	}
	//res->left = NULL;
	//res->right = NULL;
	if( ptr!= NULL && res!= NULL)
		*f = check_num(res->info);
	return res;
}

int tree_find_without_res(Tree** tree, int key) {
	Tree* ptr = *tree;
	int n = 0;
	Tree* res = NULL;// (Tree*)malloc(sizeof(Tree));
	int y;
	if (ptr == NULL) {
		res = NULL;
	}
	else {
		if (ptr->key == key) {
			res = ptr;
		}
		else {
			if (key < ptr->key) {
				res = tree_find_for_remove(&ptr->left, key, &n);
			}
			else
				res = tree_find_for_remove(&ptr->right, key, &n);
		}
	}
	//res->left = NULL;
	//res->right = NULL
	if (res!= NULL)
		n = check_num(res->info);
	//free(res);
	return n;
}

int find(Tree** tree, int key) {
	Tree* ptr = *tree;
	int n = 0;
	Tree* res = NULL;// (Tree*)malloc(sizeof(Tree));
	int y;
	if (ptr == NULL) {
		res = NULL;
	}
	else {
		if (ptr->key == key) {
			res = ptr;
		}
		else {
			if (key < ptr->key) {
				res = find(&ptr->left, key);
			}
			else
				res = find(&ptr->right, key);
		}
	}
	//res->left = NULL;
	//res->right = NULL
	if (res != NULL) {
		//delTree(&res);
		return 1;
	}
	else
		return 0;
}

Tree* tree_find_version(Tree** tree, int key, int version) {
	Tree* ptr = *tree;
	Tree* res = (Tree*)malloc(sizeof(Tree));
	if (ptr == NULL) {
		res = NULL;
	}
	else {
		if (ptr->key == key) {
			res = ptr;
		}
		else {
			if (key < ptr->key) {
				res = tree_find_version(&ptr->left, key, version);
			}
			else
				res = tree_find_version(&ptr->right, key, version);
		}
	}
	res->left = NULL;
	res->right = NULL;
	InfoType* info = res->info;
	for (int i = 0; i < version-1; i++) {
		info = info->next;
	}
	//info->next = NULL;
	res->info = copy_info(info);
	res->info->next = NULL;
	return res;
}

Tree* version(Tree** tree, int version) {
	Tree* res = *tree;
	InfoType* info = res->info;
	InfoType* inf = info;
	for (int i = 0; i < version - 1; i++) {
		info = info->next;
	}
	//info->next = NULL;
	inf = copy_info(info);
	res->info = inf;
	//res->info->next = NULL;
	return res;
}

int show_tree_version(Tree** tree, int version) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		printf("{%d", ptr->key);
		print_info_version(ptr->info, version);
		//show_tree(&ptr->left);
		//show_tree(&ptr->right);
	}
	return 1;
}

int show_tree(Tree** tree) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		printf("{%d", ptr->key);
		print_info(ptr->info);
		show_tree(&ptr->left);
		show_tree(&ptr->right);
	}
}

int show_tree_only_one(Tree** tree) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		printf("{%d", ptr->key);
		print_info(ptr->info);
		//show_tree(ptr->left);
		//show_tree(ptr->right);
	}
}

Tree* find_tree_min_find(Tree** tree, int * n) {
	Tree* ptr = *tree;

	//ptr = ptr->right;
	if (*tree == NULL) {
		ptr = NULL;
	}
	else {
		while (ptr->left != NULL) {
			ptr = ptr->left;
		}
	}
	if (ptr != NULL)
		*n = check_num(ptr->info);
	return ptr;
}

Tree* find_tree_min_remove(Tree** tree) {
	Tree* ptr = *tree;
	if (*tree == NULL) {
		ptr = NULL;
	}
	else {
		while (ptr->left != NULL) {
			ptr = ptr->left;
		}
	}
	return ptr;
}

Tree* find_tree_max_remove(Tree** tree) {
	Tree* ptr = *tree;
	if (*tree == NULL) {
		ptr = NULL;
	}
	else {
		while (ptr->right != NULL) {
			ptr = ptr->right;
		}
	}
	return ptr;
}

Tree* find_tree_next(Tree** tree, Tree* x) {
	Tree* ptr = NULL;
	if (x != NULL && x->right != NULL)
		ptr = find_tree_min_remove(&x->right);
	else if (x!= NULL){
		ptr = x->parent;
		while (ptr != NULL && ptr->right == x) {
			x = ptr;
			ptr = ptr->parent;
		}
	}
	return ptr;
}

Tree* find_tree_previous(Tree** tree, Tree* x) {
	Tree* ptr = NULL;
	ptr = find_tree_min_remove(tree);
	while (ptr != NULL && ptr->next != x)
		ptr = ptr->next;
	/*if (x != NULL && x->right != NULL)
		ptr = find_tree_max_remove(&x->left);
	else if (x != NULL) {
		ptr = x->parent;
		while (ptr != NULL && ptr->left == x) {
			x = ptr;
			ptr = ptr->parent;
		}
	}*/
	return ptr;
}

int remove_tree(Tree** tree, int key) {
	int o;
	Tree* x = tree_find_for_remove(tree, key, &o);
	Tree* root = *tree;
	Tree* y = NULL;
	Tree* p = NULL;
	Tree* par = NULL;
	if (x == NULL)
		return NOT_FOUND;
	if (x->left == NULL || x->right == NULL) {
		y = x;
	}
	else {
		y = x->next;
		if (x->next == NULL) {
			y = find_tree_previous(tree, x);
			while (y->left != NULL && y->right != NULL) {
				y = find_tree_previous(tree, x);
			}
		}
		else
			while (y->left != NULL && y->right != NULL) {
				y = x->next;// find_tree_next(tree, x);
			}
	}
	if (y->left != NULL)
		p = y->left;
	else
		p = y->right;
	par = y->parent;
	if (p != NULL)
		p->parent = par;
	if (par == NULL)
		*tree = p;
	else {
		if (par->left == y)
			par->left = p;
		else
			par->right = p;
	}
	Tree* c = find_tree_previous(tree, x);
	if (c != NULL && c->next != NULL)
		c->next = c->next->next;
	if (y != x) {
		x->key = y->key;
		x->info = y->info;
		x->next = y->next;
		Tree* b = find_tree_previous(tree, y);
		b->next = x;
	}
	c = y;
	y = NULL;
	//free(c);
	return SUCCESS;
}

int remove_tree_version(Tree** tree, int key, int version) {
	int y = 0;
	Tree* ptr = tree_find_for_remove(tree, key, &y);
	if (version == 1)
		ptr->info = ptr->info->next;
	else {
		InfoType* q = ptr->info;
		for (int i = 1; i < version - 1; i++) {
			q = q->next;
		}
		q->next = q->next->next;
	}
	return SUCCESS;
}

int show_tree_reverse(Tree** tree) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		show_tree_reverse(&ptr->right);
		printf("{%d", ptr->key);
		print_info(ptr->info);
		show_tree_reverse(&ptr->left);
	}
}

delTree(Tree** tree) {
	if (*tree != NULL) {
		Tree* left = (*tree)->left;
		Tree* right = (*tree)->right;
		Tree* ptr = *tree;
		InfoType* info = (*tree)->info;
		free(info);
		free(ptr);
		delTree(&left);
		delTree(&right);
	}
}

int show_tree_tree(Tree** tree, int n) {
	Tree* ptr = *tree;
	if (ptr != NULL) {
		n++;
		show_tree_tree(&ptr->right, n);
		for (int i = 0; i < n-1; i++)
			printf("	");
		printf("{%d", ptr->key);
		print_info(ptr->info);
		show_tree_tree(&ptr->left, n);
	}
}

int show_tree_nexts(Tree** tree) {
	Tree* ptr = find_tree_min_remove(tree);
	while (ptr != NULL) {
		printf("{%d", ptr->key);
		print_info(ptr->info);
		ptr = ptr->next;
	}
}

void show_tree_graphviz(Tree* tree, FILE* fd, int*n) {
	if (tree != NULL) {
		if (tree->left != NULL) {
			fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->left->key);
			if (tree->right != NULL) {
				fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->right->key);
				show_tree_graphviz(tree->left, fd, n);
				show_tree_graphviz(tree->right, fd, n);
			}
			else {
				char* null = elist(*n);
				fprintf(fd, "\t%s[shape = point];\n", null);
				fprintf(fd, "\t\"%d\" -> \"%s\";\n", tree->key, null);
				(*n)++;
				show_tree_graphviz(tree->left, fd, n);
			}
		}
		else  if (tree->right != NULL){

			if (tree->left != NULL) {
				fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->left->key);
				show_tree_graphviz(tree->left, fd, n);
				show_tree_graphviz(tree->right, fd, n);
				fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->right->key);
			}
			
			else {
				char* null = elist(*n);
				fprintf(fd, "\t%s[shape = point];\n", null);
				fprintf(fd, "\t\"%d\" -> \"%s\";\n", tree->key, null);
				(*n)++;
				fprintf(fd, "\t\"%d\" -> \"%d\";\n", tree->key, tree->right->key);
				show_tree_graphviz(tree->right, fd, n);
			}
		}
		/*else {
			char* null = elist(*n);
			fprintf(fd, "\t\"%d\" -> \"%s\";\n", tree->key, null);
			(*n)++;
			null = elist(*n);
			fprintf(fd, "\t\"%d\" -> \"%s\";\n", tree->key, null);
			(*n)++;
		}*/
	}
}