/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "Strings.h"
#include "StageSelect.h"
#include "Res.h"
#include "Menu.h"
#include "GameApp.h"
#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/ButtonWidget.h"

using namespace Sexy;

StageSelect::StageSelect(std::string theHeader, std::string theBody) :
Dialog(IMAGE_DIALOG, IMAGE_DIALOG_BTN, StageSelect::DIALOG_ID, true, StringToSexyStringFast(theHeader), StringToSexyStringFast(theBody), SZ_NULL, Dialog::BUTTONS_NONE)
{
	mContentInsets = Insets(40, 10, 45, 40);
	mSpaceAfterHeader = 10;
	SetHeaderFont(FONT_NORMAL);
	SetLinesFont(FONT_NORMAL);
	SetButtonFont(FONT_NORMAL);

	SetColor(COLOR_HEADER, Color::Black);
	SetColor(COLOR_LINES, Color::Black);

	mMenuButton = new DialogButton(IMAGE_DIALOG_BTN, MENU_BUTTON_ID, this);
	mMenuButton->mColors[DialogButton::COLOR_LABEL] = Color::Black;
	mMenuButton->mColors[DialogButton::COLOR_LABEL_HILITE] = Color::Black;
	mMenuButton->SetFont(FONT_NORMAL);
	mMenuButton->mLabel = "Menu";
	mStage1Button = new ButtonWidget(STAGE1_BUTTON_ID, this);
	mStage2Button = new ButtonWidget(STAGE2_BUTTON_ID, this);
	mStage3Button = new ButtonWidget(STAGE3_BUTTON_ID, this);
	mStage4Button = new ButtonWidget(STAGE4_BUTTON_ID, this);
	mStage1Button->mButtonImage = IMAGE_BG_MODERN;
	mStage2Button->mButtonImage = IMAGE_BG_CLASSIC;
	mStage3Button->mButtonImage = IMAGE_BG_OTHER1;
	mStage4Button->mButtonImage = IMAGE_BG_OTHER2;
	mStage1Button->mVisible = true;
	mStage2Button->mVisible = true;
	mStage3Button->mVisible = false;
	mStage4Button->mVisible = false;
	mStage1Button->mDoFinger = true;
	mStage2Button->mDoFinger = true;
	mStage3Button->mDoFinger = true;
	mStage4Button->mDoFinger = true;

	mMenu = NULL;
}

StageSelect::~StageSelect()
{
	delete mMenuButton;
	delete mStage1Button;
	delete mStage2Button;
	delete mStage3Button;
	delete mStage4Button;
}

void StageSelect::Draw(Graphics* g)
{
	Dialog::Draw(g);

	g->SetFont(FONT_NORMAL);
	g->SetColor(Color::Black);

	int y = mContentInsets.mTop + mSpaceAfterHeader + FONT_NORMAL->GetHeight();
	int w = IMAGE_STAGE_FRAME->GetWidth();
	int h = IMAGE_STAGE_FRAME->GetHeight();
	g->DrawImage(IMAGE_STAGE_FRAME, mContentInsets.mLeft, y);
	g->DrawImage(IMAGE_STAGE_FRAME, mWidth - mContentInsets.mRight - w, y);
	g->DrawImage(IMAGE_STAGE_FRAME, mContentInsets.mLeft, y + h + 25);
	g->DrawImage(IMAGE_STAGE_FRAME, mWidth - mContentInsets.mRight - w, y + h + 25);
}

void StageSelect::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);

	int y = mContentInsets.mTop + mSpaceAfterHeader + FONT_NORMAL->GetHeight() + 10;
	int w = 200;
	int h = 150;

	mMenuButton->Resize(theX + mWidth - 150, theY + 10, 88, IMAGE_DIALOG_BTN->GetHeight());
	mStage1Button->Resize(theX + mContentInsets.mLeft + 10, theY + y, w, h);
	mStage2Button->Resize(theX + mWidth - mContentInsets.mRight - w - 10, theY + y, w, h);
	mStage3Button->Resize(theX + mContentInsets.mLeft + 10, theY + y + h + 25 + 20, w, h);
	mStage4Button->Resize(theX + mWidth - mContentInsets.mRight - w - 10, theY + y + h + 25 + 20, w, h);
}

void StageSelect::Update()
{
	Dialog::Update();
}

void StageSelect::AddedToManager(WidgetManager* theWidgetManager)
{
	Dialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mStage1Button);
	theWidgetManager->AddWidget(mStage2Button);
	theWidgetManager->AddWidget(mStage3Button);
	theWidgetManager->AddWidget(mStage4Button);
}

void StageSelect::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Dialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mStage1Button);
	theWidgetManager->RemoveWidget(mStage2Button);
	theWidgetManager->RemoveWidget(mStage3Button);
	theWidgetManager->RemoveWidget(mStage4Button);
}

void StageSelect::ButtonDepress(int theId)
{
	Dialog::ButtonDepress(theId);

	GSET set = GSET_COUNT;
	switch (theId) {
		case MENU_BUTTON_ID:
			gSexyAppBase->KillDialog(this);
			break;
		case StageSelect::STAGE1_BUTTON_ID:	set = GSET_MODERN;	break;
		case StageSelect::STAGE2_BUTTON_ID:	set = GSET_CLASSIC;	break;
		case StageSelect::STAGE3_BUTTON_ID:	set = GSET_OTHER1;	break;
		case StageSelect::STAGE4_BUTTON_ID:	set = GSET_OTHER2;	break;
	}
	if (set != GSET_COUNT) {
		mMenu->Quickplay(set);
	}
}