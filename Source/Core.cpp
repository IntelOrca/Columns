/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include <cstdio>
#include "core.h"

Size NewSize(int w, int h)
{
	Size s;
	s.width = w;
	s.height = h;
	return s;
}

char *GetTime(int time)
{
	if (time < 0)
		time = 0;
	char cTime[10], cMins[4], cSecs[4];
	int mins = time / 60;
	int secs = time % 60;

	//Make sure format is 00:00
	if (secs < 10)
		sprintf(cSecs, "0%d", secs);
	else
		sprintf(cSecs, "%d", secs);
	if (mins < 10)
		sprintf(cMins, "0%d", mins);
	else
		sprintf(cMins, "%d", mins);

	//Draw it
	sprintf(cTime, "%s:%s", cMins, cSecs);
	return cTime;
}

char *ReadString(Buffer *sav)
{
	std::string s = sav->ReadString();
	char *str = new char[s.length()];
	strcpy(str, s.c_str());
	return str;
}