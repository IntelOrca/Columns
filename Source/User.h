/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef USER_H_
#define USER_H_

#include "SexyAppFramework\SexyAppBase.h"
#include "Highscore.h"
#include "GameData.h"
#include "levels.h"

namespace Sexy
{
	class GameApp;
	class User
	{
	public:
		GameApp*	mApp;

		bool mSuccessfulLoad;
		char mFilename[MAX_PATH];
		char *mName;

		//Saved games
		bool mHasSavedGame[3];
		GameData *mSavedGame[3];

		//Statistics
		long mTime;
		long mMoves;
		long mJewelsCrushed;
		long mHighestQuickplayScore;

		//Highscores
		Highscore mAHighscore[A_LEVEL_COUNT];
		Highscore mQHighscore[5];
		Highscore mSHighscore;

		//Adventure progress
		GameResults mAdventureResults;

		bool mCompletedAdventure;
		char mStage;
		char mLevel;
		char mStagesUnlocked;

		User();
		User(GameApp *theApp, char *name, bool create);
		void New(char *name);
		bool Load(char *filename);
		void Save();
		void Save(char *filename);
		~User();
	};
	User *FindUserFromDirectory(GameApp *mApp, char *name);
}

#endif