#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "item.h"

/*void print_info(InfoType* info) {
	if (info == NULL)
		printf("NULL}\n");
	else
		printf("%f, %f, %s}\n", info->num1, info->num2, info->str);
	return 0;
}/**/

InfoType* create_info(float num1, float num2, char* strinfo) {
	InfoType* newinfo = (InfoType*)malloc(sizeof(InfoType));
	newinfo->num1 = num1;
	newinfo->num2 = num2;
	newinfo->str = strinfo;
	return newinfo;
}

InfoType* copy_info(InfoType* info) {
	InfoType* newinfo = (InfoType*)malloc(sizeof(InfoType));
	newinfo->num1 = info->num1;
	newinfo->num2 = info->num2;
	newinfo->str = info->str;
	return newinfo;
}

void print_item(Item* item) {
	if (item == NULL)
		printf("NULL}\n");
	else
		printf("len = %d, offset = %d}\n", item->len, item->offset);
	return 0;
}

Item* create_item(float num1, float num2, char* strinfo) {
	
	Item* newItem = (Item*)malloc(sizeof(Item));
	newItem->len = 9 + strlen(strinfo);

	return newItem;
}

Item* copy_item(Item* item) {
	Item* newItem = (Item*)malloc(sizeof(Item));
	newItem->len = item->len;
	newItem->key1 = item->key1;
	newItem->offset = item->offset;
	return newItem;
}