#ifndef __MAIN_H__
#define __MAIN_H__

#include <stdio.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <sys/types.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <strings.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <unistd.h>
#include <queue>
#include <string>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/time.h>
#include <sys/resource.h>

#define DBG_PATH                        "/var/MicroWorld/var/log/eservcln.log"
#define DEFAULT_DEBUGLEVEL              4

#define XPRINT_INIT() dbg = ((dbg == NULL) ? fopen(DBG_PATH, "a+") : dbg)
#define XPRINT_STOP() if(dbg) { fclose(dbg); dbg = NULL; }
#define XXPRINT(dbglevel, ...) if( dbg && dbglevel <= DEFAULT_DEBUGLEVEL ) { \
                                char printstring[20]; \
                                struct tm *ltime; \
                                time_t sinceepoch; \
                                sinceepoch=time(NULL); \
                                ltime = localtime(&sinceepoch); \
                                sprintf(printstring, "%02d-%02d-%02d %02d:%02d:%02d",\
                                ltime->tm_mon+1, ltime->tm_mday, 1900 + ltime->tm_year, ltime->tm_hour, ltime->tm_min, ltime->tm_sec); \
                                fprintf(dbg,"%s=> ", printstring); \
                                fprintf(dbg,__VA_ARGS__); fflush(dbg); \
                             }

typedef struct ThreadData
{
        pthread_t               threadid;
        int                     sockfd;
        bool                    success;
        int                     *ac;
        unsigned long           eventcachesize;
	struct sockaddr_in      *client;
}ThreadData;

typedef struct PrimaryServerData
{
        char                    primaryServIp[256];
        unsigned int            primaryServPort;
        int                     primaryServConnRetry;
}PrimaryServerData;

#endif
