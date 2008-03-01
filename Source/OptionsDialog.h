/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef OPTIONSDIALOG_H_
#define OPTIONSDIALOG_H_

#include "SexyAppFramework/Dialog.h"
#include "SexyAppFramework/SliderListener.h"
#include "SexyAppFramework/CheckboxListener.h"

namespace Sexy
{
	class GameApp;
	class Game;
	class Graphics;
	class Slider;
	class DialogButton;
	class Checkbox;
	class OptionsDialog : public Dialog, public SliderListener, public CheckboxListener
	{
	protected:
		Slider*			mMusicVolumeSlider;
		Slider*			mSfxVolumeSlider;
	public:
		static const int DIALOG_ID = 99;
		static const int OPTIONS_WIDTH = 450;
		static const int OPTIONS_HEIGHT = 320;
		static const int GAMEMENU_WIDTH = 450;
		static const int GAMEMENU_HEIGHT = 360;
		enum
		{
			MUSIC_SLIDER_ID,
			SFX_SLIDER_ID,
			FS_CHECKBOX_ID,
			HARDWARE_CHECKBOX_ID,
			MAINMENU_BUTTON_ID,
			MESSAGE_BOX_ID,
		};
		GameApp *mApp;
		Game *mGame;

	private:
		bool			mInGame;				//Whether we are in game menu or not
		Checkbox*		m3DCheckbox;			//Toggle 3D enhanced mode
		Checkbox*		mFSCheckbox;			//Toggle fullscreen mode
		DialogButton	*mMainMenuButton;		//Main menu button
	public:
		OptionsDialog(bool inGame);
		~OptionsDialog();
		void Draw(Graphics* g);
		void Update();	
		void ButtonDepress(int theId);
		void AddedToManager(WidgetManager* theWidgetManager);
		void RemovedFromManager(WidgetManager* theWidgetManager);
		void Resize(int theX, int theY);
		void Resize(int theX, int theY, int theWidth, int theHeight);
		void SliderVal(int theId, double theVal);
		void CheckboxChecked(int theId, bool checked);
	};
}

#endif