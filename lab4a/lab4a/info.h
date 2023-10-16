#ifndef INFO_H
#define INFO_H

typedef struct InfoType {
	char* str1;
	char* str2;
	struct InfoType* next;
}InfoType;

enum Errors_e {
	SUCCESS = 0,
	NOT_FOUND = -2,
	EXISTS = -3
};

InfoType* create_info(char* str1, char* str2);
void print_info(InfoType* info);
int check_num(InfoType* info);
InfoType* copy_info(InfoType* info);
void print_info_version(InfoType* info, int version);
char* from_int_to_str(int num);
char* elist(int num);

#endif // !INFO_H

