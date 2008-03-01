/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef GAMEAPP_H_
#define GAMEAPP_H_

#include "SexyAppFramework/SexyAppBase.h"

namespace Sexy
{
	enum GAME_MODE;
	enum GSET;
	class TitleScreen;
	class Menu;
	class User;
	class UserDialog;
	class Game;
	class ResultsScreen;
	class HighscoreScreen;

	struct GameResults;
	class GameApp : public SexyAppBase
	{
	private:
		TitleScreen	*mTitleScreen;
		Menu *mMenu;
		Game *mGame;
		ResultsScreen *mResults;
		HighscoreScreen *mHighscores;

		SexyString mLastUser;
		bool mLostFocus;

	public:
		User *mCurrentUser;
		UserDialog *mUserDialog;

	public:
		GameApp();
		~GameApp();

		void Init();
		void LoadingThreadProc();
		void LoadingThreadCompleted();
		void TitleScreenIsFinished();

		void WriteToRegistry();
		void ReadFromRegistry();
		void LoadStartupUser();
		void SetCurrentUser(User *user);

		void ShowTitle();
		void CloseTitle();
		void ShowMenu();
		void CloseMenu();
		void ShowGame(GAME_MODE mode, GSET set, GameResults results);
		void ShowGame(GAME_MODE mode, GSET set);
		void CloseGame();
		void ShowResults(GameResults results);
		void CloseResults();
		void ShowHighscores();
		void CloseHighscores();

		void GotFocus();
		void LostFocus();
	};
}


#endif