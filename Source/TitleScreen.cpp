/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "Strings.h"
#include "TitleScreen.h"
#include "GameApp.h"
#include "Res.h"

#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/Image.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Rect.h"
#include "SexyAppFramework/HyperlinkWidget.h"
#include "SexyAppFramework/BassMusicInterface.h"

using namespace Sexy;

TitleScreen::TitleScreen(GameApp *pApp)
{
	mApp = pApp;		
	mContinueLink = NULL;
}

TitleScreen::~TitleScreen()
{
	//Delete the continue link
	delete mContinueLink;
}

void TitleScreen::Init(void)
{
	//Create and initialize continue link
	mContinueLink = new HyperlinkWidget(1, this);
	mContinueLink->SetFont(FONT_NORMAL);
	mContinueLink->mLabel = SZ_CLICK_HERE_TO_PLAY;
	mContinueLink->mColor = Color::White;
	mContinueLink->mOverColor = Color(0, 0, 255);
	mContinueLink->mUnderlineSize = 1;
	mContinueLink->mDoFinger = true;

	//Hide the continue link
	mContinueLink->SetVisible(false);
}

void TitleScreen::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Widget::Resize(theX, theY, theWidth, theHeight);

	//Set the size and position of the continue link
	int labelWidth = FONT_ARIAL->StringWidth(mContinueLink->mLabel);
	int labelHeight = FONT_ARIAL->GetHeight() + 4;
	mContinueLink->Resize(	mWidth / 2 - labelWidth / 2, 478, labelWidth, labelHeight);
}


void TitleScreen::AddedToManager(WidgetManager *theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);	

	//Add continue link to widget manager
	theWidgetManager->AddWidget(mContinueLink);

	//Play music
	mApp->mMusicInterface->PlayMusic(MUSIC_TITLE_SCREEN);
}

void TitleScreen::RemovedFromManager(WidgetManager *theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);

	//Fade out music
	mApp->mMusicInterface->FadeOut(MUSIC_TITLE_SCREEN);

	//Remove continue link from widget manager
	theWidgetManager->RemoveWidget(mContinueLink);
}

void TitleScreen::Draw(Graphics *g)
{
	//Draw the background image
	g->DrawImage(IMAGE_TITLE_SCREEN, 0, 0);

	//Draw the loading progress bar
	int loaderBarWidth = IMAGE_LOADING_BAR->GetWidth();
	int drawWidth = (int) (mApp->GetLoadingThreadProgress() * (double)loaderBarWidth);
	if (drawWidth > 0)
	{
		g->DrawImage(IMAGE_LOADING_BAR, mWidth / 2 - loaderBarWidth / 2, 
					 411, 
					Rect(0, 0, drawWidth, IMAGE_LOADING_BAR->GetHeight()));
	}
}

void TitleScreen::LoadingComplete()
{
	//Show the continue link
	mContinueLink->SetVisible(true);
}

void TitleScreen::ButtonDepress(int theId)
{
	if (theId == 1)
	{
		//Send a finish call to GameApp
		mApp->TitleScreenIsFinished();
	}
}