/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "Strings.h"
#include "OptionsDialog.h"
#include "Res.h"
#include "GameApp.h"
#include "Game.h"
#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/Checkbox.h"
#include "SexyAppFramework/Slider.h"

#include "MessageDialog.h"

using namespace Sexy;

OptionsDialog::OptionsDialog(bool inGame) :
Dialog(IMAGE_DIALOG, IMAGE_DIALOG_BTN, OptionsDialog::DIALOG_ID, true, SZ_NULL, SZ_NULL, SZ_CLOSE, Dialog::BUTTONS_FOOTER)
{
	mApp = NULL;
	mGame = NULL;
	mInGame = inGame;

	mContentInsets = Insets(40, 10, 45, 40);
	mSpaceAfterHeader = 10;
	SetHeaderFont(FONT_NORMAL);
	SetLinesFont(FONT_NORMAL);
	SetButtonFont(FONT_NORMAL);

	SetColor(COLOR_HEADER, Color::Black);
	SetColor(COLOR_LINES, Color::Black);
	SetColor(COLOR_BUTTON_TEXT, Color::Black);
	SetColor(COLOR_BUTTON_TEXT_HILITE, Color::Black);

	mMusicVolumeSlider = new Slider(IMAGE_SLIDER, IMAGE_SLIDER_THUMB, OptionsDialog::MUSIC_SLIDER_ID, this);
	mMusicVolumeSlider->SetValue(gSexyAppBase->GetMusicVolume());
	mSfxVolumeSlider = new Slider(IMAGE_SLIDER, IMAGE_SLIDER_THUMB, OptionsDialog::SFX_SLIDER_ID, this);
	mSfxVolumeSlider->SetValue(gSexyAppBase->GetSfxVolume());

	m3DCheckbox = new Checkbox(IMAGE_CHECKBOX, IMAGE_CHECKBOX, OptionsDialog::HARDWARE_CHECKBOX_ID, this);
	mFSCheckbox = new Checkbox(IMAGE_CHECKBOX, IMAGE_CHECKBOX, OptionsDialog::FS_CHECKBOX_ID, this);

	mMainMenuButton = new DialogButton(IMAGE_DIALOG_BTN, MAINMENU_BUTTON_ID, this);
	mMainMenuButton->mLabel = SZ_MAINMENU;
	mMainMenuButton->SetFont(FONT_NORMAL);
	mMainMenuButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
	mMainMenuButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);
	mMainMenuButton->mVisible = mInGame;

	//Set header and footer
	if (mInGame) {
		mDialogHeader = SZ_MENU;
	} else {
		mDialogHeader = SZ_OPTIONS;
	}
}

OptionsDialog::~OptionsDialog()
{
	delete mMusicVolumeSlider;
	delete mSfxVolumeSlider;
	delete m3DCheckbox;
	delete mFSCheckbox;
	delete mMainMenuButton;
}

void OptionsDialog::Draw(Graphics* g)
{
	Dialog::Draw(g);

	g->SetFont(FONT_NORMAL);
	g->SetColor(Color::Black);

	g->DrawString(SZ_MUSIC_VOLUME, mContentInsets.mLeft, mMusicVolumeSlider->mY + mMusicVolumeSlider->mHeight - mY);
	g->DrawString(SZ_SOUND_EFFECTS, mContentInsets.mLeft, mSfxVolumeSlider->mY + mSfxVolumeSlider->mHeight - mY);

	g->DrawString(SZ_FULLSCREEN, mFSCheckbox->mX + mFSCheckbox->mWidth - mX + 10, mFSCheckbox->mY + mFSCheckbox->mWidth - mY);
	g->DrawString(SZ_3D_HARDWARE, m3DCheckbox->mX + m3DCheckbox->mWidth - mX + 10, m3DCheckbox->mY + m3DCheckbox->mWidth - mY);
}

void OptionsDialog::Resize(int theX, int theY)
{
	if (mInGame) {
		Resize(theX, theY, GAMEMENU_WIDTH, GAMEMENU_HEIGHT);
	} else {
		Resize(theX, theY, OPTIONS_WIDTH, OPTIONS_HEIGHT);
	}
}

void OptionsDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);

	int sliderX = FONT_NORMAL->StringWidth(SZ_SOUND_EFFECTS) + 10;

	mMusicVolumeSlider->Resize(theX + mContentInsets.mLeft + sliderX,
							   theY + mContentInsets.mTop + mSpaceAfterHeader + FONT_NORMAL->GetHeight(),
							   mWidth - mContentInsets.mLeft - sliderX - mContentInsets.mRight,
							   IMAGE_SLIDER_THUMB->GetHeight());

	mSfxVolumeSlider->Layout(LAY_SameLeft | LAY_Below | LAY_SameWidth | LAY_SameHeight, 
								mMusicVolumeSlider, 0, 20, 0, 0);

	mFSCheckbox->Layout(LAY_SameLeft | LAY_Below, mSfxVolumeSlider, -sliderX, 20, 0, 0);
	mFSCheckbox->Resize(mFSCheckbox->mX, mFSCheckbox->mY,
						IMAGE_CHECKBOX->mWidth / 2, IMAGE_CHECKBOX->mHeight);

	m3DCheckbox->Layout(LAY_SameLeft | LAY_Below | LAY_SameWidth | LAY_SameHeight,
						mFSCheckbox, 0, 20, 0, 0);

	if (mInGame) {
		mMainMenuButton->Layout(LAY_SameLeft | LAY_Below, m3DCheckbox, 0, 20, 0, 0);
		mMainMenuButton->Resize(mMainMenuButton->mX, mMainMenuButton->mY, mWidth - mContentInsets.mRight - mContentInsets.mLeft, IMAGE_DIALOG_BTN->mHeight);
	}
}

