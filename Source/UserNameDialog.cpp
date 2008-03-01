/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "Strings.h"
#include "GameApp.h"
#include "Game.h"
#include "UserDialog.h"
#include "UserNameDialog.h"
#include "MessageDialog.h"
#include "User.h"
#include "Res.h"
#include "GameApp.h"
#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/EditWidget.h"
#include "SexyAppFramework/ScrollbarWidget.h"

using namespace Sexy;

UserNameDialog::UserNameDialog(SexyString userName) :
Dialog(IMAGE_DIALOG, IMAGE_DIALOG_BTN, DIALOG_ID, true, SZ_NEW_USER, SZ_NULL, SZ_NULL, Dialog::BUTTONS_NONE)
{
	mContentInsets = Insets(40, 10, 45, 40);
	mSpaceAfterHeader = 10;

	SetHeaderFont(FONT_NORMAL);
	SetLinesFont(FONT_NORMAL);
	SetButtonFont(FONT_NORMAL);

	SetColor(COLOR_HEADER, Color::Black);
	SetColor(COLOR_LINES, Color::Black);

	mNameTextbox = new EditWidget(NAME_TEXTBOX_ID, this);
	mNameTextbox->mFont = FONT_NORMAL->Duplicate();
	mNameTextbox->mMaxChars = 15;
	mNameTextbox->mColors[EditWidget::COLOR_BKG] = Color(115, 0, 0);
	mNameTextbox->mColors[EditWidget::COLOR_TEXT] = Color(255, 255, 0);
	mNameTextbox->mColors[EditWidget::COLOR_HILITE] = Color(255, 0, 255);
	mNameTextbox->mColors[EditWidget::COLOR_HILITE_TEXT] = Color::White;

	mCancelButton = new DialogButton(IMAGE_DIALOG_BTN, CANCEL_BUTTON_ID, this);
	mCancelButton->mLabel = SZ_CANCEL;
	mCancelButton->SetFont(FONT_NORMAL);
	mCancelButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
	mCancelButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);

	mOKButton = new DialogButton(IMAGE_DIALOG_BTN, OK_BUTTON_ID, this);
	mOKButton->mLabel = SZ_OK;
	mOKButton->SetFont(FONT_NORMAL);
	mOKButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
	mOKButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);

	if (userName.length() > 0) {
		mDialogHeader = SZ_RENAME_USER;
		mNameTextbox->SetText(userName);
	}
}

UserNameDialog::~UserNameDialog()
{
	delete mNameTextbox;
	delete mCancelButton;
	delete mOKButton;
}

bool UserNameDialog::ValidateUserName()
{
	SexyString name = mNameTextbox->mString;
	std::transform(name.begin(), name.end(), name.begin(), toupper);

	//Check if a name was entered
	if (name.length() == 0) {
		//Must have a name
		MessageDialog *msgbox = new MessageDialog(20, SZ_ENTER_YOUR_NAME, SZ_MUST_ENTER_YOUR_NAME,
			SZ_OK, SZ_NULL, MessageDialog::MSGBOX_SINGLE);
		msgbox->Resize(mApp);
		mApp->AddDialog(msgbox->mId, msgbox);
		msgbox->WaitForResult(true);

		return false;
	}

	//Check if name already exists
	for (int i = 0; i < mDisallowedNamesCount; i++) {
		SexyString dn = mDisallowedNames[i];
		std::transform(dn.begin(), dn.end(), dn.begin(), toupper);
		if (dn == name) {
			//Name already exists
			MessageDialog *msgbox = new MessageDialog(21, SZ_ENTER_YOUR_NAME, SZ_NAME_ALREADY_EXISTS,
				SZ_OK, SZ_NULL, MessageDialog::MSGBOX_SINGLE);
			msgbox->Resize(mApp);
			mApp->AddDialog(msgbox->mId, msgbox);
			msgbox->WaitForResult(true);

			return false;
		}
	}

	return true;
}

void UserNameDialog::Draw(Graphics* g)
{
	Dialog::Draw(g);

	g->SetFont(FONT_NORMAL);
	g->SetColor(Color::Black);
	g->DrawString(SZ_PLEASE_ENTER_YOUR_NAME,
		(mWidth / 2) - (FONT_NORMAL->StringWidth(SZ_PLEASE_ENTER_YOUR_NAME) / 2),
		mContentInsets.mTop + mSpaceAfterHeader + FONT_NORMAL->GetHeight() + 10);
}

void UserNameDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);

	int x = theX + mContentInsets.mLeft;
	int w = mWidth - mContentInsets.mLeft - mContentInsets.mRight;
	int y = theY + mContentInsets.mTop + mSpaceAfterHeader + FONT_NORMAL->GetHeight() + FONT_NORMAL->GetHeight();
	mOKButton->Resize(x, theY + mHeight - mContentInsets.mBottom - IMAGE_DIALOG_BTN->GetHeight(), w / 2 - 10, IMAGE_DIALOG_BTN->GetHeight());
	mCancelButton->Layout(LAY_Right | LAY_SameTop | LAY_SameSize, mOKButton, 20, 0, 0, 0);

	mNameTextbox->Resize(x, y, w, FONT_NORMAL->GetHeight());
}

void UserNameDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	Dialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mNameTextbox);
	theWidgetManager->AddWidget(mCancelButton);
	theWidgetManager->AddWidget(mOKButton);

	//Set focus to the name textbox
	theWidgetManager->SetFocus(mNameTextbox);
}

void UserNameDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Dialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mNameTextbox);
	theWidgetManager->RemoveWidget(mCancelButton);
	theWidgetManager->RemoveWidget(mOKButton);
}

void UserNameDialog::ButtonDepress(int theId)
{
	Dialog::ButtonDepress(theId);
	if (theId == OK_BUTTON_ID) {
		if (ValidateUserName()) {
			mUserName = mNameTextbox->mString;
		} else {
			return;
		}	
	}

	mResult = theId;
}