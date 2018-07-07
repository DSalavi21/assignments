/*
# Implementaion of rijndael AES algo for key size with 256-bits(32-bytes)
*/

#include <stdio.h>
#include <string.h>
#include "en_dec_header.h" // declarations of en/decryption routines
#include "aes.h" 	   // AES header file (library from openSSL project)

/*-----------------------------------
        encrypt_128bit_block()

Inputs		:	key, msg 
return value	:	0(success)	 
-----------------------------------*/

int encrypt_128bit_block( char *en_key, char *en_msg )
{
	AES_KEY		aes_key; 	// AES_KEY structure set rounds value on key size (aes.h)
	unsigned char 	key[32];	// user input key 256-bit(32-byte)
	unsigned char 	plaintext[16];
	unsigned char 	ciphertext[16];	
	FILE *enresult = NULL;
	int nrounds = 0, i = 0 ;

	if( strlen(en_key) > 32 )
	{
		printf("Error : key limit exceeds! max key_length(32)\n");
		return 1;
	}

	// copying en_key into key 
	for( i = 0; i < sizeof(key); i++ )
	{
		key[i] = *en_key != 0 ? *en_key++ : 0;
	}

	// file where cipher will stores
	enresult = fopen( "enresult.txt", "wb" );
	if( enresult == NULL )
	{
		printf("ERROR : can't create file\n");
		return 1;
	}

	// expand cipher key into encryption key 
	if( AES_set_encrypt_key( key, 256, &aes_key ) )	
	{
		printf("Error : can't expand key!\n");
		fclose(enresult);
		return 1;
	}

	// encrypting msg and writing cipher into file
	while( *en_msg != '\0' )
	{
		memset(plaintext,0,16);
		memset(ciphertext,0,16);

		// copying msg into 128-bit block to encrypt
		for( i = 0 ; ( i < sizeof(plaintext) ) && ( *en_msg != '\0' ); i++ )
		{
			plaintext[i] = *en_msg++;
		}

		if( plaintext[0] == '\0')
			break;

		// encryption routine
		AES_encrypt( plaintext, ciphertext, &aes_key);

		// write cipher
		if(fwrite(ciphertext, sizeof(ciphertext), 1, enresult) != 1)
		{
			fclose(enresult);
			printf("ERROR : File write error\n");
			return 1;
		}
	}
	fclose(enresult);
	return 0;
}

/*-----------------------------------
	decrypt_128bit_block()

Inputs		:	key 
return value	:	0(success)
-----------------------------------*/

int decrypt_128bit_block( char *dec_key )
{
        AES_KEY 	aes_key; 	// AES_KEY structure set rounds value on key size (aes.h)
        unsigned char   key[32]; 	// user input key 256-bit(32-byte)
        unsigned char 	plaintext[16];
        unsigned char 	ciphertext[16];
        FILE *cipher = NULL, *deresult = NULL; // file pointers to cipher result file and dec_result file
        int i = 0 ;

        if( strlen(dec_key) > 32 )
        {
                printf("Error : key limit exceeds! max key_length(32)\n");
                return 1;
        }

        // copying dec_key into key 
        for( i = 0; i < sizeof(key); i++ )
        {
                key[i] = *dec_key != 0 ? *dec_key++ : 0;
        }

	// open cipher file
        cipher = fopen( "enresult.txt", "rb" );
        if( cipher == NULL )
        {
                printf("ERROR : can't open file\n");
                return 1;
        }
        
	// expand cipher key into decryption key
        if( AES_set_decrypt_key( key, 256, &aes_key ) )
        {
                printf("Error : can't expand key!\n");
                fclose(cipher);
                return 1;
        }

	
	// file where we write decrypt result
	deresult = fopen( "decresult.txt", "wb" );
        if( deresult == NULL )
        {
		fclose(cipher);
                printf("ERROR : can't create file\n");
                return 1;
        }

	// decrypting msg using key and writing result into file
        while( 1 )
        {
                memset(plaintext,0,16);
                memset(ciphertext,0,16);

		// read cipher
		if(fread( ciphertext, sizeof(ciphertext), 1, cipher) != 1 )
			break;

		// decryption routine
		AES_decrypt( ciphertext, plaintext, &aes_key);

		// write result
                if(fwrite(plaintext, sizeof(plaintext), 1, deresult) != 1)
                {
                        printf("ERROR : File write error\n");
			fclose(deresult);
        		fclose(cipher);
                        return 1;
                }
	}

	fclose(deresult);
	fclose(cipher);
	return 0;
}

/*-----------------------------------------
		operation_EXOR()
Input		: key
return value	: null
-----------------------------------------*/

void operation_EXOR( char *KEY  )
{
        unsigned char *sorted = "0123456789ABCDEF0123456789ABCDEF"; // string length 32
        int j = 0;

        for( j = 0 ; j < strlen(KEY) && j < strlen(sorted); j++ )
        {
                KEY[j] = KEY[j] ^ sorted[j] ;
        }
}
