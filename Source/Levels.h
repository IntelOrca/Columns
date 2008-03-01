/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef LEVELS_H_
#define LEVELS_H_

#include "AI.h"
#include "Res.h"

using namespace Sexy;

//Adventure Levels
struct AdventureLevel
{
	const AICharacter *ai;		//The AI Profile
	int Colours;				//Number of colours
	int MBSpeed;				//Speed of moving block
	int Stage;					//Stage
	int Level;					//Level
	GSET Set;					//Graphics set
};

#define A_LEVEL_PER_STAGE	5
#define A_STAGE_COUNT		4
#define A_LEVEL_COUNT		A_STAGE_COUNT * A_LEVEL_PER_STAGE

const AdventureLevel ALevels[] = {
	{&AIChars[0], 4, 20, 1, 1, GSET_MODERN},
	{&AIChars[1], 4, 20, 1, 2, GSET_MODERN},
	{&AIChars[2], 4, 20, 1, 3, GSET_MODERN},
	{&AIChars[3], 5, 20, 1, 4, GSET_MODERN},
	{&AIChars[4], 5, 15, 1, 5, GSET_MODERN},
	{&AIChars[5], 5, 15, 2, 1, GSET_CLASSIC},
	{&AIChars[6], 5, 15, 2, 2, GSET_CLASSIC},
	{&AIChars[7], 5, 15, 2, 3, GSET_CLASSIC},
	{&AIChars[8], 6, 12, 2, 4, GSET_CLASSIC},
	{&AIChars[8], 6, 12, 2, 5, GSET_CLASSIC},
	{&AIChars[8], 6, 12, 3, 1, GSET_OTHER1},
};

//Quickplay Levels
#if 0 == 0
	#define LEVELCOUNT	22
	struct {int score; int speed; bool special; int colours;} const SPLevels[] = {
		{0, 0, false, 0},			//00
		{25, 60, false, 4},			//01
		{50, 60, false, 4},			//02
		{100, 55, false, 4},		//03
		{150, 50, false, 5},		//04
		{200, 40, false, 5},		//05
		{300, 30, false, 5},		//06
		{400, 35, false, 5},		//07
		{600, 20, true, 6},			//08
		{800, 25, false, 6},		//09
		{1000, 20, false, 6},		//10
		{1400, 15, false, 6},		//11
		{1800, 20, false, 6},		//12
		{2400, 10, false, 6},		//13
		{2600,  7, true, 6},		//14
		{3600, 10, false, 6},		//15
		{3800,  6, false, 6},		//16
		{4500, 10, false, 6},		//17
		{4700,  8, false, 6},		//18
		{5000,  6, true, 6},		//19
		{5500, 15, false, 6},		//20
		{5600,  8, true, 6},		//21
	};
#else
	#define LEVELCOUNT	21
	struct {int score; int speed; bool special; int colours;} const SPLevels[] = {
		{0, 60, false, 4},			//01
		{5, 60, false, 4},			//02
		{10, 55, false, 4},		//03
		{15, 50, false, 5},		//04
		{20, 40, false, 5},		//05
		{30, 30, false, 5},		//06
		{40, 35, false, 5},		//07
		{60, 20, true, 6},			//08
		{80, 25, false, 6},		//09
		{100, 20, false, 6},		//10
		{140, 15, false, 6},		//11
		{180, 20, false, 6},		//12
		{240, 10, false, 6},		//13
		{300,  7, true, 6},		//14
		{400, 10, false, 6},		//15
		{500,  6, false, 6},		//16
		{600, 10, false, 6},		//17
		{700,  8, false, 6},		//18
		{800,  6, true, 6},		//19
		{900, 15, false, 6},		//20
		{1000,  7, false, 6},		//21
	};
#endif

#endif