#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dialog.h"



int main()
{
	Graph* graph = NULL;
	int rc;
	char* fname = NULL;
	fname = D_Load(&graph);
		
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](&graph))
			break;
	printf("That's all. Bye!\n");
	D_Save(&graph, fname);
	delGraph(&graph);
	return 0;
}