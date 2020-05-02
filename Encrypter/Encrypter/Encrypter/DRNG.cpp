#include "DRNG.h"
static int _drng_support = DRNG_SUPPORT_UNKNOWN;


int DRNG::generate32bitRand(UINT32 *buf, int n)
{
	/* --------------------------------------------------------------------
	Generates 32 bit hardware generated random unsigned integers and populates a n sized array. 
	@PARAM UINT32 *buf- Pointer to a UINT32 array to hold the rand values
	@PARAM int n- Number of Bytes
	-----------------------------------------------------------------------
	*/
	UINT32 *temp = new UINT32;
	

	if (!HAVE_RDRAND) return 0;

	for (int i = 0; i < n/4; i++) {
		if (_rdrand32_step(temp)) {
			buf[i] = *temp;
			
		}
		else
		{
			return 0;
		}
	}

	return 1;
}



