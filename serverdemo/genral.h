#ifndef __GENRAL_H__
#define __GENRAL_H__

#include "main.h"

#define PIDFILE 	"/opt/MicroWorld/var/run/eservcln.pid"

int daemonize(void);
int is_eservcln_running();
int savePidFile();
void removePidFile();
void releaseDynamicAllocatedMemory(void * mptr);
#endif
