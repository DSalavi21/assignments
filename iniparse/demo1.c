#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#include "iniparser.h"

int  parse_conf_file(char * conf_name);

int main(int argc, char * argv[])
{
	int status = 0;

	if( argc <  2 ) 
	{
        	status = parse_conf_file("/root/programs/assignments/libraries/conf/logLevel.conf"); // default conf file
    	} 
	else 
	{
        	status = parse_conf_file(argv[1]);
    	}
    	return status;
}


int parse_conf_file(char *conf_name) // input configuration file e.g. file.ini, file.cfg, file.conf
{
    int i;

    /* To parse config file, create dictionary structure pointer */
    dictionary *dict_p; 

    /* launch parser */
    dict_p = iniparser_load(conf_name);
    if ( dict_p == NULL) {
        fprintf(stderr, "cannot parse file: %s\n", conf_name);
        return -1 ;
    }

    	/* Dump a dictionary to an opened file pointer */
    	// iniparser_dump(dict_p, stderr);

	/* Get the string associated to a key, convert to an int */
   	/* get setting attributes */
	i = iniparser_getint(dict_p, "setting:loglevel", -1);
	printf("LogLevel:   [%d]\n", i);


    /* Free all memory associated to an ini dictionary */ 
    iniparser_freedict(dict_p);
    return 0 ;
}


//######################## logLevel.conf ############################
//
// #[section]
// #	key = value	
//
// [setting]
//        LogLevel = "4"
//
//###################################################################

//########################## Makefile ###############################
//
// CC      = gcc
// CFLAGS  = -I/root/ini_parser/iniparser/src/          # iniparser projet header file path > iniparser.h
// LFLAGS  = -L/root/ini_parser/iniparser/ -liniparser  # library location > libiniparser.a *[ created after 'make' Makefile at project root ]
//
// all : demo1 
//
// demo1 : 
//	$(CC) -g -o demoexe demo1.c $(CFLAGS) $(LFLAGS)
//
// clean :
//	rm -f demoexe 
//
//###################################################################



