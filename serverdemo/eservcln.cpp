
#include "main.h"
#include "config.h"
#include "eventlist.h"
#include "genral.h"

#define EVENT_CACHE			"/opt/MicroWorld/etc/eservcln.cache"
#define EVENT_CACHE_temp		"/opt/MicroWorld/etc/eservclntemp.cache"
#define DBG_PATH                        "/var/MicroWorld/var/log/eservcln.log"
#define MINBUFF				256
#define MAXBUFF    			4096

using namespace std;

bool 		TermExitSignal 		= false;
bool 		MaxEventLstLengthReach	= false;
static FILE     *dbg                    = NULL;
int             debuglevel              = 1;


//mutex for Linklist lock
pthread_mutex_t mutexLinkedList ;
pthread_mutex_t mutexEventCache = PTHREAD_MUTEX_INITIALIZER;

//Declare front and rear pointers for linked list, available for all threads
static EventNode *eventListFront = NULL, *eventListRear = NULL;
//Declare event list length
static int EventListLength =0;

/********************************************************************************
  set timeout on socket
  Parameters:
  sockDesc :set time out on this socket descriptor
 ********************************************************************************/

int setTimeoutOnSocket( int sockDesc )
{
        if( sockDesc < 0 )
                return -1;

        fd_set set;
        struct timeval timeout;

	/* clear the set */
        FD_ZERO(&set); 
	/* add our file descriptor to the set */
        FD_SET(sockDesc, &set);
        timeout.tv_sec = 30;
        timeout.tv_usec = 0;

        int rv = select(sockDesc + 1, &set, NULL, NULL, &timeout);
        if(rv == -1)
        {
		/* an error accured */
                XXPRINT(1, "Faliled on select(socket). ErrorNo= %d Error= %s\n", errno, strerror(errno) );
                return -1; 
        }
        else if(rv == 0)
        {
		/* a timeout occured */
                XXPRINT(1, "Timeout[30-seconds] while reading response from eserv.\n");
                return 1;
        }
        else
        {
		/* data is available on server*/
                return 0;
        }


        return -1;
}

/********************************************************************************
  write event to socket
  Parameter 	:
  sockfd 	: Write event on this socket
  event 	: event which is sent to eserv
 ********************************************************************************/
int writeToSocket(int sockfd, char *event)
{
	if(event == NULL)
		return -1;
	int status = -1;
	char buffer[2048];
	status = setTimeoutOnSocket(sockfd);
        if( status != 0 )
                return -1;
	memset( buffer, 0, 2048 );
        if(read(sockfd, buffer, 2048) > 0) 
        {
                XXPRINT(4, "Banner received from eserv. Banner=%s\n", buffer);
        }
        else
        {
		XXPRINT(2, "Faliled to read banner from eserv. ErrorNo= %d Error= %s\n", errno, strerror(errno) );
		return -1;
	}
	if(write(sockfd, event, strlen(event)) < 0)
	{
		XXPRINT(2, "error Writing event on primary socket..\n");
		return -1;
	}
	int n =write(sockfd, "\r\n", 2);
	
	if(n>0)
	{
		memset( buffer, 0, 2048 );
		if(read(sockfd, buffer, 2048) > 0)
		{
			if(strncasecmp(buffer, "250 Line Accepted", 17) == 0)
			{
				XXPRINT(4,"Event successfully accepted by the eserv. {Event = %s}\n", event );
			}
		}
		else
		{
			XXPRINT(2,"Event not accepted by eserv. {Event = %s}\n", event);
			return -1;
		}
	}
	

	return 1;

}
/*********************************************************************************
  write all events from Event Link List into a Event_cache file
  Parameter :
  EventCacheFile : event cache file name
 **********************************************************************************/

int writeEventInFile(const char * EventCacheFile)
{
        fstream fileEvent;
        if(EventCacheFile == NULL)
                return -1;

	if(eventListFront == NULL)
        {
                return 0;
        }

        fileEvent.open(EventCacheFile,std::ios::out|std::ios::app);
        if(!fileEvent.is_open())
        {
                return -1;
        }

        while(eventListFront != NULL)
        {
                if(eventListFront->str != NULL)
                {
                        fileEvent<<eventListFront->str<<endl;
			if(DeleteEventFrmListAtFront(&eventListFront, &eventListRear) != 0)
			{
				XXPRINT(2,"Delete Event from List not successed\n");
			}
                }
		else
		{
			if(DeleteEventFrmListAtFront(&eventListFront, &eventListRear) != 0)
                        {
                                XXPRINT(2,"Delete Event from List not successed\n");
                        }

		}

        }
	fileEvent.close();
        return 1;

}

