/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef HIGHSCORE_H_
#define HIGHSCORE_H_

#include "SexyAppFramework\SexyAppBase.h"
#include "levels.h"

#define MAX_ADVENTURE_SCORES	A_LEVEL_COUNT
#define MAX_QUICKPLAY_SCORES	10
#define MAX_SECRET_SCORES		5

struct Highscore
{
	char *Name;
	int Time;
	int Score;
	int Moves;
};

namespace Sexy
{
	extern Highscore TopAdventureHighscores[MAX_ADVENTURE_SCORES];
	extern Highscore TopQuickplayHighscores[MAX_QUICKPLAY_SCORES];
	extern Highscore TopSecretHighscores[MAX_SECRET_SCORES];

	void AddAdventureHighscore(int level, Highscore hs);
	void AddQuickplayHighscore(Highscore hs);
	void AddSecretHighscore(Highscore hs);
	void LoadHighscores();
	void SaveHighscores();
	void ClearHighscores();
}

#endif