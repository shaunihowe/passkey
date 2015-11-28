// header file includes
#include <time.h>                       // for time stuff
#include <stdio.h>                      // standard i/o library
#include <stdlib.h>
#include <string.h>                     // output formatting

char pin[32];
char domain[128];
char user[64];
int thisyear;

static const char alphanum[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
static const char symbols[] = "!$%&*?#@";

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
	int a,passlen;
	unsigned char salt;
	passlen = strlen(passphrase);
	salt = 255;
	for (a=0;a<256;a++)
	{
		if (a >= passlen)
		{
			passphrase[a] = (((passphrase[salt % 8] << 1) ^ (passphrase[salt % 4] >> 1)) ^ salt) + a;
		}
		salt ^= (((salt << 1) ^ (salt >> 1)) ^ passphrase[a]) + a;
	}
	for (a=0;a<256;a++)
	{
		passphrase[a] ^= (((passphrase[salt % 32] << 1) ^ (passphrase[salt % 128] >> 1)) ^ salt) + a;
		salt ^= (((salt << 1) ^ (salt >> 1)) ^ passphrase[a]) + a;
	}
	return;
}

void key_generate4096(unsigned char* key, unsigned char* passphrase)
{
	int a,i,offset,rounds;
	unsigned char salt;
	salt = 255;
	for (a=0;a<512;a++)
	{
		key[a] = (passphrase[a % 256] ^ salt) + a;
		salt ^= (((salt << 1) ^ (salt >> 1)) ^ key[a]) + a;
	}
	for (rounds=0;rounds<32;rounds++)
	{
		offset = (salt % 32);
		key[offset] = passphrase[salt];
		for (a=offset;a<2048;a++)
		{
			i = a % 512;
			key[i]^= (((key[i] << 1) ^ (key[i] >> 1)) ^ salt) + a * i + rounds;
			salt ^= (((salt << 1) ^ (salt >> 1)) ^ key[i]) + a + i * rounds;
		}
	}
    return;
}

void password_generate(unsigned char* pass, unsigned char* key, int length)
{
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
