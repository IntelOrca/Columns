/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef TITLESCREEN_H_
#define TITLESCREEN_H_

#include "SexyAppFramework/Widget.h"
#include "SexyAppFramework/ButtonListener.h"

namespace Sexy
{
	class GameApp;
	class Graphics;
	class WidgetManager;
	class HyperlinkWidget;

	class TitleScreen : public Widget, public ButtonListener
	{
	private:
		GameApp* mApp;
		HyperlinkWidget *mContinueLink;

	public:
		TitleScreen(GameApp* pApp);
		~TitleScreen();
		void Init(void);
		void Resize(int theX, int theY, int theWidth, int theHeight);
		void AddedToManager(WidgetManager* theWidgetManager);
		void RemovedFromManager(WidgetManager* theWidgetManager);
		void ButtonDepress(int theId);
		void Draw(Graphics* g);
		void LoadingComplete();
	};

} 

#endif