#include <stdio.h>
#include "headfileLog.h"

int main()
{
	FILE *fp = fopen("Unknown","r");
	
	if( fp != NULL )
	{
		printf("\tOpened Successfully\n");
		fclose(fp);
	}
	else
	{
		writeToLog(1,"Failed to open file");
	}

	return 0;	
}


