#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

void password_generate(char *pass, const char *seed, const int length);

int main(int argc, char *argv[])
{
	char master[64];
	char seed[256];
	char pass[64];

	sprintf(master, "(nomaster)");
	printf("Enter Master Password: ");
	if (!fgets(master, 60, stdin)){return 0;}
	master[strlen(master)-1] = 0;

	sprintf(seed, "(noseed)");
	for (;;)
	{
		printf("> ");
		if (!fgets(seed, 252, stdin)){return 0;}
		seed[strlen(seed)-1] = 0;
		sprintf(seed, "%s%s", seed, master);
		
		printf("Password (8)      Passphrase (24)\n");
		password_generate(pass, seed, 8);
		printf("%s",pass);
		password_generate(pass, seed, 24);
		printf("          %s\n",pass);
		fflush(stdout); // 
	}
	return 0;
}

void password_generate(char *pass, const char *seed, const int length)
{
	static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static const char symbols[] = "!$%&*?#@";
	int a,i,seedlen,round;
	uint8_t salt;
	uint8_t key[256];
	seedlen = strlen(seed);
	salt = 255 - seedlen;
	
	// 1st, stretch out seed to fill key[256]
	for (a=0;a<256;a++)
	{
		i = a % seedlen;
		key[a] = (uint8_t)seed[i];
		salt ^= key[a] + a + i;
	}
	
	// 2nd, obscure, hash and generate keys
	for (round=0;round<1024;round++)
	{
		for (a=0;a<256;a++)
		{
			key[a] ^= salt;
			salt ^= (salt << 1) + key[a] + a + round + length;
		}
	}

	// lastly, create password of desired length
	pass[length] = 0;
	i = (length + key[length]) % 16;
	for (a=0;a<length;a++)
	{
		if (key[i] % 16 == 0)
			pass[a] = symbols[key[i+1] % 8];
		else
			pass[a] = alphanum[key[i+1] % 64];
		i=i+2;
	}
	// ensure an upper case letter
	pass[ key[i] % 2     ] = alphanum[ key[i+1] % 26      ];
	i=i+2;
	// a lower case letter
	pass[(key[i] % 2) + 2] = alphanum[(key[i+1] % 26) + 26];
	i=i+2;
	// a number
	pass[(key[i] % 2) + 4] = alphanum[(key[i+1] % 10) + 52];
	i=i+2;
	// and finally a symbol
	pass[(key[i] % 2) + 6] = symbols [ key[i+1] % 8       ];
	return;
}
