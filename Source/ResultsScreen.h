/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef RESULTS_SCREEN_H_
#define RESULTS_SCREEN_H_

#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonWidget.h"
#include "SexyAppFramework/WidgetManager.h"

namespace Sexy
{
	enum GSET;
	class WidgetManager;
	class DialogButton;
	class GameApp;
	class Game;

	struct RoundResults
	{
		int Score,
			Jewels,
			Moves,
			Chains,
			MaxChain,
			Level,
			Time;
	};

	struct GameResults
	{
		int Type;
		int PlayedRounds;
		int Wins[3];
		RoundResults Rounds[3];
	};

	class ResultsScreen : public Widget, ButtonListener
	{
		private:
			enum WIDGET_ID
			{
				MENU_BUTTON_ID,
				CONTINUE_BUTTON_ID,
			};
			ButtonWidget *mMenuButton;
			DialogButton *mContinueButton;

			GameResults mResults;
		public:
			GameApp* mApp;
			ResultsScreen(GameApp* theApp);
			~ResultsScreen();
			void Init(GameResults results);
			void Update();
			void Draw(Graphics* g);
			void DrawBackground(Graphics* g);
			void DrawQuickPlay(Graphics* g);
			void DrawAdventure(Graphics* g);
			void DrawScoreAttribute(Graphics *g, int value, int x, int y);
			void DrawScoreAttribute(Graphics *g, char *value, int x, int y);
			void DrawScoreAttribute(Graphics *g, char *header, int value, int x, int y);
			void DrawScoreAttribute(Graphics *g, char *header, char *value, int x, int y);
			void AddedToManager(WidgetManager *theWidgetManager);
			void RemovedFromManager(WidgetManager *theWidgetManager);
			void ButtonDepress(int theId);
	};
}


#endif