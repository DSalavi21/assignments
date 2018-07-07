
#include <stdio.h>
#include <string.h>

#include "en_dec_header.h"

int main( int argc, char** argv )
{
	if( argc < 3 )
	{
		printf("Error : Incomplete parameters\n\nuse : \n\t./en_dec_module [OPTION] [KEY]\n\t[OPTIONS] : ENCRYPT / DECRYPT\n\te.g.: ./en_dec_module ENCRYPT PASSWORD\n\t      ./en_dec_module DECRYPT PASSWORD\n");
		return 1;
	}

	if( strcasecmp( argv[1], "ENCRYPT" ) == 0 )
	{
	        char msg[128];
	        memset(msg,0,128);
		
		printf("\nWrite your message    : ");
                scanf(" %[^'\n']s",msg);
                printf("\n");
                if( !encrypt_128bit_block( argv[2], msg ) )
                        printf("\n[Encrypted Successfully] : cat enresult.txt\n");
                else
                        printf("\nError : Encryption Failed !!\n");
	
	}
	else if( strcasecmp( argv[1], "DECRYPT" ) == 0 )
        {
                if( !decrypt_128bit_block( argv[2] ) )
                        printf("\n[Decrypted Successfully] : cat decresult.txt\n");
                else
                        printf("\nError : Decryption Failed !!\n");
	}
	else
	{
		printf("Error : Incorrect parameters\n\nuse : \n\t./en_dec_module [OPTION] [KEY]\n\t[OPTIONS] : ENCRYPT / DECRYPT\n\te.g.: ./en_dec_module ENCRYPT PASSWORD\n\t      ./en_dec_module DECRYPT PASSWORD\n");
                return 1;
	}

	return 0;
}
