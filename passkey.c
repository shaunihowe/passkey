// header file includes
#include <time.h>                       // for time stuff
#include <stdio.h>                      // standard i/o library
#include <stdlib.h>
#include <string.h>                     // output formatting

char pin[32];
char domain[128];
char user[64];
int thisyear;

void passphrase_stretch512(uint8_t* passphrase);
void password_generate(char* pass, uint8_t* seed, int length);

int main(int argc, char *argv[])
{
	uint8_t passphrase[512];
	char pass[64];
	int year;
	sprintf(domain, "(nodomain)");
	sprintf(user, "(nouser)");
	sprintf(pin, "(nopin)");
	thisyear = 2015;

	printf("enter domain: ");
	if (!fgets(domain, 128, stdin)){return 0;}
	domain[strlen(domain)-1] = 0;
	printf("enter user: ");
	if (!fgets(user, 64, stdin)){return 0;}
	user[strlen(user)-1] = 0;
	printf("enter pin: ");
	if (!fgets(pin, 32, stdin)){return 0;}
	pin[strlen(pin)-1] = 0;
	printf("year    password            passphrase\n");
	for (year=thisyear-5;year<=thisyear;year++)
	{
		sprintf((char*)passphrase, "%s%s%s%i", pin, domain, user, year);
		passphrase_stretch512(passphrase);
		
		printf("%i",year);fflush(stdout);
		
		password_generate(pass, passphrase, 8);
		printf("    %s",pass);fflush(stdout);
		password_generate(pass, passphrase, 24);
		printf("            %s\n",pass);
	}
	return 0;
}

void passphrase_stretch512(uint8_t* passphrase)
{
	int a,i,passlen,round;
	uint8_t salt;
	passlen = strlen((char*)passphrase);
	salt = 255;
	for (a=0;a<512;a++)
	{
		i = a % passlen;
		passphrase[a] = passphrase[i];
		salt ^= passphrase[a] + a + i;
	}
	for (round=0;round<1024;round++)
	{
		for (a=0;a<512;a++)
		{
			passphrase[a] ^= salt;
			salt ^= (salt << 1) + passphrase[a] + a + round;
		}
	}
	return;
}

void password_generate(char* pass, uint8_t* seed, int length)
{
	static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static const char symbols[] = "!$%&*?#@";
	int a, i;
	pass[length] = 0;
	i = (length + seed[length]) % 128;
	for (a=0;a<length;a++)
	{
		if (seed[i] % 16 == 0)
			pass[a] = symbols[seed[i+1] % 8];
		else
			pass[a] = alphanum[seed[i+1] % 64];
		i=i+2;
	}
	pass[seed[i] % length] = symbols[seed[i+1] % 8];
	return;
}