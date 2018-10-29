
#include <stdio.h>
#include <stdlib.h>
#include "sfl.h"

#include <iostream>
using namespace std;

int readConf( char *filename )
{
	SYMTAB *Table=NULL;
	SYMBOL *sym=NULL;
	char *cPtr = NULL;

	if ((Table=sym_create_table()) == NULL)
	{
                return -1;
	}

	if (!file_exists(filename))	
	{
		return -1;
	}

	if (ini_dyn_load(Table,filename) == NULL)
        {
		sym_delete_table(Table);
		return -1;
        }

	cPtr = sym_get_value (Table, "section:value1", NULL);
        if(cPtr == NULL)
        {
		sym_delete_table(Table);
                return -1;
	}
	//cout << "value1 is : " << cPtr <<endl;
	printf("value1 is : %s\n", cPtr);

	cPtr = sym_get_value (Table, "section:value2", NULL);
        if(cPtr == NULL)
        {
                sym_delete_table(Table);
                return -1;
        }
        //cout << "value2 is : " << cPtr <<endl;
	printf("value2 is : %s\n", cPtr);
	
	return 0;
}

int main(int argc, char **argv)
{
	cout << "Inside main()..." <<endl;	
	
	if( argc != 2 )
	{
		cout<< "use :\t[./runSFL] [filename]" <<endl;
		return -1;
	}

	if( readConf(argv[1]) == 0 )
	        cout << "success..." <<endl;
	else
	        cout << "fail..." <<endl;


	return 0;
}

