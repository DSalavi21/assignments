/****************************************************************************************

 ****************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "sfl.h"
#include "config.h"

#define MAX_INST        	50       //Maximum number of worker threads
#define MIN_INST	        5        //Minimum number of worker threads
#define DEFAULT_INST        	10       //Default number of worker threads

#define MAX_CACHESIZE    	50000    //Maximum number of nodes in Event List
#define MIN_CACHESIZE	        30000    //Minimum number of nodes in Event List
#define DEFAULT_CACHESIZE       40000    //Default number of nodes in Event List

int read_eservcln_config(const char *conffile, eservcln_config *conf)
{
	SYMTAB *Table = NULL;
	char *tmp;
	int n = 0;
	unsigned long eventcachesize = 0;
	unsigned int port;
	if (access(conffile, F_OK) != 0 )
		return (-1);

	Table = sym_create_table();
	Table = ini_dyn_load (Table, conffile);//load conf file

	if(!Table)
		return -1;

	if((port = atoi(sym_get_value (Table, "eservcln:eservclnport", NULL))) != 0)
		conf->eservclnport = port;

	if((tmp = sym_get_value (Table, "eservcln:debuglevel", NULL)) != NULL)
	{
		int tmpDbgLevel = atoi(tmp);
		if( tmpDbgLevel <= 0 || tmpDbgLevel > 4 )
			tmpDbgLevel = 1;

		conf->debugLevel = tmpDbgLevel;
	}
	else
	{
		conf->debugLevel = 1;
	}
	
	if((n = atoi(sym_get_value (Table, "eservcln:instances", NULL))) != 0)
	{
		if( n < MIN_INST)
		conf->instances = DEFAULT_INST;
		else if(n > MAX_INST)	
		conf->instances = DEFAULT_INST;	
		else
		conf->instances = n;
	}

	if((n = atoi(sym_get_value (Table, "eservcln:connectionretrytime", NULL))) != 0)
		conf->primarySerConRetry = n;

	if((tmp = sym_get_value (Table, "eserv:eservip", NULL)) != NULL)
                strncpy(conf->primaryServerIP, tmp, sizeof(conf->primaryServerIP));

	if((port = atoi(sym_get_value (Table, "eserv:eservport", NULL))) != 0)
		conf->primaryServerPort = port;	
	
	if((eventcachesize =(unsigned long) atol(sym_get_value (Table, "eventcache:cachesize", NULL))) != 0)
	{
		
                if( eventcachesize < MIN_CACHESIZE)
                conf->eventcachesize = DEFAULT_CACHESIZE;
                else if(eventcachesize > MAX_CACHESIZE)
                conf->eventcachesize = DEFAULT_CACHESIZE;
                else
                conf->eventcachesize = eventcachesize;
        }

	sym_delete_table (Table);

	return conf->eservclnport ? 0 : -1;
}

