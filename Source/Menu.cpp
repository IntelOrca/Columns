/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework/BassMusicInterface.h"
#include "SexyAppFramework/HyperlinkWidget.h"
#include "SexyAppFramework/Font.h"

#include "Menu.h"
#include "Res.h"
#include "Strings.h"
#include "User.h"
#include "Game.h"
#include "StageSelect.h"
#include "OptionsDialog.h"
#include "UserDialog.h"
#include "GameApp.h"

using namespace Sexy;

Menu::Menu(GameApp* theApp)
{
	mApp = theApp;
	mAdventureButton = NULL;
	mQuickplayButton = NULL;
	mOptionsButton = NULL;
	mQuitButton = NULL;

	mUserHyperlink = NULL;
	mHighscoresHyperlink = NULL;
}

void Menu::Init()
{
	mSkypos = -800;

	//Widgets
	mAdventureButton = new ButtonWidget(ADVENTURE_BUTTON_ID, this);
	mQuickplayButton = new ButtonWidget(QUICK_BUTTON_ID, this);
	mOptionsButton = new ButtonWidget(OPTIONS_BUTTON_ID, this);
	mQuitButton = new ButtonWidget(QUIT_BUTTON_ID, this);
	mUserHyperlink = new HyperlinkWidget(USER_HYPERLINK_ID, this);
	mHighscoresHyperlink = new HyperlinkWidget(HIGHSCORES_HYPERLINK_ID, this);

	//Misc
	mAdventureButton->mDoFinger = true;
	mQuickplayButton->mDoFinger = true;
	mOptionsButton->mDoFinger = true;
	mQuitButton->mDoFinger = true;
	mUserHyperlink->SetFont(FONT_NORMAL);
	mUserHyperlink->mLabel = SZ_NOT_YOU;
	mUserHyperlink->mColor = Color::White;
	mUserHyperlink->mOverColor = Color(255, 255, 0);
	mUserHyperlink->mUnderlineSize = 1;
	mHighscoresHyperlink->SetFont(FONT_NORMAL);
	mHighscoresHyperlink->mLabel = SZ_HIGHSCORES;
	mHighscoresHyperlink->mColor = Color::White;
	mHighscoresHyperlink->mOverColor = Color(255, 255, 0);
	mHighscoresHyperlink->mUnderlineSize = 1;

	//Location & Size
	mAdventureButton->Resize(		472, 130, 280, 82);
	mQuickplayButton->Resize(		472, 231, 280, 82);
	mOptionsButton->Resize(			472, 332, 280, 82);
	mQuitButton->Resize(			472, 434, 280, 82);
	mUserHyperlink->Resize(			5, 5 + FONT_NORMAL->GetHeight(), FONT_NORMAL->StringWidth(mUserHyperlink->mLabel), FONT_NORMAL->GetHeight() + 4);
	mHighscoresHyperlink->Resize(	442, 525, FONT_NORMAL->StringWidth(mUserHyperlink->mLabel), FONT_NORMAL->GetHeight() + 4);

	//Images
	mAdventureButton->mButtonImage	=	IMAGE_BTN_MENU_ADVENTURE_UP;
	mAdventureButton->mOverImage	=	IMAGE_BTN_MENU_ADVENTURE_OVER;
	mAdventureButton->mDownImage	=	IMAGE_BTN_MENU_ADVENTURE_DOWN;
	mQuickplayButton->mButtonImage		=	IMAGE_BTN_MENU_QUICK_UP;
	mQuickplayButton->mOverImage		=	IMAGE_BTN_MENU_QUICK_OVER;
	mQuickplayButton->mDownImage		=	IMAGE_BTN_MENU_QUICK_DOWN;
	mOptionsButton->mButtonImage	=	IMAGE_BTN_MENU_OPTIONS_UP;
	mOptionsButton->mOverImage		=	IMAGE_BTN_MENU_OPTIONS_OVER;
	mOptionsButton->mDownImage		=	IMAGE_BTN_MENU_OPTIONS_DOWN;
	mQuitButton->mButtonImage		=	IMAGE_BTN_MENU_QUIT_UP;
	mQuitButton->mOverImage		=	IMAGE_BTN_MENU_QUIT_OVER;
	mQuitButton->mDownImage		=	IMAGE_BTN_MENU_QUIT_DOWN;
}

void Menu::AddedToManager(WidgetManager *theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mAdventureButton);
	theWidgetManager->AddWidget(mQuickplayButton);
	theWidgetManager->AddWidget(mOptionsButton);
	theWidgetManager->AddWidget(mQuitButton);
	theWidgetManager->AddWidget(mUserHyperlink);
	theWidgetManager->AddWidget(mHighscoresHyperlink);
	mApp->mMusicInterface->FadeIn(MUSIC_MAIN_MENU);

	MakeSureUser();
}

void Menu::RemovedFromManager(WidgetManager *theWidgetManager)
{
	mApp->mMusicInterface->FadeOut(MUSIC_MAIN_MENU);
	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mAdventureButton);
	theWidgetManager->RemoveWidget(mQuickplayButton);
	theWidgetManager->RemoveWidget(mOptionsButton);
	theWidgetManager->RemoveWidget(mQuitButton);
	theWidgetManager->RemoveWidget(mUserHyperlink);
	theWidgetManager->RemoveWidget(mHighscoresHyperlink);
}

