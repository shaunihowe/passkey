// header file includes
#include <time.h>                       // for time stuff
#include <stdio.h>                      // standard i/o library
#include <stdlib.h>
#include <string.h>                     // output formatting

unsigned char pin[32];
unsigned char domain[128];
unsigned char user[64];
int thisyear;

void passphrase_stretch256(unsigned char* passphrase);
void key_generate4096(unsigned char* key, unsigned char* passphrase);
void password_generate(unsigned char* pass, unsigned char* key, int length);

int main(int argc, char *argv[])
{
	unsigned char passphrase[256];
	unsigned char key[512];
	unsigned char pass[64];
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
	//printf("domain: |%s|\nuser:   |%s|\npin:    |%s|\n", domain, user, pin);
	//printf("%i, %i, %i\n", strlen(domain), strlen(user), strlen(pin));
	sprintf(passphrase, "%s%s%s%i", pin, domain, user, thisyear);
	printf("%s\n",passphrase);
	printf("year    password            passphrase\n");
	for (year=thisyear-5;year<=thisyear;year++)
	{
		printf("%i",year);
		sprintf(passphrase, "%s%s%s%i", pin, domain, user, year);
		passphrase_stretch256(passphrase);
		key_generate4096(key, passphrase);
		password_generate(pass, key, 8);
		printf("    %s",pass);
		sprintf(passphrase, "%s%s%s%i", pin, domain, user, year);
		passphrase_stretch256(passphrase);
		key_generate4096(key, passphrase);
		password_generate(pass, key, 24);
		printf("            %s\n",pass);
	}
	return 0;
}

void passphrase_stretch256(unsigned char* passphrase)
{
	int a,i,passlen;
	unsigned char salt;
	passlen = strlen(passphrase);
	salt = 255;
	for (a=0;a<256;a++)
	{
		i = a % passlen;
		passphrase[a] = passphrase[i];
		salt ^= passphrase[a] + a + i;
	}
	for (a=0;a<256;a++)
	{
		passphrase[a] ^= salt + a;
		salt ^= passphrase[a] + a;
	}
	return;
}

void key_generate4096(unsigned char* key, unsigned char* passphrase)
{
	int a;
	unsigned char salt;
	salt = 255;
	for (a=0;a<512;a++)
	{
		key[a] = (passphrase[a % 256] ^ salt) + a + 5;
		salt ^=  key[a] + a + 6;
	}
    return;
}

void password_generate(unsigned char* pass, unsigned char* key, int length)
{
	static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	static const char symbols[] = "!$%&*?#@";
	int a, i;
	pass[length] = 0;
	i = (length + key[a]) % 128;
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