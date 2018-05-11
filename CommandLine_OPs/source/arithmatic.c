// perform arithmatic operations bt cmd line

#include "arithHeader.h"


int main(int argc, char** argv)
{
	//int ret = 0 ;	
	if(argc < 2)
	{
		printf("\t error : Incomplete parameters\n");
                usage();
	}	
	else
	{
		if((strcmp( argv[1] , "--add" ) == 0 ))
		{
			if(Addition( argc, argv ) == 0)
			{
				printf("\t\t-- Enter Valid Inputs --\n");
				usage();
			}		
		}
		else if((strcmp( argv[1] , "--sub" ) == 0 ))
		{
			if(Subtraction( argc, argv ) == 0)
                        {
                                printf("\t\t-- Enter Valid Inputs --\n");
                                usage();
                        }
			
		}
		else if((strcmp( argv[1] , "--mul" ) == 0 ))
		{
			if(Multiplication( argc, argv ) == 0)
                        {
                                printf("\t\t-- Enter Valid Inputs --\n");
                                usage();
                        }

		}
		else if((strcmp( argv[1] , "--div" ) == 0 ))
		{
			if(Division( argc, argv ) == 0)
                        {
                                printf("\t\t-- Enter Valid Inputs --\n");
                                usage();
                        }
			
		}
		else if((strcmp( argv[1] , "--help" ) == 0 ))
		{
			printf("#####################################################################################\n");
			usage();
			printf("#####################################################################################\n");
		}
		else
		{
			printf("\t error : not a valid [option]\n");
			usage();
		}
	}
	return 0;
}
