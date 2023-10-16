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
	int offset; //смещение относительно начала файла
	int len; //длина информации (8 байт 2 числа + длина строки)
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
InfoType* create_info(float num1, float num2, char* strinfo);

#endif
