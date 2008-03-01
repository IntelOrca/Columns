/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef STAGESELECT_H_
#define STAGESELECT_H_

#include "SexyAppFramework/Dialog.h"

namespace Sexy
{
	class Graphics;
	class DialogButton;
	class ButtonWidget;
	class Menu;
	class StageSelect : public Dialog
	{
		public:
			static const int DIALOG_ID = 3;
			enum
			{
				MENU_BUTTON_ID,
				STAGE1_BUTTON_ID,
				STAGE2_BUTTON_ID,
				STAGE3_BUTTON_ID,
				STAGE4_BUTTON_ID,
			};

			DialogButton	*mMenuButton;
			ButtonWidget	*mStage1Button;
			ButtonWidget	*mStage2Button;
			ButtonWidget	*mStage3Button;
			ButtonWidget	*mStage4Button;

			Menu *mMenu;
		public:	
			StageSelect(std::string theHeader, std::string theBody);
			virtual ~StageSelect();
			virtual void Draw(Graphics* g);
			virtual void Update();
			virtual void ButtonDepress(int theId);
			virtual void AddedToManager(WidgetManager* theWidgetManager);
			virtual void RemovedFromManager(WidgetManager* theWidgetManager);
			virtual void Resize(int theX, int theY, int theWidth, int theHeight);
	};
}

#endif