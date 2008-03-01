/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework/BassMusicInterface.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/Font.h"

#include "ResultsScreen.h"
#include "Game.h"
#include "Res.h"
#include "Strings.h"
#include "Core.h"
#include "GameApp.h"

using namespace Sexy;

ResultsScreen::ResultsScreen(GameApp* theApp)
{
	mApp = theApp;
	mMenuButton = NULL;
	mContinueButton = NULL;
}

ResultsScreen::~ResultsScreen()
{
	delete mMenuButton;
	delete mContinueButton;
}

void ResultsScreen::Init(GameResults results)
{
	mResults = results;

	//Widgets
	mMenuButton = new ButtonWidget(MENU_BUTTON_ID, this);
	mContinueButton = new DialogButton(IMAGE_DIALOG_BTN, CONTINUE_BUTTON_ID, this);

	//Misc
	mMenuButton->mDoFinger = true;
	mContinueButton->mDoFinger = true;

	//Location & Size
	mMenuButton->Resize(		0, 0, 115, 31);
	mContinueButton->Resize(	500, 543, 270, 31);

	//Buttons
	mMenuButton->mButtonImage = IMAGE_DIALOG_BTN;
	mMenuButton->SetFont(FONT_NORMAL);
	mMenuButton->mLabel = SZ_MENU;

	mContinueButton->SetFont(FONT_NORMAL);
	mContinueButton->mLabel = SZ_CLICK_HERE_TO_CONTINUE;
	mContinueButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
	mContinueButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);
}

void ResultsScreen::AddedToManager(WidgetManager *theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mContinueButton);

	mApp->mMusicInterface->FadeIn(MUSIC_TITLE_SCREEN);
}

void ResultsScreen::RemovedFromManager(WidgetManager *theWidgetManager)
{
	mApp->mMusicInterface->FadeOut(MUSIC_TITLE_SCREEN);

	Widget::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mContinueButton);
}

void ResultsScreen::ButtonDepress(int theId)
{
	//Buttons
	switch (theId) {
		case MENU_BUTTON_ID:
			mApp->ShowMenu();
			mApp->CloseResults();
			break;
		case CONTINUE_BUTTON_ID:
			if (mResults.Type == 0) {
				//Adventure
				mApp->ShowGame(MODE_ADVENTURE, GSET_MODERN);
				mApp->CloseResults();
			} else if (mResults.Type == 1) {
				//Quick play
				mApp->ShowMenu();
				mApp->CloseResults();
			}
			break;
	}
}

void ResultsScreen::Draw(Graphics* g)
{
	DrawBackground(g);

	if (mResults.Type == 0) {
		DrawAdventure(g);
	} else if (mResults.Type == 1) {
		DrawQuickPlay(g);
	}
}

void ResultsScreen::DrawBackground(Graphics* g)
{
	g->SetColor(Color(100, 0, 255));
	g->FillRect(0, 0, mWidth, mHeight);
}

void ResultsScreen::DrawQuickPlay(Graphics* g)
{
	char buffer[10];
	RoundResults results = mResults.Rounds[0];

	//Title
	g->SetColor(Color::White);
	g->SetFont(FONT_HUX);
	g->DrawString(SZ_RESULTS, (mWidth / 2) - (g->StringWidth(SZ_RESULTS) / 2), FONT_HUX->GetHeight() + 5);

	g->SetColor(Color(255, 255, 0));
	g->SetFont(FONT_NORMAL);

	char ctime[10];
	strcpy(ctime, GetTime(results.Time));
	
	DrawScoreAttribute(g, "MOVES", results.Moves,					80,		110);
	DrawScoreAttribute(g, "JEWELS", results.Jewels,					600,	110);
	DrawScoreAttribute(g, "LEVEL", results.Level,					80,		230);
	DrawScoreAttribute(g, "CHAIN REACTIONS", results.Chains,		600,	230);
	DrawScoreAttribute(g, "TIME", ctime,							80,		360);
	DrawScoreAttribute(g, "MAX CHAIN REACTION", results.MaxChain,	600,	360);
	DrawScoreAttribute(g, "SCORE", results.Score,					200,	480);
}

