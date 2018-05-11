#ifndef __HEADFILELOG_H__

#define __HEADFILELOG_H__

#include <stdio.h>
#include <unistd.h> // access()
#include <string.h> // memset()
#include <time.h>

#include "iniparser.h" // parsing conf file

#define BUFF_SIZE 1024

int isFileExists(const char*);
void writeToLog( const int, const char* );

#endif
