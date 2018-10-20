#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <vector>
#include <exception>


using namespace std;
typedef unsigned int uint;

std::string AVAILABLE_CHARS = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789 ";

char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

static std::string base64_encode(const std::string &in)
{
    std::string out;

    int val=0, valb=-6;
    for (uint jj = 0; jj < in.size(); jj++)
	{
    	//char c = in[jj];
	char c = in[jj];
        val = (val<<8) + c;
        valb += 8;
        while (valb>=0)
		{
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val>>valb)&0x3F]);
            valb-=6;
        }
    }

    if (valb>-6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val<<8)>>(valb+8))&0x3F]);
    while (out.size()%4) out.push_back('=');
    return(out);
}

static std::string base64_decode(const std::string &in)
{
    std::string out;

    std::vector<int> T(256,-1);
    for (int i=0; i<64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i; 

    int val=0, valb=-8;
    for (uint jj = 0; jj < in.size(); jj++)
	{
    	//char c = in[jj];
	char c = in[jj];
        if (T[c] == -1)
			break;

        val = (val<<6) + T[c];
        valb += 6;
        if (valb>=0)
		{
            out.push_back(char((val>>valb)&0xFF));
            valb-=8;
        }
    }

    return(out);
}

int index(char c)
{
	for(uint ii = 0; ii < AVAILABLE_CHARS.size(); ii++)
	{
		if (AVAILABLE_CHARS[ii] == c)
			return(ii);
	}

	return(-1);
}

std::string extend_key(std::string& msg, std::string& key)
{
	//generating new key
	int msgLen = msg.size();
	std::string newKey(msgLen, 'x');
	int keyLen = key.size(), i, j;
    for(i = 0, j = 0; i < msgLen; ++i, ++j)
	{
        if (j == keyLen)
            j = 0;
         newKey[i] = key[j];
    }

    newKey[i] = '\0';
	return(newKey);
}

std::string encrypt_wfx(std::string& msg, std::string& key)
{
	uint msgLen = msg.size(), keyLen = key.size(), i;
 	std::string encryptedMsg(msgLen, 'x');
    // char newKey[msgLen], encryptedMsg[msgLen], decryptedMsg[msgLen];
 
	std::string newKey = extend_key(msg, key);
 
    //encryption
    for(i = 0; i < msgLen; ++i)
	{
    	// std::cout << msg[i] << " " << myisalnum(msg[i]) << std::endl;
	
    	if (isalnum(msg[i]) || msg[i] == ' ')
		{
    		encryptedMsg[i] = AVAILABLE_CHARS[((index(msg[i]) + index(newKey[i])) % AVAILABLE_CHARS.size())];
    	}
		else
		{
    		encryptedMsg[i] = msg[i];
    	}
    }
	
    encryptedMsg[i] = '\0';
    return(encryptedMsg);
}

std::string decrypt_wfx(std::string& encryptedMsg, std::string& newKey)
{
	// decryption
	int msgLen = encryptedMsg.size();
	std::string decryptedMsg(msgLen, 'x');
	int i;
    for(i = 0; i < msgLen; ++i)
	{
    	if (isalnum(encryptedMsg[i]) || encryptedMsg[i] == ' ')
		{
    		decryptedMsg[i] = AVAILABLE_CHARS[(((index(encryptedMsg[i]) - index(newKey[i])) + AVAILABLE_CHARS.size()) % AVAILABLE_CHARS.size())];
    	}
		else
		{
    		decryptedMsg[i] = encryptedMsg[i];
    	}
    }

    decryptedMsg[i] = '\0';
	return(decryptedMsg);
}

std::string encrypt(std::string& msg, std::string& key)
{
	std::string b64_str = base64_encode(msg);
	std::string wfx_msg = encrypt_wfx(b64_str, key);
	// std::cout << wfx_msg << std::endl;
	return(wfx_msg);
}

std::string decrypt(std::string& encrypted_msg, std::string& key)
{
	std::string newKey = extend_key(encrypted_msg, key);
	std::string b64_encoded_str = decrypt_wfx(encrypted_msg, newKey);
	std::string b64_decode_str = base64_decode(b64_encoded_str);
	return(b64_decode_str);
}

//encrypt_flag = 0 for encrypt
//encrypt_flag = 1 for decrypt
void encstrWrap(char *Str, int encrypt_flag, char *Output)
{
    //if (argc != 4) {
    //    std::cout << "Usage: ./a.out message key (0:encrypt|1:decrypt)" << std::endl;
    //    std::cout << "./a.out \"Hello world\" MYPRIVATEKEY 0" << std::endl;
    //    std::cout << "./a.out ttz9JqxZHBClNtu= MYPRIVATEKEY 1" << std::endl;
    //    return -1;
    //}

	if (Output)
		Output[0] = 0;

	if (Str == NULL || Str[0] == 0)
		return;

	static char EncryptionKey1[]="baksiweoguialbbvbblhywraaubnakldsafoiuyyoq";
	static char EncryptionKey2[]="zymabvvuuywpoozcrreiykbbpbaqzweppoidcwxzcw";

	char *ec1 = (char*)EncryptionKey1;
	char *ec2 = (char*)EncryptionKey2;
 	std::string key1 = ec1;
	std::string key2 = ec2;

	int len;
	int j;

 	if (encrypt_flag == 0)
	{
		std::string msg = Str;

		std::string st1 = encrypt(msg, key1);
		std::string st2 = encrypt(st1, key2);
		//std::string st3 = encrypt(st2, key1);
		//std::string st4 = encrypt(st3, key2);

		strcpy(Output, st2.c_str());
		len = strlen(Output);

		for (j = len-1; j >= 0; j--)
		{
			if (Output[j] != '=')
				break;
			Output[j] = '$';
		}

		char *rev = strrev(Output);
		strcpy(Output, rev);
 	    //printf("encrypted output: %s\n", st.c_str());
	}
	else
	{
		len = strlen(Str);
		char *Input = (char*)malloc(len + 1);

		if (Input == NULL)
			return;

		strcpy(Input, Str);

		char *rev = strrev(Input);
		strcpy(Input, rev);

		for (j = len-1; j >= 0; j--)
		{
			if (Input[j] != '$')
				break;
			Input[j] = '=';
		}

		std::string msg = Input;

		//std::string st4 = decrypt(msg, key2);
		//std::string st3 = decrypt(st4, key1);
		std::string st2 = decrypt(msg, key2);
		std::string st1 = decrypt(st2, key1);
		strcpy(Output, st1.c_str());
		free(Input);
 	    //printf("decrypted output: %s\n", st.c_str());
 	}
}

//encrypt_flag = 0 for encrypt
//encrypt_flag = 1 for decrypt
void encstr(char *Str, int encrypt_flag, char *Output)
{
	try
	{
		encstrWrap(Str, encrypt_flag, Output);
	}

	catch(...) // catch all
	{
		//writeLog(NULL, "Exception in encstr()...");
		cout << "Exception in encstr()..." << endl;
	}
}


int main(int argc, char **argv)
{
	if( (argc == 3) && ( strcmp( argv[1],"--encrypt") == 0 || strcmp( argv[1],"--decrypt") == 0 ) )
	{
		int en_flag = 0;
		if( strcmp( argv[1],"--decrypt") == 0 )	
		{
			en_flag = 1;		
		}
		char result[128];
		memset(result,0,sizeof(result));
		
		encstr( argv[2] , en_flag , result );
		cout<<"Result is = " << result << endl;

		FILE * pFile;
		pFile = fopen ("RESULT.txt","w");
		fprintf (pFile, "%s", result);

	}
	else
	{
		cout << "Incorrect arguments..." <<endl;
		return -1;
	}
	return 0;
}



