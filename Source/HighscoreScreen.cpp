/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework/BassMusicInterface.h"
#include "SexyAppFramework/HyperlinkWidget.h"
#include "SexyAppFramework/Font.h"

#include "HighscoreScreen.h"
#include "Res.h"
#include "Strings.h"
#include "Core.h"
#include "User.h"
#include "Highscore.h"
#include "GameApp.h"

using namespace Sexy;

HighscoreScreen::HighscoreScreen(GameApp* theApp)
{
	mApp = theApp;
	mMenuButton = NULL;
	mAdventureButton = NULL;
	mQuickplayButton = NULL;
	mSecretButton = NULL;
	mPersonalButton = NULL;
}

HighscoreScreen::~HighscoreScreen()
{
	delete mMenuButton;
	delete mAdventureButton;
	delete mQuickplayButton;
	delete mSecretButton;
	delete mPersonalButton;
}

void HighscoreScreen::Init()
{
	mPage = 0;

	//Widgets
	mMenuButton = new ButtonWidget(MENU_BUTTON_ID, this);
	mAdventureButton = new ButtonWidget(ADVENTURE_BUTTON_ID, this);
	mQuickplayButton = new ButtonWidget(QUICK_BUTTON_ID, this);
	mSecretButton = new ButtonWidget(SECRET_BUTTON_ID, this);
	mPersonalButton = new ButtonWidget(PERSONAL_BUTTON_ID, this);

	//Misc
	mMenuButton->mDoFinger = true;
	mAdventureButton->mDoFinger = true;
	mQuickplayButton->mDoFinger = true;
	mSecretButton->mDoFinger = true;
	mPersonalButton->mDoFinger = true;

	//Location & Size
	mMenuButton->Resize(		0, 0, 115, 31);
	mAdventureButton->Resize(	39, 543, 115, 31);
	mQuickplayButton->Resize(	233, 543, 115, 31);
	mSecretButton->Resize(		447, 543, 115, 31);
	mPersonalButton->Resize(	646, 543, 115, 31);

	//Buttons
	mMenuButton->mButtonImage = IMAGE_DIALOG_BTN;
	mMenuButton->SetFont(FONT_NORMAL);
	mMenuButton->mLabel = SZ_MENU;

	mAdventureButton->mButtonImage = IMAGE_DIALOG_BTN;
	mAdventureButton->SetFont(FONT_NORMAL);
	mAdventureButton->mLabel = SZ_ADVENTURE;

	mQuickplayButton->mButtonImage = IMAGE_DIALOG_BTN;
	mQuickplayButton->SetFont(FONT_NORMAL);
	mQuickplayButton->mLabel = SZ_QUICKPLAY;

	mSecretButton->mButtonImage = IMAGE_DIALOG_BTN;
	mSecretButton->SetFont(FONT_NORMAL);
	mSecretButton->mLabel = SZ_SECRET;

	mPersonalButton->mButtonImage = IMAGE_DIALOG_BTN;
	mPersonalButton->SetFont(FONT_NORMAL);
	mPersonalButton->mLabel = SZ_PERSONAL;

	SetPage(0);
}

void HighscoreScreen::AddedToManager(WidgetManager *theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mAdventureButton);
	theWidgetManager->AddWidget(mQuickplayButton);
	theWidgetManager->AddWidget(mSecretButton);
	theWidgetManager->AddWidget(mPersonalButton);

	mApp->mMusicInterface->FadeIn(MUSIC_TITLE_SCREEN);
}

void HighscoreScreen::RemovedFromManager(WidgetManager *theWidgetManager)
{
	mApp->mMusicInterface->FadeOut(MUSIC_TITLE_SCREEN);

	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mAdventureButton);
	theWidgetManager->RemoveWidget(mQuickplayButton);
	theWidgetManager->RemoveWidget(mSecretButton);
	theWidgetManager->RemoveWidget(mPersonalButton);
}

void HighscoreScreen::ButtonDepress(int theId)
{
	//Buttons
	switch (theId) {
		case MENU_BUTTON_ID:
			mApp->CloseHighscores();
			mApp->ShowMenu();
			break;
		case ADVENTURE_BUTTON_ID:
			SetPage(0);
			break;
		case QUICK_BUTTON_ID:
			SetPage(1);
			break;
		case SECRET_BUTTON_ID:
			SetPage(2);
			break;
		case PERSONAL_BUTTON_ID:
			SetPage(3);
			break;
	}
}

void HighscoreScreen::Draw(Graphics* g)
{
	char buffer[100];
	int x, y, height;

	g->SetColor(Color(100, 0, 255));
	g->FillRect(0, 0, mWidth, mHeight);

	//Draw title
	g->SetColor(Color::White);
	g->SetFont(FONT_HUX);
	x = (mWidth / 2) - (g->StringWidth(SZ_HIGHSCORES) / 2);
	y = 5 + FONT_HUX->GetHeight();
	g->DrawString(SZ_HIGHSCORES, x, y);

	//Draw page name
	g->SetFont(FONT_NORMAL);
	x = (mWidth / 2) - (g->StringWidth(mPageName) /2 );
	y = 50 + FONT_NORMAL->GetHeight();
	g->DrawString(mPageName, x, y);


	if (mPage == 1) {
		height = FONT_NORMAL->GetHeight();
		y = 130;

		//Draw highscore table
		const int tableStart = 150;
		const int colSpacing = 50;
		int colWidths[5];
		colWidths[0] = g->StringWidth(SZ_PLACE) + colSpacing;
		colWidths[1] = g->StringWidth(SZ_SCORE) + colSpacing;
		colWidths[2] = g->StringWidth(SZ_MOVES) + colSpacing;
		colWidths[3] = g->StringWidth(SZ_TIME) + colSpacing;
		colWidths[4] = g->StringWidth(SZ_NAME) + colSpacing;

		//Headings
		x = tableStart;
		g->DrawString(SZ_PLACE, x, y);
		g->DrawString(SZ_SCORE, x += colWidths[0], y);
		g->DrawString(SZ_MOVES, x += colWidths[1], y);
		g->DrawString(SZ_TIME, x += colWidths[2], y);
		g->DrawString(SZ_NAME, x += colWidths[3], y);

		g->DrawLineAA(tableStart, y + 10, x += colWidths[4], y + 10);

		for (int i = 0; i < MAX_QUICKPLAY_SCORES; i++) {
			x = tableStart;
			y += height + 5;

			//Get highscore
			Highscore hs = TopQuickplayHighscores[i];

			//Draw number
			sprintf(buffer, "%d.", i + 1);
			g->DrawString(buffer, x, y);

			//Draw score
			sprintf(buffer, "%d", hs.Score);
			g->DrawString(buffer, x += colWidths[0], y);

			//Draw moves
			sprintf(buffer, "%d", hs.Moves);
			g->DrawString(buffer, x += colWidths[1], y);

			//Draw time
			strcpy(buffer, GetTime(hs.Time));
			g->DrawString(buffer, x += colWidths[2], y);

			//Draw name
			g->DrawString(hs.Name, x += colWidths[3], y);
		}
	}
}

void HighscoreScreen::Update()
{

}

void HighscoreScreen::SetPage(int page)
{
	mPage = page;
	
	switch (page) {
		case 0:
			mPageName = SZ_ADVENTURE;
			break;
		case 1:
			mPageName = SZ_QUICKPLAY;
			break;
		case 2:
			mPageName = SZ_SECRET;
			break;
		case 3:
			mPageName = SZ_PERSONAL;
			break;
	}

	//MarkAllDirty();
}