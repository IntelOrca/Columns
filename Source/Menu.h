/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef MENU_H_
#define MENU_H_

#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonWidget.h"
#include "SexyAppFramework/WidgetManager.h"

namespace Sexy
{
	enum GSET;
	class WidgetManager;
	class HyperlinkWidget;
	class GameApp;
	class Game;
	class Menu : public Widget, ButtonListener
	{
	private:
		enum WIDGET_ID
		{
			ADVENTURE_BUTTON_ID,
			QUICK_BUTTON_ID,
			OPTIONS_BUTTON_ID,
			QUIT_BUTTON_ID,
			USER_HYPERLINK_ID,
			HIGHSCORES_HYPERLINK_ID,
			DIALOG_ID,
		};
		ButtonWidget *mAdventureButton;
		ButtonWidget *mQuickplayButton;
		ButtonWidget *mOptionsButton;
		ButtonWidget *mQuitButton;
		HyperlinkWidget *mUserHyperlink;
		HyperlinkWidget *mHighscoresHyperlink;

		float mSkypos;

	public:
		GameApp*	mApp;

	public:
		Menu(GameApp* theApp);
		~Menu();

		void Draw(Graphics* g);
		void UpdateF(float theFrac);
		void AddedToManager(WidgetManager *theWidgetManager);
		void RemovedFromManager(WidgetManager *theWidgetManager);
		void ButtonMouseEnter(int theId);
		void ButtonDepress(int theId);

		void Init();
		void MakeSureUser();
		void Quickplay(GSET set);
	};
}


#endif