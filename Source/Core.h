/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef CORE_H_
#define CORE_H_

#include "SexyAppFramework/SexyAppBase.h"
#define AIVSAI	0   //1 = AI plays both sides

using namespace Sexy;

struct Size
{
	int width, height;
};

Size NewSize(int w, int h);
char *GetTime(int time);
char *ReadString(Buffer *sav);

#endif