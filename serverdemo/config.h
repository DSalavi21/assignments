/*******************************************************************************


********************************************************************************/

#ifndef __CONFIG_H__
#define __CONFIG_H__

#define ESERVCLN_CONF  "/opt/MicroWorld/etc/eservcln.conf"

typedef struct __eservcln_config 
{
        unsigned int	eservclnport;
	int    		debugLevel;
	int 		instances;
	char 		primaryServerIP[256];
	unsigned int	primaryServerPort;
	int 		primarySerConRetry;
	unsigned long 	eventcachesize;
	 
} eservcln_config;

int read_eservcln_config(const char *conffile, eservcln_config *conf);

#endif	// __CONFIG_H__

