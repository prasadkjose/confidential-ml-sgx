#pragma once
#include <windows.h>
#include <immintrin.h>


#define DRNG_SUPPORT_UNKNOWN	-1
#define DRNG_SUPPORT_NONE		0
#define DRNG_SUPPORT_RDRAND		0x01
#define DRNG_SUPPORT_RDSEED		0x02

#define HAVE_RDRAND ((_drng_support & DRNG_SUPPORT_RDRAND)==DRNG_SUPPORT_RDRAND)
#define HAVE_RDSEED ((_drng_support & DRNG_SUPPORT_RDSEED)==DRNG_SUPPORT_RDSEED)


class DRNG
{
public:
	int generate32bitRand(UINT32 *, int);
};