void OptionsDialog::Update()
{
	Dialog::Update();
}

void OptionsDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	Dialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mMusicVolumeSlider);
	theWidgetManager->AddWidget(mSfxVolumeSlider);

	int checkWidth = IMAGE_CHECKBOX->GetWidth() / 2;
	m3DCheckbox->mUncheckedRect = Rect(0, 0, checkWidth, IMAGE_CHECKBOX->GetHeight());
	m3DCheckbox->mCheckedRect = Rect(checkWidth, 0, checkWidth, IMAGE_CHECKBOX->GetHeight());

	mFSCheckbox->mUncheckedRect = Rect(0, 0, checkWidth, IMAGE_CHECKBOX->GetHeight());
	mFSCheckbox->mCheckedRect = Rect(checkWidth, 0, checkWidth, IMAGE_CHECKBOX->GetHeight());
	
	//Let's set the initial checked state of the 3d checkbox to true if the
	//user has 3D support enabled. We determine that via a call to Is3DAccelerated
	m3DCheckbox->mChecked = gSexyAppBase->Is3DAccelerated();
	
	//We can check if the app is fullscreen by examining the mIsWindowed variable:
	mFSCheckbox->mChecked = !gSexyAppBase->mIsWindowed;

	theWidgetManager->AddWidget(m3DCheckbox);
	theWidgetManager->AddWidget(mFSCheckbox);
	theWidgetManager->AddWidget(mMainMenuButton);
}

void OptionsDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Dialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mMusicVolumeSlider);
	theWidgetManager->RemoveWidget(mSfxVolumeSlider);
	theWidgetManager->RemoveWidget(m3DCheckbox);
	theWidgetManager->RemoveWidget(mFSCheckbox);
	theWidgetManager->RemoveWidget(mMainMenuButton);
}

void OptionsDialog::SliderVal(int theId, double theVal)
{
	if (theId == OptionsDialog::MUSIC_SLIDER_ID) {
		gSexyAppBase->SetMusicVolume(theVal);
	} else if (theId == OptionsDialog::SFX_SLIDER_ID) {
		gSexyAppBase->SetSfxVolume(theVal);
		if (!mSfxVolumeSlider->mDragging)
			gSexyAppBase->PlaySample(SOUND_SELECT);
	}
}

void OptionsDialog::ButtonDepress(int theId)
{
	MessageDialog *msgbox;
	int aWidth;
	int result;

	Dialog::ButtonDepress(theId);

	switch (theId) {
		case MAINMENU_BUTTON_ID:
			gSexyAppBase->SwitchScreenMode(!mFSCheckbox->mChecked, m3DCheckbox->mChecked);

			msgbox = new MessageDialog(65, SZ_LEAVE_GAME,
				SZ_RETURN_TO_MAIN_MENU, SZ_LEAVE, SZ_CANCEL,
				MessageDialog::MSGBOX_LEFT_RIGHT);

			aWidth = mApp->mWidth / 2;
			msgbox->Resize(mApp);
			mApp->AddDialog(msgbox->mId, msgbox);

			result = msgbox->WaitForResult(true);
			if (result == MessageDialog::ID_LEFT) {
				//Close options dialog
				gSexyAppBase->KillDialog(this);
				
				//Close game
				mGame->MainMenu();
			}

			break;
		case ID_YES:
			gSexyAppBase->SwitchScreenMode(!mFSCheckbox->mChecked, m3DCheckbox->mChecked);
			gSexyAppBase->KillDialog(this);

			//If game
			if (mInGame) {
				mGame->CloseMenu();
			}

			break;
	}
}

void OptionsDialog::CheckboxChecked(int theId, bool checked)
{
	if (theId == m3DCheckbox->mId) {
		if (checked) {
			if (!gSexyAppBase->Is3DAccelerationSupported())
			{
				m3DCheckbox->SetChecked(false);
				gSexyAppBase->DoDialog(OptionsDialog::MESSAGE_BOX_ID, true, SZ_NOT_SUPPORTED, 
							SZ_HARDWARE_DISABLED,
							SZ_OK, Dialog::BUTTONS_FOOTER);
			}
			else if(!gSexyAppBase->Is3DAccelerationRecommended()) {
				gSexyAppBase->DoDialog(OptionsDialog::MESSAGE_BOX_ID, true, SZ_WARNING, 
					SZ_HARDWARE_WARNING,
					SZ_OK, Dialog::BUTTONS_FOOTER);
			}
			
		}
	} else if (theId == mFSCheckbox->mId) {
		if (gSexyAppBase->mForceFullscreen && !checked) {
			gSexyAppBase->DoDialog(OptionsDialog::MESSAGE_BOX_ID, true, SZ_NO_WINDOW,
			SZ_WINDOW_DISABLED, SZ_OK, Dialog::BUTTONS_FOOTER);

			mFSCheckbox->SetChecked(true);
		}
	}
}