#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

void password_generate(char *pass, const char *seed, const int length);

int main(int argc, char *argv[])
{
	int thisyear = 2017;
	char seed[256];
	char pass[64];
	int year;
	char useseed[256];
	sprintf(seed, "(noseed)");

	printf("enter seed: ");
	if (!fgets(seed, 252, stdin)){return 0;}
	seed[strlen(seed)-1] = 0;
	printf("year    password (8)        passphrase (24)\n");
	for (year=thisyear;year>=2015;year--)
	{
		sprintf(useseed, "%s%i", seed, year);
		
		printf("%i",year);fflush(stdout);
		
		password_generate(pass, useseed, 8);
		printf("    %s",pass);fflush(stdout);
		password_generate(pass, useseed, 24);
		printf("            %s\n",pass);
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
	salt = 255;
	
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
