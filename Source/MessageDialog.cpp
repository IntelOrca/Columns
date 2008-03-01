/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "Strings.h"
#include "MessageDialog.h"
#include "Res.h"
#include "GameApp.h"
#include "Game.h"
#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/DialogButton.h"

using namespace Sexy;

MessageDialog::MessageDialog(int id, const SexyString& title, const SexyString& content,
		   const SexyString& leftButton, const SexyString& rightButton, int msgboxType) :
Dialog(IMAGE_DIALOG, IMAGE_DIALOG_BTN, id, true, title, content, SZ_NULL, Dialog::BUTTONS_NONE)
{
	mContentInsets = Insets(40, 10, 45, 40);
	mSpaceAfterHeader = 10;
	SetHeaderFont(FONT_NORMAL);
	SetLinesFont(FONT_NORMAL);
	SetButtonFont(FONT_NORMAL);

	SetColor(COLOR_HEADER, Color::Black);
	SetColor(COLOR_LINES, Color::Black);
	SetColor(COLOR_BUTTON_TEXT, Color::Black);
	SetColor(COLOR_BUTTON_TEXT_HILITE, Color::Black);

	//Left button
	mLeftButton = new DialogButton(IMAGE_DIALOG_BTN, ID_LEFT, this);
	mLeftButton->SetFont(FONT_NORMAL);
	mLeftButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
	mLeftButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);
	mLeftButton->mLabel = leftButton;

	if (msgboxType == MSGBOX_LEFT_RIGHT) {
		//Right button
		mRightButton = new DialogButton(IMAGE_DIALOG_BTN, ID_RIGHT, this);
		mRightButton->SetFont(FONT_NORMAL);
		mRightButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
		mRightButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);
		mRightButton->mLabel = rightButton;
	} else {
		mRightButton = NULL;
	}
}

MessageDialog::~MessageDialog()
{
	delete mLeftButton;
	delete mRightButton;
}

int	MessageDialog::GetPreferredHeight(int theWidth)
{
	EnsureFonts();

	int aHeight = mContentInsets.mTop + mContentInsets.mBottom + mBackgroundInsets.mTop + mBackgroundInsets.mBottom;

	bool needSpace = false;
	if (mDialogHeader.length() > 0)
	{
		aHeight += mHeaderFont->GetHeight() - mHeaderFont->GetAscentPadding();
		needSpace = true;
	}
	
	if (mDialogLines.length() > 0)
	{
		if (needSpace)
			aHeight += mSpaceAfterHeader;
		Graphics g;
		g.SetFont(mLinesFont);	
		aHeight += GetWordWrappedHeight(&g, theWidth-mContentInsets.mLeft-mContentInsets.mRight-mBackgroundInsets.mLeft-mBackgroundInsets.mRight-4, mDialogLines, mLinesFont->GetLineSpacing() + mLineSpacingOffset);
		needSpace = true;
	}

	if ((mDialogFooter.length() != 0) && (mButtonMode != BUTTONS_FOOTER))
	{
		if (needSpace)
			aHeight += 8;
		aHeight += mHeaderFont->GetLineSpacing();
		needSpace = true;
	}

	if (mLeftButton != NULL)
	{
		if (needSpace)
			aHeight += 8;
		aHeight += mButtonHeight + 8;		
	}	

	return aHeight;
}

void MessageDialog::Resize(GameApp *app)
{
	int aWidth = app->mWidth / 2;
	Resize((app->mWidth - aWidth) / 2, app->mHeight / 5, aWidth, GetPreferredHeight(aWidth));
}

void MessageDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);

	int aBtnWidth = (mWidth - mContentInsets.mLeft - mContentInsets.mRight - mBackgroundInsets.mLeft - mBackgroundInsets.mRight - mButtonSidePadding * 2 - mButtonHorzSpacing) / 2;
	int aBtnHeight = mButtonHeight;

	if (mRightButton != NULL) {
		mLeftButton->Resize(mX + mBackgroundInsets.mLeft + mContentInsets.mLeft + mButtonSidePadding, mY + mHeight - mContentInsets.mBottom - mBackgroundInsets.mBottom - aBtnHeight, aBtnWidth, aBtnHeight);
		mRightButton->Resize(mLeftButton->mX + aBtnWidth + mButtonHorzSpacing, mLeftButton->mY, aBtnWidth, aBtnHeight);
	} else {
		aBtnWidth *= 2;
		mLeftButton->Resize(mX + (mWidth / 2) - (aBtnWidth / 2),
			mY + mHeight - mContentInsets.mBottom - mBackgroundInsets.mBottom - aBtnHeight, aBtnWidth, aBtnHeight);
	}
}

void MessageDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	Dialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mLeftButton);

	if (mRightButton != NULL) {
		theWidgetManager->AddWidget(mRightButton);
	}
}

void MessageDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Dialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mLeftButton);

	if (mRightButton != NULL) {
		theWidgetManager->RemoveWidget(mRightButton);
	}
}