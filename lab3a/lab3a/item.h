#ifndef ITEM_H
#define ITEM_H

#define MSIZE1 5
#define MSIZE2 5




typedef struct InfoType {
	float num1;
	float num2;
	char* str;
}InfoType;

typedef struct Item {
	char* key1;
	int index2;
	InfoType* info;
}Item;

enum Errors_e {
	SUCCESS = 0,
	KS_IS_FULL = -1,
	NOT_FOUND = -2,
	EXISTS = -3
};

void print_item(Item* item);
Item* create_item(float num1, float num2, char* strinfo);
Item* copy_item(Item* item);

#endif