/*************************************************************************************
 Threadhandle function which read events from client socket and process them
 Parameter :
 TData 	   : sructure which contains client sockfd, clientdata,thread success
**************************************************************************************/

void* ThreadHandle(void *TData)
{
	if( TData == NULL )
		return NULL;

	ThreadData *ThreadHandleData = (ThreadData*)TData;
	int sockfd = -1;
	char Hellostring[MINBUFF] = "";
	char hostname[MINBUFF]="";
	char socketData[MAXBUFF]="";
	int ret, writeFileFlag = 0;

	if(ThreadHandleData == NULL)
		return NULL;

	ThreadHandleData->success = true;
	while(!TermExitSignal)
	{
		if((sockfd = ThreadHandleData->sockfd) < 0)
		{
			usleep(200);
			continue;
		}
		//Get host name 
		if(gethostname(hostname, MINBUFF) < 0)
		{
			XXPRINT(1, "error in getting hostname\n");
		}
		sprintf(Hellostring, "Welcome [IP : %s] Server 1.0.0[%s]\r\n", inet_ntoa(ThreadHandleData->client->sin_addr), hostname);

		if((ret = write(sockfd, Hellostring, strlen(Hellostring))) < 0)
		{
			XXPRINT(1, "connect to eserv is not successful\n");
			close(sockfd);
			ThreadHandleData->sockfd = -1;
			sockfd = -1;
			*ThreadHandleData->ac -= 1;
		}
		//Set time out for read client response
		if(setTimeoutOnSocket(sockfd) != 0)
		{
			close(sockfd);
			ThreadHandleData->sockfd = -1;
			*ThreadHandleData->ac -= 1;
		}
		
		bzero(socketData, MAXBUFF);
		int n = read(sockfd, socketData, MAXBUFF);
		if(n > 0)
		{
			if(strlen(socketData) <= 0 )
			{
				write(sockfd, "250 Line Not Accepted.\r\n", 25) ;
				close(sockfd);
	                        ThreadHandleData->sockfd = -1;
        	                *ThreadHandleData->ac -= 1;
				continue;
			}

			if((ret = write(sockfd, "250 Line Accepted.\r\n", 21)) < 0) 
			{
				XXPRINT(2, "Failed to send Acknowledge event accept\n");
			}

			pthread_mutex_lock(&mutexLinkedList);
			if(AddNodeToEventsList(socketData, &eventListFront, &eventListRear) < 0)
			{
				close(sockfd);
				ThreadHandleData->sockfd = -1;
				sockfd = -1;
				*ThreadHandleData->ac -= 1;
				TermExitSignal = true;
				pthread_mutex_unlock(&mutexLinkedList);
				XXPRINT(2, "Error in Add Event in Event list\n");
				continue;	
			}
			else
			{
				EventListLength++;
				if(EventListLength == ThreadHandleData->eventcachesize)
				{
					MaxEventLstLengthReach = true;
	
					pthread_mutex_lock(&mutexEventCache);

					int retValFileWrite = writeEventInFile(EVENT_CACHE);
					if(retValFileWrite < 0)
					{
						XXPRINT(2,"Error opening event cache file. Failed to save received events.\n");
					}
					else if(retValFileWrite == 0)
					{
						XXPRINT(4,"Event list is empty. No need to save cache.\n");
					}
					MaxEventLstLengthReach = false;
					EventListLength = 0 ;
					pthread_mutex_unlock(&mutexEventCache);

				}
			}
			pthread_mutex_unlock(&mutexLinkedList);
		}

		close(sockfd);
		ThreadHandleData->sockfd = -1;
		sockfd = -1;
		*ThreadHandleData->ac -= 1;

	}

	return ThreadHandleData;
}

