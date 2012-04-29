/* Shiren's Skeleton Key
 * ---------------------
 * Password encoder/decoder/modifier/generator for Shiren The Wanderer (1) on 
 * Nintendo DS.
 *
 * -- nko, March 11 2010
 */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <ctype.h>
#include "base32.h"
#include "mt.h"

#define ASCII_LENGTH 56
#define BINARY_LENGTH 35

// Table used to swap/unswap bytes of password
const unsigned char TABLE_215CD88[] =  {
    0x06, 0x20, 0x15, 0x03, 0x0E, 0x1C, 0x00, 0x11, \
    0x07, 0x19, 0x0B, 0x13, 0x09, 0x1F, 0x01, 0x0C, \
    0x1D, 0x05, 0x0F, 0x14, 0x02, 0x21, 0x1B, 0x04, \
    0x18, 0x10, 0x16, 0x08, 0x12, 0x17, 0x0A, 0x1E, \
    0x1A, 0x0D, 0x00 \
};

//
// Calculate simple sum checksum
unsigned char checksum(unsigned char *buf)
{
	int i;
	unsigned int sum = 0;

	for (i = 0; i < BINARY_LENGTH-1; i++)
		sum += buf[i];
	sum -= buf[2];

	return (unsigned char)sum;
}

//
// Seed the RNG (Mersenne Twister)
void seed_rng(unsigned char *buf)
{
	unsigned long mtseed = 0;
	mtseed = (buf[2]<<8) + buf[1];
	init_genrand(mtseed);
}

//
// Swap bytes around according to TABLE_215CD88 and
// store in 'dst', unswap when dir!=0
void swap(unsigned char *dst, unsigned char *src, int dir)
{
	int i;
	if (!dir)
		for (i = 0; i < 34; i++)
			dst[i] = src[TABLE_215CD88[i]];
	else
		for (i = 0; i < 34; i++)
			dst[TABLE_215CD88[i]] = src[i];
}

//
// "Encrypt" a binary password
void encrypt(unsigned char *dst, unsigned char *src)
{
	int i = 0;
	int tmp = 0;
	unsigned char buf[BINARY_LENGTH];

	memcpy(buf, src, BINARY_LENGTH);

	// seed RNG
	seed_rng(buf);

	// encrypt
	for (i = 3; i < 34; i++) {
		tmp = genrand_int32() & 0xFF; // take LSB of a random number
		tmp += buf[i];                // add random byte to the byte to encode
		buf[i] = (unsigned char)tmp;  // and store that back into array
	}

	// swap bytes around
	swap(dst, buf, 0);
}

//
// "Decrypt" a binary password
int decrypt(unsigned char *dst, unsigned char *src)
{
	int i = 0;
	unsigned int tmp = 0;

	// swap bytes around
	swap(dst, src, 1);

	// seed RNG
	seed_rng(dst);

	// decrypt
	for (i = 3; i < 34; i++) {
		tmp = genrand_int32() & 0xFF; // take LSB of a random number
		tmp = dst[i] - tmp;           // subtract random byte from byte to decode
		dst[i] = (char)tmp;           // and store that back into array
	}

	if (checksum(dst) == dst[2])
		return 0;

	return 1;
}

/** Stripped
char bincode[] = {
0x42, 0x00, 0xef, 0x00, 0x09, 0xbf, 0x12, 0x34,
0x56, 0x65, 0x14, 0xc0, 0x04, 0x00, 0x09, 0xbf,
0xa5, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
0x4e, 0x05, 0x05, 0x0b, 0x00, 0x00, 0x00, 0x00,
0x3c, 0x00, 0x00
};
*/

int read_bin(FILE *fp, unsigned char *buf)
{
	int count = 0;
	unsigned char val = 0;

	while (count < BINARY_LENGTH) {
		if (fscanf(fp, "%hhx", &val) != 1)
			break;
		buf[count] = val;	
		count += 1;
	}

	return count;
}

int read_ascii(FILE *fp, unsigned char *buf)
{
	int count = 0;
	int val = 0;

	while (count < ASCII_LENGTH) {
		val = fgetc(fp);
		if (val == EOF)
			break;
		if (val == '\r' || val == '\n' || val == ' ')
			continue;
		else
		if (strchr("BCDFGHJKLMNPQRSTVWXYZ1234567890!", val)) {
			buf[count] = (char)val;
			count++;
		}
		else {
			fprintf(stderr, "Invalid character in password!\n");
			break;
		}
	}

	return count;
}

void revive(unsigned char *rev, unsigned char *buf)
{
	unsigned char spell[BINARY_LENGTH];
	int i;

	memset(spell, 0, BINARY_LENGTH);

	// Password type (revival spell)
	spell[0] = 0x42;
	// Rescuee ID, +2 bytes after
	for (i = 3; i < 9; i++) spell[i] = buf[i];	
	// ??
	for (i = 10; i < 14; i++) spell[i-1] = buf[i];
	// Rescuer game ID
	spell[13] = 0x00; spell[14] = 0x09; spell[15] = 0xbf; spell[16] = 0xa5;
	// Rescuer name
	spell[24] = 0x5b; spell[25] = 0x4e; spell[26] = 0x05; spell[27] = 0x05;
	spell[28] = 0x0b; spell[29] = 0x5c;

	spell[2] = checksum(spell);

	memcpy(rev, spell, BINARY_LENGTH);
}

