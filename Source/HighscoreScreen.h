/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef HIGHSCORE_DIALOG_H_
#define HIGHSCORE_DIALOG_H_

#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonWidget.h"
#include "SexyAppFramework/WidgetManager.h"

namespace Sexy
{
	enum GSET;
	class WidgetManager;
	class GameApp;
	class Game;
	class HighscoreScreen : public Widget, ButtonListener
	{
		private:
			enum WIDGET_ID
			{
				MENU_BUTTON_ID,
				ADVENTURE_BUTTON_ID,
				QUICK_BUTTON_ID,
				SECRET_BUTTON_ID,
				PERSONAL_BUTTON_ID,
			};
			ButtonWidget *mMenuButton;
			ButtonWidget *mAdventureButton;
			ButtonWidget *mQuickplayButton;
			ButtonWidget *mSecretButton;
			ButtonWidget *mPersonalButton;

			int mPage;
			char *mPageName;
		public:
			GameApp*	mApp;
			HighscoreScreen(GameApp* theApp);
			void Init();
			void AddedToManager(WidgetManager *theWidgetManager);
			void RemovedFromManager(WidgetManager *theWidgetManager);
			void ButtonDepress(int theId);
			virtual ~HighscoreScreen();
			virtual void Draw(Graphics* g);
			virtual void Update();

			void SetPage(int page);
	};
}


#endif