/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef RANDOM_H_
#define RANDOM_H_

namespace Sexy
{
	class Random
	{
	public:
		int mLastValue;
		Random(int seed);
		int Next(int low, int high);
	};
}

#endif