void print_hex(unsigned char *hex)
{
	int i;

	for (i = 0; i < BINARY_LENGTH; i++) {
		if (i && !(i%8)) printf("\n");
		printf("0x%02x ", hex[i]);
	}
	printf("\n");
}

void print_password(unsigned char *pass)
{
	int i;

	for (i = 0; i < 56; i++) {
		if (i && !(i%14)) printf("\n");
		if (i == 5 || i == 9) printf(" ");
		if (i == 19 || i == 23) printf(" ");
		if (i == 33 || i == 37) printf(" ");
		if (i == 47 || i == 51) printf(" ");
		printf("%c", pass[i]);
	}
	printf("\n");
}

int main(int argc, char *argv[])
{
	unsigned char binbuf[BINARY_LENGTH];
	unsigned char e_binbuf[BINARY_LENGTH];
	unsigned char asciibuf[ASCII_LENGTH];
	unsigned char outbuf[ASCII_LENGTH];
	unsigned char verifybuf[ASCII_LENGTH];
	int c;
	FILE *fp;
	char *filename = NULL;
	enum { BINARY = 0, ASCII } mode = ASCII;
	int nosum = 0;
	int output_hex = 0;
	int revival = 0;
	int verify = 0;

	//
	// Parse cmdline
	if (argc < 2) {
		/* Print some help */
		fprintf(stderr, "Shiren's Skeleton Key::::\n" \
		               "\t-, read password from stdin (default)\n" \
					   "\t-b, read password in hexidecimal\n" \
					   "\t-f <file>, read password from <file>\n" \
					   "\t-n, don't verify checksum of input\n" \
					   "\t-r, attempt to make revival spell\n" \
					   "\t-v, verify output\n" \
					   "\t-x, output hexidecimal\n");
		return 1;
	}

	opterr = 0;

	while ((c = getopt(argc, argv, "bf:nrvx")) != -1) {
		switch (c) {
			case 'b':
				mode = BINARY;
				break;
			case 'f':
				filename = optarg;	
				break;
			case 'n':
				nosum = 1;
				break;
			case 'r':
				revival = 1;
				break;
			case 'v':
				verify = 1;
				break;
			case 'x':
				output_hex = 1;
				break;
			case '?':
				switch (optopt) {
					case 'f':
						fprintf(stderr, "Option '-%c' requires an argument.\n", optopt);
						break;
					default:
						fprintf(stderr, "Invalid option");
						if (isprint(c)) fprintf(stderr, ": '-%c'\n", (char)optopt);
						else fprintf(stderr, "\n");
				}
				return 1;
			default:
				abort();
		}
	}

	//
	// Clear buffers
	memset(binbuf, 0, BINARY_LENGTH);
	memset(e_binbuf, 0, BINARY_LENGTH);
	memset(asciibuf, 0, ASCII_LENGTH);
	memset(outbuf, 0, ASCII_LENGTH);
	memset(verifybuf, 0, ASCII_LENGTH);

	//
	// Get password data 
	if (filename != NULL) {
		fp = fopen(filename, "r");
		if (fp == NULL) {
			fprintf(stderr, "Couldn't open file!\n");
			return 1;
		}
	}
	else fp = stdin;

	if (mode == BINARY) {
		if (read_bin(fp, binbuf) != BINARY_LENGTH) {
			fprintf(stderr, "Failed to read binary data!\n");
			// Just letting the OS close the file, thanks.
			return 1;
		}
	}
	else
	if (mode == ASCII) {
		if (read_ascii(fp, asciibuf) != ASCII_LENGTH) {
			fprintf(stderr, "Failed to read ASCII data!\n");
			// Same as above, ty.
			return 1;
		}
		base32_decode((char *)e_binbuf, 35, asciibuf, 56);
		decrypt(binbuf, e_binbuf);
	}

	if (filename != NULL) fclose(fp);

	//
	// Check binary checksum?
	if (!nosum) {
		if (binbuf[2] != checksum(binbuf)) {
			fprintf(stderr, "Bad checksum! [is:%x/shouldbe:%x]\n",
			        binbuf[2], checksum(binbuf));
			return 1;
		}
	}

	//
	// Change to revival?
	if (revival) {
		if (binbuf[0] != 0x21) {
			// Not a rescue request
			fprintf(stderr, "This is not a rescue request!\n");
			return 1;
		}
		revive(binbuf, binbuf);
	}

	//
	// Encrypt
	encrypt(e_binbuf, binbuf);

	//
	// Encode
	base32_encode((char*)outbuf, 56, e_binbuf, 35);

	//
	// Verify?
	if (verify) {
		fprintf(stderr, "VERIFICATION: ");
		base32_decode((char*)e_binbuf, 35, verifybuf, 56);
		if (decrypt(verifybuf, e_binbuf)) {
			fprintf(stderr, "VERIFICATION FAILED\n");
			return 1;
		}
		else fprintf(stderr, "VERIFIED!\n");
	} // verify

	//
	// Display
	printf("\n");
	if (output_hex)
		print_hex(binbuf);
	else
		print_password(outbuf);

	return 0;
}