void Menu::ButtonMouseEnter(int theId)
{
	if (theId == ADVENTURE_BUTTON_ID ||
		theId == QUICK_BUTTON_ID ||
		theId == OPTIONS_BUTTON_ID ||
		theId == QUIT_BUTTON_ID) {
		mApp->PlaySample(SOUND_SELECT);
	}
}

void Menu::ButtonDepress(int theId)
{
	UserDialog *udlg;
	OptionsDialog *dlg;
	StageSelect *ssdlg;
	//Close options dialog
	if (theId == OptionsDialog::MESSAGE_BOX_ID + 2000)
		mApp->KillDialog(theId - 2000);
	//Main buttons
	switch (theId) {
		case ADVENTURE_BUTTON_ID:
			mApp->CloseMenu();
			mApp->ShowGame(MODE_ADVENTURE, GSET_MODERN);
			break;
		case QUICK_BUTTON_ID:
			ssdlg = new StageSelect(SZ_QUICKPLAY, SZ_NULL);
			ssdlg->mMenu = this;
			ssdlg->Resize(mWidth / 2 - 550 / 2, mHeight / 2 - 470 / 2, 550, 470);
			mApp->AddDialog(StageSelect::DIALOG_ID, ssdlg);
			break;
		case OPTIONS_BUTTON_ID:
			dlg = new OptionsDialog(false);
			dlg->Resize(mWidth / 2 - 450 / 2, mHeight / 2 - 320 / 2);
			mApp->AddDialog(OptionsDialog::DIALOG_ID, dlg);
			break;
		case QUIT_BUTTON_ID:
			gSexyAppBase->Shutdown();
			break;
		case USER_HYPERLINK_ID:
			udlg = new UserDialog(SZ_WHO_ARE_YOU, SZ_NULL);
			udlg->mApp = mApp;
			udlg->Resize(mWidth / 2 - 400 / 2, mHeight / 2 - 400 / 2, 400, 400);
			mApp->AddDialog(udlg);
			mApp->mUserDialog = udlg;
			break;
		case HIGHSCORES_HYPERLINK_ID:
			mApp->CloseMenu();
			mApp->ShowHighscores();
			break;
	}
}

void Menu::Draw(Graphics* g)
{
	int x, y, w, h;
	g->DrawImage(IMAGE_BLUE_SKY, mSkypos - 1600, 0);
	g->DrawImage(IMAGE_BLUE_SKY, mSkypos, 0);
	g->DrawImage(IMAGE_MAIN_MENU, 0, 0);
	g->SetColor(Color::White);
	g->SetFont(FONT_HUX);

	if (mApp->mCurrentUser != NULL) {
		//Draw username
		char username[50];	sprintf(username, SZ_WELCOME_TO_COLUMNS_USER, mApp->mCurrentUser->mName);
		g->DrawString(username, 5, FONT_NORMAL->GetHeight());

		//Draw adventure progress
		char level[20];	sprintf(level, SZ_CURRENT_A_LEVEL, mApp->mCurrentUser->mStage, mApp->mCurrentUser->mLevel);
		g->SetFont(FONT_NORMAL);
		g->SetColor(Color(255, 255, 0));

		x = mAdventureButton->mX + (mAdventureButton->mWidth / 2);
		w = g->StringWidth(level);
		g->DrawString(level, x - (w / 2), mAdventureButton->mY - 10);
	} else {
		//Draw welcome
		g->DrawString(SZ_WELCOME_TO_COLUMNS, 5, FONT_NORMAL->GetHeight());
	}

	//Draw copyright
	g->SetColor(Color::White);
	g->SetFont(FONT_HUX);
	g->DrawString(APP_COPYRIGHT, 5, mHeight - 20);
}

void Menu::UpdateF(float theFrac)
{
	mSkypos += 0.2f * theFrac;

	//Restart the sky if bigger than width * 2
	if (mSkypos > mWidth * 2)
		mSkypos = 0;

	MarkAllDirty();
}

Menu::~Menu()
{
	delete mAdventureButton;
	delete mQuickplayButton;
	delete mOptionsButton;
	delete mQuitButton;
	delete mUserHyperlink;
	delete mHighscoresHyperlink;
}

void Menu::MakeSureUser()
{
	if (mApp->mCurrentUser == NULL) {
		//User name dialog
		UserDialog *udlg;
		udlg = new UserDialog(SZ_WHO_ARE_YOU, SZ_NULL);
		udlg->mApp = mApp;
		udlg->Resize(mWidth / 2 - 400 / 2, mHeight / 2 - 400 / 2, 400, 400);
		mApp->AddDialog(udlg);
		mApp->mUserDialog = udlg;

		udlg->NewUserDialog();
	}
}

void Menu::Quickplay(GSET set)
{
	mApp->CloseMenu();
	mApp->ShowGame(MODE_QUICKPLAY, set);
}