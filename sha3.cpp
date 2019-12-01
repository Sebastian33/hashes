#include"sha3.h"
#include<stdlib.h>
#include<cstring>
#include<iostream>

const int l(6);
const int rnds(12 + 2 * l);

#define THETA {\
	tmpbuf[0] = vectorState[0] ^ vectorState[5] ^ vectorState[10] ^ vectorState[15] ^ vectorState[20]; \
	tmpbuf[1] = vectorState[1] ^ vectorState[6] ^ vectorState[11] ^ vectorState[16] ^ vectorState[21]; \
	tmpbuf[2] = vectorState[2] ^ vectorState[7] ^ vectorState[12] ^ vectorState[17] ^ vectorState[22]; \
	tmpbuf[3] = vectorState[3] ^ vectorState[8] ^ vectorState[13] ^ vectorState[18] ^ vectorState[23]; \
	tmpbuf[4] = vectorState[4] ^ vectorState[9] ^ vectorState[14] ^ vectorState[19] ^ vectorState[24]; \
	tmp = tmpbuf[4] ^ _rotl64(tmpbuf[1], 1);\
	vectorState[0] ^= tmp; \
	vectorState[5] ^= tmp; \
	vectorState[10] ^= tmp; \
	vectorState[15] ^= tmp; \
	vectorState[20] ^= tmp; \
	tmp = tmpbuf[0] ^ _rotl64(tmpbuf[2], 1);\
	vectorState[1] ^= tmp; \
	vectorState[6] ^= tmp; \
	vectorState[11] ^= tmp; \
	vectorState[16] ^= tmp; \
	vectorState[21] ^= tmp; \
	tmp = tmpbuf[1]^_rotl64(tmpbuf[3], 1);\
	vectorState[2] ^= tmp; \
	vectorState[7] ^= tmp; \
	vectorState[12] ^= tmp; \
	vectorState[17] ^= tmp; \
	vectorState[22] ^= tmp; \
	tmp = tmpbuf[2]^_rotl64(tmpbuf[4], 1);\
	vectorState[3] ^= tmp; \
	vectorState[8] ^= tmp; \
	vectorState[13] ^= tmp; \
	vectorState[18] ^= tmp; \
	vectorState[23] ^= tmp; \
	tmp = tmpbuf[3]^_rotl64(tmpbuf[0], 1);\
	vectorState[4] ^= tmp; \
	vectorState[9] ^= tmp; \
	vectorState[14] ^= tmp; \
	vectorState[19] ^= tmp; \
	vectorState[24] ^= tmp; \
}

#define RHO {\
	vectorState[1] = _rotl64(vectorState[1], 1);\
	vectorState[2] = _rotl64(vectorState[2], 62);\
	vectorState[3] = _rotl64(vectorState[3], 28);\
	vectorState[4] = _rotl64(vectorState[4], 27);\
	vectorState[5] = _rotl64(vectorState[5], 36);\
	vectorState[6] = _rotl64(vectorState[6], 44);\
	vectorState[7] = _rotl64(vectorState[7], 6);\
	vectorState[8] = _rotl64(vectorState[8], 55);\
	vectorState[9] = _rotl64(vectorState[9], 20);\
	vectorState[10] = _rotl64(vectorState[10], 3);\
	vectorState[11] = _rotl64(vectorState[11], 10);\
	vectorState[12] = _rotl64(vectorState[12], 43);\
	vectorState[13] = _rotl64(vectorState[13], 25);\
	vectorState[14] = _rotl64(vectorState[14], 39);\
	vectorState[15] = _rotl64(vectorState[15], 41);\
	vectorState[16] = _rotl64(vectorState[16], 45);\
	vectorState[17] = _rotl64(vectorState[17], 15);\
	vectorState[18] = _rotl64(vectorState[18], 21);\
	vectorState[19] = _rotl64(vectorState[19], 8);\
	vectorState[20] = _rotl64(vectorState[20], 18);\
	vectorState[21] = _rotl64(vectorState[21], 2);\
	vectorState[22] = _rotl64(vectorState[22], 61);\
	vectorState[23] = _rotl64(vectorState[23], 56);\
	vectorState[24] = _rotl64(vectorState[24], 14);\
}

#define PI {\
	tmpbuf[0] = vectorState[13];\
	tmpbuf[1] = vectorState[9];\
	tmpbuf[2] = vectorState[21];\
	tmpbuf[3] = vectorState[17];\
	\
	vectorState[13] = vectorState[19];\
	vectorState[9] = vectorState[22];\
	vectorState[21] = vectorState[8];\
	vectorState[17] = vectorState[11];\
	\
	vectorState[19] = vectorState[23];\
	vectorState[22] = vectorState[14];\
	vectorState[8] = vectorState[16];\
	vectorState[11] = vectorState[7];\
	\
	vectorState[23] = vectorState[15];\
	vectorState[14] = vectorState[20];\
	vectorState[16] = vectorState[5];\
	vectorState[7] = vectorState[10];\
	\
	vectorState[15] = vectorState[4];\
	vectorState[20] = vectorState[2];\
	vectorState[5] = vectorState[3];\
	vectorState[10] = vectorState[1];\
	\
	vectorState[4] = vectorState[24];\
	vectorState[2] = vectorState[12];\
	vectorState[3] = vectorState[18];\
	vectorState[1] = vectorState[6];\
	\
	vectorState[24] = tmpbuf[2];\
	vectorState[12] = tmpbuf[0];\
	vectorState[18] = tmpbuf[3];\
	vectorState[6] = tmpbuf[1];\
}

