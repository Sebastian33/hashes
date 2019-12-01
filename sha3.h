#pragma once
/* according to FIPS 202*/

#ifndef SHA3
#define SHA3
using u64 = unsigned long long;
const int u64size(sizeof(u64));
const int rcsize(5); // rows and columns size
const int stateSize(rcsize*rcsize*u64size);

class Sha3Engine
{
private:
	union
	{
		u64 vectorState[rcsize*rcsize];
		unsigned char byteState[stateSize];
	};
	u64 tmpbuf[rcsize];
	int r;
	int n;
	
	void keccak();
	void output();

public:
	/*c in {224, 256, 384, 512} */
	Sha3Engine(int c);
	void absorb(void *buf, unsigned size); //byte-wise
	void getHash(void *buf);
};

#endif //SHA3