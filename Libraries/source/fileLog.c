/* Functions List*/
// 1. isFileExists()
// 2. writeToLog()

#include "headfileLog.h"

//########################## isFileExists() ##############################
//                                                                      //
//      use : int ret_value = isFileExists( char* filename );	        //
//                                                                      //
//      return values : ( 1 ) : success case                            //
//                      ( 0 ) : Failure case                            //
//                                                                      //
//########################################################################

int isFileExists(const char* fname)
{
        if( access(fname,F_OK) != -1 ) // mode F_OK ; file existance
        {
                return 1;
        }
        else
        {
                return 0;
        }
}

//################################ writeToLog() ##################################
//										//
//	use : void ret_value = writeToLog( const int err_no, const char* msg );	//
//										//
//	return values : ( 1 ) : success case 					//
//		        ( 0 ) : Failure case					//
//										//
//################################################################################

void writeToLog( const int loglevel, const char *msg )
{
	int conf_loglevel = 0;

	char buff[BUFF_SIZE];
	memset(buff,'\0',BUFF_SIZE);

	FILE *fp = NULL;
	
	/* To parse config file, create dictionary structure pointer */
	dictionary *dict_p;	

	/* launch parser */
	dict_p = iniparser_load("/root/programs/assignments/libraries/conf/logLevel.conf");
		
		if ( dict_p != NULL)
		{
			/* Get the string associated to a key, convert to an int [ get setting attribute ] */
			conf_loglevel = iniparser_getint( dict_p, "setting:loglevel", -1); // returns configuration loglevel otherwise -1
		
     			    					
			    if( conf_loglevel >= loglevel )	// Write as per configuration setting **
			    {
				fp = fopen("LOGFILE","a");
				if( fp != NULL )
				{
					//Find Current Date-Time   
                			char buffer[20];
                        		{
                        			time_t timer;
                        			struct tm* tm_info;

                        			time(&timer);
                        			tm_info = localtime(&timer);

                        			strftime(buffer, 20, "%Y-%m-%d %H:%M:%S", tm_info);

                        		}

                        		fprintf(fp,"[ %s ] : %d : %s\n", buffer, loglevel, msg);
                 
                        		fclose(fp);
				}	
						  
			    }		   	
		}	
	/* Free all memory associated to an ini dictionary */
	iniparser_freedict(dict_p);

}

//  