#define CHI {\
	std::memcpy(tmpbuf, vectorState, rcsize<<3);\
	vectorState[0] ^= ~tmpbuf[1] & tmpbuf[2];\
	vectorState[1] ^= ~tmpbuf[2] & tmpbuf[3];\
	vectorState[2] ^= ~tmpbuf[3] & tmpbuf[4];\
	vectorState[3] ^= ~tmpbuf[4] & tmpbuf[0];\
	vectorState[4] ^= ~tmpbuf[0] & tmpbuf[1];\
	\
	std::memcpy(tmpbuf, vectorState + rcsize, rcsize<<3);\
	vectorState[5] ^= ~tmpbuf[1] & tmpbuf[2];\
	vectorState[6] ^= ~tmpbuf[2] & tmpbuf[3];\
	vectorState[7] ^= ~tmpbuf[3] & tmpbuf[4];\
	vectorState[8] ^= ~tmpbuf[4] & tmpbuf[0];\
	vectorState[9] ^= ~tmpbuf[0] & tmpbuf[1];\
	\
	std::memcpy(tmpbuf, vectorState + 2*rcsize, rcsize<<3);\
	vectorState[10] ^= ~tmpbuf[1] & tmpbuf[2];\
	vectorState[11] ^= ~tmpbuf[2] & tmpbuf[3];\
	vectorState[12] ^= ~tmpbuf[3] & tmpbuf[4];\
	vectorState[13] ^= ~tmpbuf[4] & tmpbuf[0];\
	vectorState[14] ^= ~tmpbuf[0] & tmpbuf[1];\
	\
	std::memcpy(tmpbuf, vectorState + 3*rcsize, rcsize<<3);\
	vectorState[15] ^= ~tmpbuf[1] & tmpbuf[2];\
	vectorState[16] ^= ~tmpbuf[2] & tmpbuf[3];\
	vectorState[17] ^= ~tmpbuf[3] & tmpbuf[4];\
	vectorState[18] ^= ~tmpbuf[4] & tmpbuf[0];\
	vectorState[19] ^= ~tmpbuf[0] & tmpbuf[1];\
	\
	std::memcpy(tmpbuf, vectorState + 4*rcsize, rcsize<<3);\
	vectorState[20] ^= ~tmpbuf[1] & tmpbuf[2];\
	vectorState[21] ^= ~tmpbuf[2] & tmpbuf[3];\
	vectorState[22] ^= ~tmpbuf[3] & tmpbuf[4];\
	vectorState[23] ^= ~tmpbuf[4] & tmpbuf[0];\
	vectorState[24] ^= ~tmpbuf[0] & tmpbuf[1];\
}

#define IOTA {\
	rc = R & 1;\
	\
	R <<= 1;\
	tmp = (R >> 8) & 1;\
	R ^= tmp ^ (tmp << 4) ^ (tmp << 5) ^ (tmp << 6);\
	rc |= (R & 1) << 1;\
	\
	R <<= 1;\
	tmp = (R >> 8) & 1;\
	R ^= tmp ^ (tmp << 4) ^ (tmp << 5) ^ (tmp << 6);\
	rc |= (R & 1) << 3;\
	\
	R <<= 1;\
	tmp = (R >> 8) & 1;\
	R ^= tmp ^ (tmp << 4) ^ (tmp << 5) ^ (tmp << 6);\
	rc |= (R & 1) << 7;\
	\
	R <<= 1;\
	tmp = (R >> 8) & 1;\
	R ^= tmp ^ (tmp << 4) ^ (tmp << 5) ^ (tmp << 6);\
	rc |= (R & 1) << 15;\
	\
	R <<= 1;\
	tmp = (R >> 8) & 1;\
	R ^= tmp ^ (tmp << 4) ^ (tmp << 5) ^ (tmp << 6);\
	rc |= (R & 1) << 31;\
	\
	R <<= 1;\
	tmp = (R >> 8) & 1;\
	R ^= tmp ^ (tmp << 4) ^ (tmp << 5) ^ (tmp << 6);\
	rc |= (R & 1) << 63;\
	\
	R <<= 1;\
	tmp = (R >> 8) & 1;\
	R ^= tmp ^ (tmp << 4) ^ (tmp << 5) ^ (tmp << 6);\
	vectorState[0] ^= rc;\
}

void Sha3Engine::keccak()
{
	u64 tmp(0);
	u64 R(1);
	u64 rc(0);
	for (int i = 0; i < rnds; i++)
	{
		THETA;
		RHO;
		PI;
		CHI;
		IOTA;
	}
}

Sha3Engine::Sha3Engine(int c):r(stateSize-c/4), n(0)
{
	memset(byteState, 0, rcsize*rcsize*u64size);
}

void Sha3Engine::absorb(void* buf, unsigned size)
{
	unsigned char *data = static_cast<unsigned char*>(buf);
	while (size > 0)
	{
		if (n == r)
		{
			n = 0;
			keccak();
		}
		byteState[n] ^= *data;
		n++;
		size--;
		data++;
	}
}

void Sha3Engine::getHash(void* buf)
{
	if (n == r)
	{
		keccak();
		n = 0;
	}
	byteState[n] ^= 0x6; // padding
	byteState[r - 1] ^= 0x80;
	keccak();
	memcpy(buf, byteState, (stateSize - r) / 2);
	memset(byteState, 0, stateSize);
	n = 0;
}

void Sha3Engine::output()
{
	for (int i = 0; i < 12; i++)
	{
		for (int j = 0; j < 16; j++)
		{
			std::cout << std::hex << (unsigned)byteState[16 * i + j] << ' ';
		}
		std::cout << std::endl;
	}
	for (int i = 192; i < 200; i++)
	{
		std::cout << std::hex << (unsigned)byteState[i] << ' ';
	}
	std::cout << "\n\n";
}