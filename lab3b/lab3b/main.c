#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dialog.h"


// problems with tab->ks2[9].item = ???, initTable returns correct info


int main()
{
	Table table = { MSIZE1, MSIZE2, 0, NULL, NULL, NULL, NULL };
	int n = initTable_zeroes(&table);
	int rc;
	if (D_Load(&table) == 0)
		return 1;
	while (rc = dialog(msgs, NMsgs))
		if (!fptr[rc](&table))
			break;
	save(&table); 
	printf("That's all. Bye!\n");
	delTable(&table); 
	return 0;
}