#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "info.h"

InfoType* create_info(char* str1, char* str2) {
	InfoType* new_info = (InfoType*)malloc(sizeof(InfoType));
	new_info->str1 = str1;
	new_info->str2 = str2;
	new_info->next = NULL;
	return new_info;
}



void print_info(InfoType* info) {
	InfoType* ptr = info;
	while (ptr != NULL) {
		printf(", %s, %s", ptr->str1, ptr->str2);
		ptr = ptr->next;
	}
	printf("}\n");
}

void print_info_version(InfoType* info, int version) {
	InfoType* ptr = info;
	for (int i = 0; i < version - 1; i++) {
		ptr = ptr->next;
	}
	printf(", %s, %s", ptr->str1, ptr->str2);
	printf("}\n");
}

int check_num(InfoType* info) {
	InfoType* ptr = info;
	int n = 0;
	while (ptr != NULL) {
		n++;
		ptr = ptr->next;
	}
	return n;
}

InfoType* copy_info(InfoType* info) {
	InfoType* new_info = (InfoType*)malloc(sizeof(InfoType));
	new_info->str1 = info->str1;
	new_info->str2 = info->str2;
	new_info->next = NULL;
	return new_info;
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

char* elist(int num) {
	char* number = from_int_to_str(num);
	int i = strlen(number);
	char* null = (char*)malloc(5 + i);
//	scanf("[\n]\n", null);
	char* n = "null";
	null = strcpy(null, n);
	null = strcat(null, number);
	return null;
}