/********************************************************************************************************************
  Connect to eserv where Events has to be sent 
  if connection is not available than it retry for connection acording to time which is define in conf file
  and awake thread according to awake time interval which is 2 sec
  Parameter 		:
  cdata 		: Data which is required to thread for connecting eserv(eserv ip,portno)
  timeIntervalCount	: Time interval to connect eserv
 *********************************************************************************************************************/

int connectToPrimaryServer(void *cdata, int &timeIntervalCount)
{
	struct sockaddr_in client;
	int awakeThrdIntrvl = 2;
	int errno;

	if(cdata == NULL)
		return -1;

	PrimaryServerData *connData = (PrimaryServerData*)cdata;
	if(connData == NULL)
		return -1;


	int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_desc == -1)
	{
		XXPRINT(1,"socket open error for connecting eserv : %s.\n", strerror(errno));
		return -1;
	}

	bzero(&client,sizeof(client));
	client.sin_family = AF_INET;
	client.sin_addr.s_addr = inet_addr(connData->primaryServIp);
	client.sin_port = htons(connData->primaryServPort);

	if(timeIntervalCount == 0)
	{
		if (connect(socket_desc,(struct sockaddr*)&client, sizeof(client)) < 0)
		{
			if(errno == EAGAIN || errno == ECONNREFUSED)
			{
				timeIntervalCount += awakeThrdIntrvl;
				close(socket_desc);
				sleep(awakeThrdIntrvl);
				return -2;
			}
			XXPRINT(1,"connect error to eserv %s.\n", strerror(errno));

		}
		else
		{
			timeIntervalCount = 0;
			XXPRINT(2,"Connected to eserv.\n");
			return socket_desc;
		}
	}
	else
	{
		timeIntervalCount += awakeThrdIntrvl;
		if(timeIntervalCount == (connData->primaryServConnRetry * 60))
			timeIntervalCount = 0;
		close(socket_desc);
		sleep(awakeThrdIntrvl);
		return -2;
	}
	return -1;
}

/*********************************************************************************
  send Event from Cache file to Primary server
  Parameter 	:
  cdata		:structure which contains time timeInterval,eserv ip,port
  EventCacheFile: event cache file name
 **********************************************************************************/

int sendCacheEventToServer(void *cdata, const char *EventCacheFile)
{
	fstream fileEvent;
	fstream tempfile;
	string event;
	char *event_ctype;
	int sockfd, timeIntervalCount = 0;
	bool sendEventFailed = false;

	if(cdata == NULL)
		return -1;

	if(EventCacheFile == NULL)
		return -1;

	while(!TermExitSignal)
	{

		pthread_mutex_lock(&mutexEventCache);

		if(access(EventCacheFile, R_OK | F_OK) == 0)
		{
			fileEvent.open(EventCacheFile,std::ios::in);
			if(!fileEvent.is_open())
			{
				pthread_mutex_unlock(&mutexEventCache);
				return -1;
			}
			if (fileEvent.peek() == std::ifstream::traits_type::eof())
			{
				fileEvent.close();
				remove(EventCacheFile);
				pthread_mutex_unlock(&mutexEventCache);
				return 0;
			}
			while(!fileEvent.eof() && !TermExitSignal && !MaxEventLstLengthReach)
			{
				sockfd = connectToPrimaryServer(cdata, timeIntervalCount);
				if(sockfd == -1)
				{
					XXPRINT(1, "Error in connection to eserv\n");
					fileEvent.close();
					pthread_mutex_unlock(&mutexEventCache);
					return -1;
				}
				else if(sockfd == -2)
				{
					fileEvent.close();
					pthread_mutex_unlock(&mutexEventCache);
					usleep(10);
					continue;
				}

				getline(fileEvent, event);
				event_ctype = (char*)malloc(event.length()+1);
				if(event_ctype == NULL)
				{
					TermExitSignal = true;
					fileEvent.close();
					pthread_mutex_unlock(&mutexEventCache);
					close(sockfd);
					XXPRINT(1, "Event cache reading event failed,Memory insufficient\n");
					return -1;
				}
				bzero(event_ctype, event.length()+1);
				strncpy(event_ctype, event.c_str(), event.length());

				if(event_ctype && strlen(event_ctype) > 0)
				{
					if(writeToSocket(sockfd, event_ctype) < 0)
					{
						XXPRINT(2, "error sending event to eserv..\n");
						fileEvent.close();
						sendEventFailed = true;
						close(sockfd);
						releaseDynamicAllocatedMemory((void*)event_ctype);
						goto ResendEvent;
					}

				}
                                releaseDynamicAllocatedMemory((void*)event_ctype);
				close(sockfd);
			}
			close(sockfd);
			if(fileEvent.eof())
			{
				fileEvent.close();
				remove(EventCacheFile);
				pthread_mutex_unlock(&mutexEventCache);
				break;
			}

ResendEvent:
			tempfile.open(EVENT_CACHE_temp,std::ios::out);
			if(!tempfile.is_open())
			{
				pthread_mutex_unlock(&mutexEventCache);
				return -1;
			}
			if(sendEventFailed)
				tempfile<<event<<endl;
			while(!fileEvent.eof())
			{
				getline(fileEvent,event);
				tempfile<<event<<endl;
			}
			fileEvent.close();
			tempfile.close();
			remove(EventCacheFile);
			rename(EVENT_CACHE_temp, EventCacheFile);
			pthread_mutex_unlock(&mutexEventCache);
			usleep(50);
		}
	}
	return 0;
}

