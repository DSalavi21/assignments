// All Arithmatic Functions

#include "arithHeader.h"

void usage()
{
	 printf(" Usage :\n\t./arithop [option] value_1 value_2 ...<value_N>\n\t[options] : \n\t'--add' : Addition\n\t'--sub' : Subtraction\n\t'--mul' : Multiplication\n\t'--div' : Division\n\t'--help'\n");

}

int Addition(const int c, char** v)
{
	if( c < 4 )	
	{
		printf("\t error : Incomplete parameters\n");
		return 0;		
	}	
	else
	{
		int flag = 0;
		long result = 0;
		char* ptr = NULL; // used in strtol() 
		long lnum = 0;   // used in strtol()
		errno = 0;

		int i;
		for( i = 2 ; i < c ; ++i )
		{
			    // long int strtol(const char *nptr, char **endptr, int base);
			    lnum = strtol(v[i], &ptr, 10); // strtol() fails in, Overflow(LONG_MAX) and Underflow(LONG_MIN); & set errno to ERANGE
			
				if( ptr == v[i] ) // 
				{	
					printf("\t error : invalid parameters\n");
					flag = 1;
					break;	
				}	
				if( errno == ERANGE ) // strtol() fails		
				{
					flag = 1;
					printf("\t error : OUT OF RANGE\n");
					break;
				}
			
			    result = result + lnum;
	
		}
		if( flag == 1 ) // not a valid decimal input
		{
			return 0;
		}
		printf("\t RESULT : %ld\n",result);
		return 1;
	}

}


int Subtraction(const int c, char** v)
{
	if( c < 4 )	
	{
		printf("\t error : Incomplete parameters\n");
		return 0;		
	}
	if( c > 4 )
	{
		printf("\t error : More than two values\n");
		return 0;
	}	
	else
	{
		int flag = 0;
		long result = 0;
                char *ptr1, *ptr2; // used in strtol() 
                long lnum1, lnum2;   // used in strtol()
                errno = 0;

		int i;

	  		lnum1 = strtol(v[2], &ptr1, 10); // strtol() fails in, Overflow(LONG_MAX) and Underflow(LONG_MIN); & set errno to ERANGE
			lnum2 = strtol(v[3], &ptr2, 10);
                                if( (ptr1 == v[2]) || (ptr2 == v[3]) )  
                                {
                                        printf("\t error : invalid parameters\n");
                                        flag = 1;
                                }
                                else if( errno == ERANGE ) // strtol() fails         
                                {
                                        printf("\t error : OUT OF RANGE\n");
					flag = 1;
                                }
				else
				{
					result = lnum1 - lnum2;
				}

		if( flag == 1 ) // not a valid decimal input
		{
			return 0;
		}
		printf("\t RESULT : %ld\n",result);
		return 1;
	}

}


int Multiplication(const int c, char** v)
{
	if( c < 4 )	
	{
		printf("\t error : Incomplete parameters\n");
		return 0;		
	}	
	else
	{
		int flag = 0;
		long result = 1;
		char* ptr = NULL; // used in strtol() 
		long lnum = 0;   // used in strtol()
		errno = 0;

		int i;
		for( i = 2 ; i < c ; ++i )
		{
			    // long int strtol(const char *nptr, char **endptr, int base);
			    lnum = strtol(v[i], &ptr, 10); // strtol() fails in, Overflow(LONG_MAX) and Underflow(LONG_MIN); & set errno to ERANGE
			
				if( ptr == v[i] ) // 
				{	
					printf("\t error : invalid parameters\n");
					flag = 1;
					break;	
				}	
				if( errno == ERANGE ) // strtol() fails		
				{
					flag = 1;
					printf("\t error : OUT OF RANGE\n");
					break;
				}
			
			    result = result * lnum;
		}

		if( flag == 1 ) // not a valid decimal input
		{
			return 0;
		}
		printf("\t RESULT : %ld\n",result);
		return 1;
	}

}


int Division(const int c, char** v)
{
	if( c < 4 )	
	{
		printf("\t error : Incomplete parameters\n");
                usage();
		return 0;		
	}
	if( c > 4 )
	{
		printf("\t error : More than two values\n");
		return 0;
	}	
	else
	{
		int flag = 0;
		double result = 0;
                char *ptr1, *ptr2; // used in strtol() 
                long lnum1, lnum2;   // used in strtol()
                errno = 0;

		int i;

	  		lnum1 = strtol(v[2], &ptr1, 10); // strtol() fails in, Overflow(LONG_MAX) and Underflow(LONG_MIN); & set errno to ERANGE
			lnum2 = strtol(v[3], &ptr2, 10);
                                if( (ptr1 == v[2]) || (ptr2 == v[3]) )  
                                {
                                        printf("\t error : invalid parameters\n");
                                        flag = 1;
                                }
                                else if( errno == ERANGE ) // strtol() fails         
                                {
                                        printf("\t error : OUT OF RANGE\n");
					flag = 1;
                                }
				else if( lnum2 == 0 )
				{
					printf("\t error : Divide by zero ( Not Possible )\n");
					flag = 1;
				}
				else
				{
					result = (double)lnum1 / (double)lnum2;
				}

		if( flag == 1 ) // not a valid decimal input
		{
			return 0;
		}
		printf("\t RESULT : %lf\n",result);
		return 1;
	}
			
}

//
