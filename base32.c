/*
 * $Id: base32.c 15781 2008-09-02 16:30:18Z rmanfredi $
 *
 * Copyright (c) 2006, Christian Biere
 *
 *----------------------------------------------------------------------
 * This file is part of gtk-gnutella.
 *
 *	gtk-gnutella is free software; you can redistribute it and/or modify
 *	it under the terms of the GNU General Public License as published by
 *	the Free Software Foundation; either version 2 of the License, or
 *	(at your option) any later version.
 *
 *	gtk-gnutella is distributed in the hope that it will be useful,
 *	but WITHOUT ANY WARRANTY; without even the implied warranty of
 *	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *	GNU General Public License for more details.
 *
 *	You should have received a copy of the GNU General Public License
 *	along with gtk-gnutella; if not, write to the Free Software
 *	Foundation, Inc.:
 *		59 Temple Place, Suite 330, Boston, MA	02111-1307	USA
 *----------------------------------------------------------------------
 */

/**
 * @ingroup lib
 * @file
 *
 * Base32 encoding/decoding.
 *
 * @author Christian Biere
 * @date 2006
 */

/*
 * Copyright (c) 2006 Christian Biere <christianbiere@gmx.de>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *	notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *	notice, this list of conditions and the following disclaimer in the
 *	documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the authors nor the names of its contributors
 *	may be used to endorse or promote products derived from this software
 *	without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHORS AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.	IN NO EVENT SHALL THE REGENTS OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <string.h>
#include <unistd.h>
#include <stdint.h>
#include <ctype.h>
#include "base32.h"

/*
 * See RFC 3548 for details about Base 32 encoding:
 *	http://www.faqs.org/rfcs/rfc3548.html
 */

/* Shiren DS encoding */
static const char base32_alphabet[32] = {
	'0', '1', '2', '3', '4', '5', '6', '7',
	'8', '9', 'B', 'C', 'D', 'F', 'G', 'H',
	'J', 'K', 'L', 'M', 'N', 'P', 'Q', 'R',
	'S', 'T', 'V', 'W', 'X', 'Y', 'Z', '!'
};

/**
 * Encode in base32 `len' bytes of `data' into the buffer `dst'.
 *
 * @param dst		destination buffer
 * @param size		length of destination
 * @param data		start of data to encode
 * @param len		amount of bytes to encode
 *
 * @return the amount of bytes generated into the destination.
 */
int
base32_encode(char *dst, size_t size, const void *data, size_t len)
{
	size_t i = 0;
	const uint8_t *p = data;
	const char *end = &dst[size];
	char *q = dst;

	do {
		size_t j, k;
		uint8_t x[5];
		char s[8];

		switch (len - i) {
		case 4:
			k = 7;
			break;
		case 3:
			k = 5;
			break;
		case 2:
			k = 3;
			break;
		case 1:
			k = 2;
			break;
		default:
			k = 8;
		}

		for (j = 0; j < 5; j++)
			x[j] = i < len ? p[i++] : 0;

		/*
		  +-------+-----------+--------+
		  | target| source	  | source |
		  | byte  | bits	  | byte   |
		  +-------+-----------+--------+
		  |		0 | 7 6 5 4 3 | 0	   |
		  |		1 | 2 1 0 7 6 | 0-1	   |
		  |		2 | 5 4 3 2 1 | 1	   |
		  |		3 | 0 7 6 5 4 | 1-2	   |
		  |		4 | 3 2 1 0 7 | 2-3	   |
		  |		5 | 6 5 4 3 2 | 3	   |
		  |		6 | 1 0 7 6 5 | 3-4	   |
		  |		7 | 4 3 2 1 0 | 4	   |
		  +-------+-----------+--------+
		*/
/****** Shiren changes the order around
		s[0] = (x[0] >> 3);
		s[1] = ((x[0] & 0x07) << 2) | (x[1] >> 6);
		s[2] = (x[1] >> 1) & 0x1f;
		s[3] = ((x[1] & 0x01) << 4) | (x[2] >> 4);
		s[4] = ((x[2] & 0x0f) << 1) | (x[3] >> 7);
		s[5] = (x[3] >> 2) & 0x1f;
		s[6] = ((x[3] & 0x03) << 3) | (x[4] >> 5);
		s[7] = x[4] & 0x1f;
********/
        s[0] = (x[0] & 0x1F);
        s[1] = ((x[0] & 0xE0) >> 5) | ((x[1] & 0x03) << 3);
        s[2] = (x[1] & 0x7C) >> 2;
        s[3] = ((x[1] & 0x80) >> 7) | ((x[2] & 0x0F) << 1);
        s[4] = ((x[2] & 0xF0) >> 4) | ((x[3] & 0x01) << 4);
        s[5] = ((x[3] & 0x3E) >> 1);
        s[6] = ((x[3] & 0xC0) >> 6) | ((x[4] & 0x07) << 2);
        s[7] = (x[4] & 0xF8) >> 3;

		for (j = 0; j < k && q != end; j++) {
			*q++ = base32_alphabet[(uint8_t) s[j]];
		}

		if (end == q) {
			break;
		}

	} while (i < len);

	return q - dst;
}

