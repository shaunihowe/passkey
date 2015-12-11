// header file includes
#include <time.h>                       // for time stuff
#include <stdio.h>                      // standard i/o library
#include <stdlib.h>
#include <string.h>                     // output formatting
int thisyear;

void password_generate(char* pass, char* seed, int length);

int main(int argc, char *argv[])
{
	char seed[256];
	char pass[64];
	int year;
	sprintf(seed, "(noseed)");
	thisyear = 2015;

	printf("enter seed: ");
	if (!fgets(seed, 252, stdin)){return 0;}
	domain[strlen(seed)-1] = 0;
	printf("year    password            passphrase\n");
	for (year=thisyear-5;year<=thisyear;year++)
	{
		sprintf(seed, "%s%i", seed, year);
		
		printf("%i",year);fflush(stdout);
		
		password_generate(pass, seed, 8);
		printf("    %s",pass);fflush(stdout);
		password_generate(pass, seed, 24);
		printf("            %s\n",pass);
	}
	return 0;
}

void password_generate(char* pass, char* seed, int length)
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
			salt ^= (salt << 1) + key[a] + a + round;
		}
	}

	// lastly, create password of desired length
	pass[length] = 0;
	i = (length + key[length]) % 8;
	for (a=0;a<length;a++)
	{
		if (key[i] % 16 == 0)
			pass[a] = symbols[key[i+1] % 8];
		else
			pass[a] = alphanum[key[i+1] % 64];
		i=i+2;
	}
	pass[key[i] % length] = symbols[key[i+1] % 8];
	return;
}