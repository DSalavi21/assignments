#include "myhead.h"

int main(int argc, char **argv)
{

    if( argc < 2 )
    {
                printf("\n error : Incomplete parameters\n");
                printf("   Usage :\n\t./build [options] \n\ttry     './build -v' \n\t\t'./build -V'\n\t\t'./build --version'\n");
    }
    else if( argc == 2 )
    {
        if( (strcmp( argv[1] , "-v" ) == 0 ) || (strcmp( argv[1] , "--version" ) == 0 ) || (strcmp( argv[1] , "-V" ) == 0 )  )
        {
                #ifdef MYVER
                        printf("Build : %.1f \n", MYVER);
                #endif
        }
        else
        {
                printf("\nerror : Invalid option\n");
                printf("   Usage :\n\t./build [options] \n\ttry     './build -v' \n\t\t'./build -V'\n\t\t'./build --version'\n");
        }

    }
    else
    {
                printf("\nerror : Invalid option\n");
                printf("   Usage :\n\t./build [options] \n\ttry     './build -v' \n\t\t'./build -V'\n\t\t'./build --version'\n");
    }

 return 0;
}
