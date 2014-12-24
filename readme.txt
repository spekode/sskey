Shiren's Skeleton Key:

Typical usage goes something like this.
$ ./sskey -rf test.txt 
6LFJY MTTT JJVYF
PTJND VKG0 0VKS9
ZML4B 3WBM 2RVL9
BL63T P5MN 9TY30

where test.txt contains the request request,
C8FQ3 DVGQ B80LQ
37BJZ KP1S 9Y530
LVT16 9XFX V591N
Q01N0 7MX9 PB320

and 6LFJY... is the revival spell.

WINDOWS USERS:
==============
I have included a precompiled .exe (compiled with MinGW32, tested on Windows 7 64bit)
in the /windows directory. If you put your rescue request in the file 'rescue.txt' and run
'rescue.bat' the result should be your revival spell. (Or you can open a shell as normal.)
MD5 hash:
9b0eaf3141a79af8b89ed7381493a2a3  sskey.exe


/* Notes about password structs:
 * --
 * mtseed:
 * It is the 16 bit seed for mersenne twister used to decode the binary in both
 * cases. The MSB is the lower 8 bits of the sum of all the bytes in the password
 * EXCEPT the MSB itself (checksum.) The LSB doesn't matter except as part of the seed.
 */
struct PWD_rescue {
	uint8_t pwdtype;    // type of pwd (0x21)
	uint16_t mtseed;	// seed for decoding
	uint32_t rescuee;   // ID of rescuee
	uint16_t notsure1;
	uint8_t notsure2;
	uint32_t notsure3;  // included in revival spell, <<8
	uint32_t notsure4;  // field before name (same across two rescue pwds?)
	uint8_t name[10];   // name of rescuee
	uint16_t floor;     // floor of rescue
	uint8_t rngseed[3]; // RNG seed
	uint16_t zero;      // unused pad?
};

struct PWD_revival {
	uint8_t pwdtype;    // type of pwd (0x42)
	uint16_t mtseed;    // seed for decoding, and rngseed[0] from rescue req. (LSB)
	uint32_t rescuee;   // ID of rescuee
	uint16_t notsure1;  // from rescue pwd, notsure1
	uint32_t notsure2;  // from rescue pwd, notsure3
	uint32_t rescuer;   // ID of rescuer
	uint8_t notsure3;   // -- Set both these fields to 00 and the pass
	uint16_t notsure4;  // -- was still accepted.
	uint32_t notsure5;  // Set to 0 and pass was still accepted
	uint8_t name[10];   // rescuer name
	uint8_t zero;       // unused pad?
};


