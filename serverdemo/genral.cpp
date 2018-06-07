
#include "genral.h"

/***********************************************************************************
  Function for convert running process into daemon 
 ************************************************************************************/

int daemonize(void)
{
        pid_t pid;
        int fds[3],i;

        fds[0] = open("/dev/null", O_RDONLY);
        fds[1] = open("/dev/null", O_WRONLY);
        fds[2] = open("/dev/null", O_WRONLY);
        if(fds[0] == -1 || fds[1] == -1 || fds[2] == -1) {
                fputs("Can't open /dev/null\n", stderr);
                for( i = 0; i <= 2; i++)
                        if(fds[i] != -1)
                                close(fds[i]);
                return -1;
        }
        for(i = 0; i <= 2; i++) {
                if(dup2(fds[i], i) == -1) {
                        return -1;
                }
        }
        for(i = 0; i <= 2; i++)
                if(fds[i] > 2)
                        close(fds[i]);

        pid = fork();
        if(pid == -1)
                return -1;
        if(pid)
                exit(0);
        setsid();
        return 0;
}

/**************************************************************************
  check service is already running or not                                 
 **************************************************************************/

int is_eservcln_running()
{
        FILE *fp;
        long pid = -1;
        int i = 0;
        struct stat st;
        if(access(PIDFILE, F_OK) < 0 || stat(PIDFILE, &st) < 0 || !S_ISREG(st.st_mode))
                return -1;

        char line[100], ch;
        bzero(line, 100);
        fp = fopen(PIDFILE, "r");
        if(fp == NULL)
                return -1;

        while(1)
        {
                ch = fgetc(fp);
                if(ch == EOF || strlen(line) == 99)
                        break;
                if(ch >= '0' && ch <= '9')
                        line[i++] = ch;
        }
        fclose(fp);

        if(strlen(line) == 0)
                return -1;
        pid = atol(line);

        return (kill(pid, SIGKILL) == 0) ? 0 : -1;
}

/**************************************************************************
  save procees id in a file                                       
 **************************************************************************/

int savePidFile()
{
        if (access(PIDFILE, F_OK) == 0)
                if (unlink(PIDFILE) < 0)
                        return -1;
        FILE *pf = fopen(PIDFILE, "w");
        if (pf == NULL)
                return -1;
        fprintf(pf, "%u", getpid());
        fclose(pf);
        return 0;
}

/**************************************************************************
  remove proceesid file(/opt/MicroWorld/var/run/eservcln.pid)      
 **************************************************************************/

void removePidFile()
{
        if(access(PIDFILE, F_OK) == 0)
        {
                unlink(PIDFILE);
        }
}

/***************************************************************************
 Realease Dynamic allocted memmory
***************************************************************************/

void releaseDynamicAllocatedMemory(void * mptr)
{
	if(mptr)
	{
		free(mptr);
		mptr = NULL;
	}
}
