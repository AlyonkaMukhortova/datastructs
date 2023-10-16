#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "item.h"

void print_item(Item* item) {
	if (item == NULL)
		printf("NULL}\n");
	else
		printf("%f, %f, %s}\n", item->info->num1, item->info->num2, item->info->str);
	return 0;
}

Item* create_item(float num1, float num2, char* strinfo) {
	InfoType* newInfo = (InfoType*)malloc(sizeof(InfoType));
	Item* newItem = (Item*)malloc(sizeof(Item));
	newInfo->num1 = num1;
	newInfo->num2 = num2;
	newInfo->str = strinfo;
	//newItem->key1 = &key1;
	//newItem->key2 = &key2;
	newItem->info = newInfo;
	
	return newItem;
}

Item* copy_item(Item* item) {
	InfoType* newInfo = (InfoType*)malloc(sizeof(InfoType));
	Item* newItem = (Item*)malloc(sizeof(Item));
	newInfo->num1 = item->info->num1;
	newInfo->num2 = item->info->num2;
	newInfo->str = item->info->str;
	//newItem->key1 = &key1;
	//newItem->key2 = &key2;
	newItem->info = newInfo;
	newItem->key1 = item->key1;
	return newItem;
}