/*************************************************************************************
  send Events from Event list
Parameter 	:
cdata		: structure which contains eservip, portno,timeinterval
 *************************************************************************************/

int sendEventfromList(void *cdata)
{
	char *event;
	int errno;
	int sockfd, timeIntervalCount = 0;

	if(cdata == NULL)
		return -1;

	if(eventListFront == NULL)
	{
		return 1;
	}
	while(!TermExitSignal)
	{
		sockfd = connectToPrimaryServer(cdata, timeIntervalCount);
		if(sockfd == -1)
		{
			XXPRINT(1, "Error in connection to eserv\n");
			return -1;
		}
		else if(sockfd == -2)
		{
			continue;
		}
		else
			break;
	}	

	pthread_mutex_lock(&mutexLinkedList);

	if(eventListFront->str != NULL)
	{
		if(writeToSocket(sockfd, eventListFront->str) < 0)
		{
			XXPRINT(1, "error send event to eserv\n");
			pthread_mutex_unlock(&mutexLinkedList);
			close(sockfd);
			return -1;
		}
		else
		{
			if(DeleteEventFrmListAtFront(&eventListFront, &eventListRear) != 0)
			{
				XXPRINT(2,"Delete Event from eventlist not successed\n");
			}
			else
				--EventListLength;
		}
	}
	else
	{
		if(DeleteEventFrmListAtFront(&eventListFront, &eventListRear) != 0)
		{
			XXPRINT(2,"Delete Event from eventlist not successed\n");
		}
		else
			--EventListLength;
	}
	pthread_mutex_unlock( &mutexLinkedList );
	close(sockfd);
	return 0;

}
/********************************************************************************
  Thread handle function for reading events from Event Link List	
Parameter	:
cdata		: structure which contains eservip, portno,timeinterval
 ********************************************************************************/


void *HandleQueue(void *cdata)
{
	char *event; 
	int errno;

	if(cdata == NULL)
		return NULL;
	while(!TermExitSignal)
	{
		//check for remaining Events in Event cache file
		if(access(EVENT_CACHE, R_OK|F_OK) == 0)
		{
			int retVal = sendCacheEventToServer(cdata, EVENT_CACHE);
			if(retVal < 0)
			{
				XXPRINT(1,"Error send Event from Event file.\n");
				continue;
			}
		}
		if(TermExitSignal)
			continue;
		//send Events from Event List
		int retVal = sendEventfromList(cdata);
		if(retVal < 0)
		{
			XXPRINT(1,"ERROR Send Event from Event List\n");
		}
		else if(retVal == 1)
			usleep(500);
		else if(retVal == 0)
			usleep(10);
	}
	return 0;	
}

/**************************************************************************
  SIGINT and SIGTERM  signal handling for termination                
 **************************************************************************/

void term_signal_handle(int signum)
{
	TermExitSignal = true;
	XXPRINT(3,"Term signal detected (%d).\n", signum);
}


/**************************************************************************
  eservcln entering function 	                                          
 **************************************************************************/

