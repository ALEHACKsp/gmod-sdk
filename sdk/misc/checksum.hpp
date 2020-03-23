#pragma once
#include "../../includes.hpp"

#define Q_binarytohex	V_binarytohex
#define MD5_DIGEST_LENGTH 16  
#define MD5_BIT_LENGTH ( MD5_DIGEST_LENGTH * sizeof(unsigned char) )
#define COPY_ALL_CHARACTERS -1

struct MD5Value_t
{
	unsigned char bits[MD5_DIGEST_LENGTH];

	void Zero();
	bool IsZero() const;

	bool operator==(const MD5Value_t& src) const;
	bool operator!=(const MD5Value_t& src) const;

};
typedef struct
{
	unsigned int	buf[4];
	unsigned int	bits[2];
	unsigned char	in[64];
} MD5Context_t;
// The four core functions - F1 is optimized somewhat
// #define F1(x, y, z) (x & y | ~x & z)
#define F1(x, y, z) (z ^ (x & (y ^ z)))
#define F2(x, y, z) F1(z, x, y)
#define F3(x, y, z) (x ^ y ^ z)
#define F4(x, y, z) (y ^ (x | ~z))

// This is the central step in the MD5 algorithm.
#define MD5STEP(f, w, x, y, z, data, s) \
        ( w += f(x, y, z) + data,  w = w<<s | w>>(32-s),  w += x )

unsigned int MD5_PseudoRandom(unsigned int nSeed);