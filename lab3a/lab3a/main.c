#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dialog.h"


//   проблемы с пустой строкой


int main()
{
	Table table = {NULL,NULL,MSIZE1,MSIZE2};
	int f;
	f = initTable_zeroes(&table);
	int rc;
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](&table))
			break;
	printf("That's all. Bye!\n");
	delTable(&table); 
	return 0;
	//printf("1230");
}