static char base32_map[(unsigned char) -1];

/**
 * Decode a base32 encoding of `len' bytes of `data' into the buffer `dst'.
 *
 * @param dst		destination buffer
 * @param size		length of destination
 * @param data		start of data to decode
 * @param len		amount of encoded data to decode
 *
 * @return the amount of bytes decoded into the destination.
 */
int
base32_decode(char *dst, size_t size, const void *data, size_t len)
{
	const char *end = &dst[size];
	const unsigned char *p = data;
	char s[8];
	char *q = dst;
	int pad = 0;
	size_t i, si;

	if (0 == base32_map[0]) {
		for (i = 0; i < G_N_ELEMENTS(base32_map); i++) {
			const char *x;

			x = memchr(base32_alphabet, toupper(i),
					   sizeof base32_alphabet);
			base32_map[i] = x ? (x - base32_alphabet) : (unsigned char) -1;
		}
	}

	memset(&s[0], 0, sizeof s);
	si = 0;
	i = 0;

	while (i < len) {
		unsigned char c;

		c = p[i++];
		if ('=' == c) {
			pad++;
			c = 0;
		} else {
			c = base32_map[c];
			if ((unsigned char) -1 == c) {
				return -1;
			}
		}

		s[si++] = c;

		if (G_N_ELEMENTS(s) == si || pad > 0 || i == len) {
			char b[5];
			size_t bi;

			memset(&s[si], 0, G_N_ELEMENTS(s) - si);
			si = 0;
/***** Shiren reverses this, starts at the LSB first
			b[0] =
				((s[0] << 3) & 0xf8) |
				((s[1] >> 2) & 0x07);
			b[1] =
				((s[1] & 0x03) << 6) |
				((s[2] & 0x1f) << 1) |
				((s[3] >> 4) & 1);
			b[2] =
				((s[3] & 0x0f) << 4) |
				((s[4] >> 1) & 0x0f);
			b[3] =
				((s[4] & 1) << 7) |
				((s[5] & 0x1f) << 2) |
				((s[6] >> 3) & 0x03);
			b[4] =
				((s[6] & 0x07) << 5) |
				(s[7] & 0x1f);
******/
            b[0] = 
                (s[0] & 0x1F) |
                ((s[1] & 0x07) << 5);
            b[1] =
                ((s[1] & 0x18) >> 3) |
                ((s[2] & 0x1F) << 2) | 
                ((s[3] & 0x01) << 7);
            b[2] =
                ((s[3] & 0x1E) >> 1) |
                ((s[4] & 0x0F) << 4);
            b[3] =
                ((s[4] & 0x10) >> 4) |
                ((s[5] & 0x1F) << 1) |
                ((s[6] & 0x03) << 6);
            b[4] = 
                ((s[6] & 0x1C) >> 2) |
                ((s[7] & 0x1F) << 3);


			for (bi = 0; bi < G_N_ELEMENTS(b) && q != end; bi++) {
				*q++ = b[bi];
			}
		}

		if (end == q) {
			break;
		}
	}

	return q - dst;
}

/* vi: set ai et ts=4 sw=4 cindent: */
