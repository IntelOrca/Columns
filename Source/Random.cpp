/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "random.h"
#include <cstdlib>

using namespace Sexy;

Random::Random(int seed)
{
	mLastValue = seed;
}

int RandomValue(int lastvalue)
{
	srand(lastvalue);
	return rand();
}

int Random::Next(int low, int high)
{
	if (high - low == 0) return low;
	int range = (high - low) + 1; 
	int random = RAND_MAX;
	mLastValue = RandomValue(mLastValue);
	random = ((double) mLastValue / (double) RAND_MAX) * range + low;
	return random;
}