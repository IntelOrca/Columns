/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef GAME_DATA_H_
#define GAME_DATA_H_

#include "Board.h"
#include "ResultsScreen.h"

namespace Sexy
{
	//Game modes
	enum GAME_MODE
	{
		MODE_ADVENTURE,
		MODE_QUICKPLAY,
		MODE_SECRET,
	};

	class Buffer;
	struct GameData
	{
		Board	*mPlayer[3];
		Board	*mStdkeys[2];

		GameResults	mResults;

		int mHumanID;
		int mUTF;
		int mTime;
		int mPrestart;
		int mSeed;

		GAME_MODE mMode;
		GSET mSet;
		int mPlayers;
		int mCurrentRound;
		int mStage;
		int mLevel;
		bool mEnding;
		bool mPlaying;
		bool mPaused;

		static GameData *FromBuffer(Buffer *sav);
		void SaveToBuffer(Buffer *sav);
	};
}

#endif