void ResultsScreen::DrawAdventure(Graphics* g)
{
	char buffer[10];
	RoundResults results = mResults.Rounds[0];

	//Title
	g->SetColor(Color::White);
	g->SetFont(FONT_HUX);
	g->DrawString(SZ_RESULTS, (mWidth / 2) - (g->StringWidth(SZ_RESULTS) / 2), FONT_HUX->GetHeight() + 5);

	g->SetColor(Color(255, 255, 0));
	g->SetFont(FONT_NORMAL);

	//Draw headings
	g->DrawString("ROUND 1", 235, 130);
	g->DrawString("ROUND 2", 385, 130);
	g->DrawString("ROUND 3", 535, 130);
	g->DrawString("TOTAL", 700, 130);

	g->DrawString("MOVES", 15, 190);
	g->DrawString("CHAIN", 15, 240);
	g->DrawString("REACTIONS", 15, 260);
	g->DrawString("MAX CHAIN", 15, 315);
	g->DrawString("REACTION", 15, 335);
	g->DrawString("TIME", 15, 400);
	g->DrawString("SCORE", 15, 485);

	//Create a round for the total results
	RoundResults total;
	memset(&total, 0, sizeof(RoundResults));

	for (int i = 0; i < 3; i++) {
		sprintf(buffer, "ROUND %d", i + 1);
		int cx = (235 + (i * 150)) + (g->StringWidth(buffer) / 2);

		RoundResults round = mResults.Rounds[i];

		if (mResults.PlayedRounds > i) {
			DrawScoreAttribute(g, round.Moves, cx, 190);
			DrawScoreAttribute(g, round.Chains, cx, 250);
			DrawScoreAttribute(g, round.MaxChain, cx, 325);
			DrawScoreAttribute(g, round.Time, cx, 400);
			DrawScoreAttribute(g, round.Score, cx, 485);

			//Add the round to total
			total.Moves += round.Moves;
			total.Chains += round.Chains;
			total.Time += round.Time;
			total.Score += round.Score;

			if (round.MaxChain > total.MaxChain)
				total.MaxChain = round.MaxChain;
		} else {
			DrawScoreAttribute(g, "-", cx, 190);
			DrawScoreAttribute(g, "-", cx, 250);
			DrawScoreAttribute(g, "-", cx, 325);
			DrawScoreAttribute(g, "-", cx, 400);
			DrawScoreAttribute(g, "-", cx, 485);
		}
	}

	//Total scores
	int cx = 700 + (g->StringWidth("TOTAL") / 2);
	DrawScoreAttribute(g, total.Moves, cx, 190);
	DrawScoreAttribute(g, total.Chains, cx, 250);
	DrawScoreAttribute(g, total.MaxChain, cx, 325);
	DrawScoreAttribute(g, total.Time, cx, 400);
	DrawScoreAttribute(g, total.Score, cx, 485);
}

void ResultsScreen::DrawScoreAttribute(Graphics *g, int value, int x, int y)
{
	//Turn value into cstring
	char buffer[10];
	sprintf(buffer, "%d", value);
	DrawScoreAttribute(g, buffer, x, y);
}

void ResultsScreen::DrawScoreAttribute(Graphics *g, char *value, int x, int y)
{
	//Calculate  value
	int vWidth = g->StringWidth(value);
	int vX = x - (vWidth / 2);
	int vY = y;

	//Draw value
	g->DrawString(value, vX, vY);
}

void ResultsScreen::DrawScoreAttribute(Graphics *g, char *header, int value, int x, int y)
{
	//Turn value into cstring
	char buffer[10];
	sprintf(buffer, "%d", value);
	DrawScoreAttribute(g, header, buffer, x, y);
}

void ResultsScreen::DrawScoreAttribute(Graphics *g, char *header, char *value, int x, int y)
{
	//Calculate header and value
	int hWidth = g->StringWidth(header);
	int hHeight = g->mFont->GetHeight();
	int hX = x - (hWidth / 2);
	int hY = y + hHeight;

	int vWidth = g->StringWidth(value);
	int vX = x - (vWidth / 2);
	int vY = y + hHeight + hHeight;

	//Draw header
	g->DrawString(header, hX, hY);

	//Draw value
	g->DrawString(value, vX, vY);
}

void ResultsScreen::Update()
{

}