int main(int argc, char **argv)
{
	struct sockaddr_in serv_addr, cli_addr;
	eservcln_config           ifconf;
	const char      *eservcln_conf;
	int server_sock, portno, sockfd;
	int ac = 0, ind, errno;
	ThreadData *threaddata = NULL;
	pthread_t *threadid =NULL ;
	PrimaryServerData *primaryServData =NULL;
	pthread_t queue_thread;	
	XPRINT_INIT();

	//set resource limit 
	struct rlimit core_limits;
	core_limits.rlim_cur = core_limits.rlim_max = RLIM_INFINITY;
	setrlimit(RLIMIT_CORE, &core_limits);

	//check process should run as root
	if (getuid() != 0)
	{
		XXPRINT(1,"Non root user can not be run the process\n");
		XPRINT_STOP();
		exit(1);	
	}

	//Check another instance is running or not if yes than kill existing process
	if ( is_eservcln_running() == 0 )
	{
		XXPRINT(3, "Another instance of eservcln service already running.process kill that \n");

	}

	//Make daemon process
	XXPRINT(1,"Daemonizing wait...\n");
	if(daemonize() < 0)
	{
		XXPRINT(1,"process not daemonize\n");
		XPRINT_STOP();
		exit(1);
	}

	//Save process id in a file
	if (savePidFile() < 0)
	{
		XXPRINT( 2, "Error saving pid file.\n");
		exit(1);
	}

	//Termination signal caught 
	signal(SIGTERM, term_signal_handle);
	signal(SIGINT, term_signal_handle);
	signal(SIGPIPE, SIG_IGN);

	eservcln_conf = (argc < 2) ? ESERVCLN_CONF : argv[1];	

	if( access(eservcln_conf, R_OK | F_OK) < 0)
	{
		XXPRINT(1, "Access error config file\n");
		XPRINT_STOP();
		removePidFile();
		exit(1);
	}

	if(read_eservcln_config(eservcln_conf, &ifconf) < 0)
	{
		XXPRINT(2,"Cannot read conf file\n");
		XPRINT_STOP();
		removePidFile();
		exit(1);
	}

	debuglevel = ifconf.debugLevel;	

	//open eservcln server socket
	server_sock = socket(AF_INET, SOCK_STREAM, 0);
	if(server_sock == -1)
	{
		XXPRINT(1,"Error in open eservcln socket%s\n",strerror(errno));
		XPRINT_STOP();
		removePidFile();
		exit(1);
	}	

	fcntl(server_sock, F_SETFD, FD_CLOEXEC);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(ifconf.eservclnport);

	int turniton = 1;
	if ( setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &turniton, sizeof(turniton)) < 0 )
	{
		XXPRINT(1,"ERROR setting reuse option:%s\n", strerror(errno));
		XPRINT_STOP();
		removePidFile();
		exit(1);
	}

	//Bind eservcln server socket to tcp connection
	if(bind( server_sock, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
	{
		XXPRINT(1,"Can not bind eservcln socket%s\n",strerror(errno));
		XPRINT_STOP();
		removePidFile();
		exit(1);
	}

	//listen for incoming connection and keep in network Queue
	if( listen(server_sock,500) == -1)
	{
		XXPRINT(1,"Listen error on eservcln: %s.\n", strerror(errno));
		XPRINT_STOP();
		removePidFile();
		exit(1);

	}
	fcntl(server_sock, F_SETFL, O_NONBLOCK);	

	if (pthread_mutex_init(&mutexLinkedList, NULL) != 0)
	{
		XXPRINT(1,"event list locking initilization error: %s.\n", strerror(errno));
		XPRINT_STOP();
		removePidFile();
		exit(1);
	}

	//Allocate memory for thread handle data	
	threaddata = (ThreadData*) malloc(sizeof(ThreadData) * ifconf.instances);
	if(threaddata == NULL)
	{
		XXPRINT(1,"MemAllocErr(malloc). Memory not sufficient.\n");
		XPRINT_STOP();
		removePidFile();
		exit(1);
	}

	//Allocate memory for storing Threadid
	threadid = (pthread_t *) malloc(sizeof(pthread_t) *ifconf.instances);
	if(threadid == NULL)
	{
		XXPRINT(1,"MemAllocErr(malloc). Memory not sufficient.\n");
		XPRINT_STOP();
		removePidFile();
                releaseDynamicAllocatedMemory((void*)threaddata);

		exit(1);
	}

	//Allocte Memory for Primary server connection data
	primaryServData =(PrimaryServerData *)malloc(sizeof(PrimaryServerData));
	if(primaryServData == NULL)
	{
		XXPRINT(1,"MemAllocErr(malloc). Memory not sufficient.\n");
		XPRINT_STOP();
		removePidFile();
                releaseDynamicAllocatedMemory((void*)threaddata);
		releaseDynamicAllocatedMemory((void*)threadid);


		exit(1);

	}

	bzero(primaryServData->primaryServIp, sizeof(primaryServData->primaryServIp));	
	strncpy(primaryServData->primaryServIp, ifconf.primaryServerIP, strlen(ifconf.primaryServerIP));
	primaryServData->primaryServPort = ifconf.primaryServerPort;
	primaryServData->primaryServConnRetry = ifconf.primarySerConRetry ;

	//create threads for process the client request
	for(ind = 0; ind < ifconf.instances ; ind++)  
	{  
		//Initialize Thread Data
		threaddata[ind].threadid = ac;
		threaddata[ind].sockfd = -1;
		threaddata[ind].success = false;
		threaddata[ind].ac = &ac;
		threaddata[ind].eventcachesize = ifconf.eventcachesize;
		threaddata[ind].client = NULL;
		if(pthread_create(&threadid[ind], NULL, ThreadHandle, &threaddata[ind]) < 0)
		{
			TermExitSignal = true;
			break;
		}
	}

	//create thread for reading Events from event queue
	if(pthread_create(&queue_thread, NULL, HandleQueue, (void*)primaryServData) < 0)
	{
		TermExitSignal = true;
	}

	// Give all threads some time to start running
	sleep(1);
	XXPRINT(3,"starting threads to handle connections ok.\n");

	int t = sizeof(cli_addr);
	while(!TermExitSignal)
	{
		//Accept request from network stack
		if ((sockfd = accept(server_sock, (struct sockaddr *)&cli_addr, (socklen_t *)&t)) < 0)
		{
			// accept will return EAGAIN if there is no connection to accept
			if(errno == EAGAIN)    
			{
				usleep(500);   
				continue;
			}
			XXPRINT(2,"Request accept error..: %s.\n", strerror(errno));
			break;
		}
		//Assign accepted request to free thread	
		while(1)
		{
			for(ind = 0; ind < ifconf.instances; ind++)
			{
				XXPRINT(2,"Matching thread instance %d (sockFd = %d, success = %d).\n", ind, threaddata[ind].sockfd, threaddata[ind].success);
				if(threaddata[ind].sockfd < 0 && threaddata[ind].success)
				{
					XXPRINT(3,"Thread instance %d free to process.\n", ind);
					ac++;
					threaddata[ind].sockfd = sockfd;
					threaddata[ind].client = &cli_addr;
					break;
				}
			}
			if(ind < ifconf.instances)
				break;
			XXPRINT(2,"No thread instance free to process, waiting to release one.\n");
			usleep(200);
		}
	}

	//Waiting for all threads finish process
	XXPRINT(3,"Waiting for worker threads to finish...\n");

	for(ind = 0; ind < ifconf.instances; ind++)
	{
		if(threaddata[ind].success)
			pthread_join(threadid[ind], NULL);
	}
	pthread_join(queue_thread, NULL);
	XXPRINT(3, "Threads finish ok.\n");
	//Write remaining events from event Queue in a event cache file 

	int retValFileWrite = writeEventInFile(EVENT_CACHE);
	if(retValFileWrite < 0)
	{
		XXPRINT(1, "Error opening event cache file. Failed to save received events.\n");
	}
	else if(retValFileWrite == 0)
	{
		XXPRINT(3, "Event list is empty. No need to save cache.\n");	
	}

	XXPRINT(1, "eservcln service stopped successfully.\n");
	
	releaseDynamicAllocatedMemory((void*)threaddata);                
	releaseDynamicAllocatedMemory((void*)threadid);
	releaseDynamicAllocatedMemory((void*)primaryServData);

	pthread_mutex_destroy(&mutexLinkedList);
	removePidFile();
	close(server_sock);
	close(sockfd);
	return 0;
}

