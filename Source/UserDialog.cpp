/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "Strings.h"
#include "GameApp.h"
#include "Game.h"
#include "MessageDialog.h"
#include "UserDialog.h"
#include "UserNameDialog.h"
#include "User.h"
#include "Res.h"
#include "GameApp.h"
#include "SexyAppFramework/SexyAppBase.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/DialogButton.h"
#include "SexyAppFramework/ListWidget.h"
#include "SexyAppFramework/EditWidget.h"
#include "SexyAppFramework/ScrollbarWidget.h"
#include "SexyAppFramework/ScrollbuttonWidget.h"

using namespace Sexy;

UserDialog::UserDialog(std::string theHeader, std::string theBody) :
Dialog(IMAGE_DIALOG, IMAGE_DIALOG_BTN, UserDialog::DIALOG_ID, true, StringToSexyStringFast(theHeader), StringToSexyStringFast(theBody), "", Dialog::BUTTONS_NONE)
{
	usercount = 0;
	userlist = NULL;

	mContentInsets = Insets(40, 10, 45, 40);
	mSpaceAfterHeader = 10;
	SetHeaderFont(FONT_NORMAL);
	SetLinesFont(FONT_NORMAL);
	SetButtonFont(FONT_NORMAL);

	SetColor(COLOR_HEADER, Color::Black);
	SetColor(COLOR_LINES, Color::Black);

	mListScrollbuttonU = new ScrollbuttonWidget(USERSCROLLU_SCROLLBUTTON_ID, this, 1);
	mListScrollbuttonD = new ScrollbuttonWidget(USERSCROLLD_SCROLLBUTTON_ID, this, 0);
	mListScroll = new ScrollbarWidget(USERSCROLL_SCROLLBAR_ID, this);
	mListScroll->mUpButton = mListScrollbuttonU;
	mListScroll->mDownButton = mListScrollbuttonD;

	mUserList = new ListWidget(USER_LIST_ID, FONT_NORMAL, this);
	mUserList->mScrollbar = mListScroll;
	mUserList->mJustify = ListWidget::JUSTIFY_CENTER;
	mUserList->mItemHeight = FONT_NORMAL->GetHeight() - 10;
	mUserList->mColors[ListWidget::COLOR_BKG] = Color(115, 0, 0);
	mUserList->mColors[ListWidget::COLOR_OUTLINE] = Color::Black;
	mUserList->mColors[ListWidget::COLOR_TEXT] = Color(255, 255, 0);
	mUserList->mColors[ListWidget::COLOR_HILITE] = Color(255, 255, 255);
	mUserList->mColors[ListWidget::COLOR_SELECT] = Color(255, 0, 255);
	mUserList->mColors[ListWidget::COLOR_SELECT_TEXT] = Color(255, 255, 0);
	mUserList->AddLine(SZ_CREATE_NEW_USER, false);

	RefreshList();

	mRenameButton = new DialogButton(IMAGE_DIALOG_BTN, RENAME_BUTTON_ID, this);
	mRenameButton->mLabel = SZ_RENAME;
	mRenameButton->SetFont(FONT_NORMAL);
	mRenameButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
	mRenameButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);

	mDeleteButton = new DialogButton(IMAGE_DIALOG_BTN, DELETE_BUTTON_ID, this);
	mDeleteButton->mLabel = SZ_DELETE;
	mDeleteButton->SetFont(FONT_NORMAL);
	mDeleteButton->SetColor(DialogButton::COLOR_LABEL, Color::Black);
	mDeleteButton->SetColor(DialogButton::COLOR_LABEL_HILITE, Color::Black);

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
}

UserDialog::~UserDialog()
{
	delete[] userlist;
	delete mListScroll;
	delete mUserList;
	delete mRenameButton;
	delete mDeleteButton;
	delete mCancelButton;
	delete mOKButton;
}

User *UserDialog::SelectedUser()
{
	if (mUserList->mLines.size() > 1)
		return &userlist[mUserList->mSelectIdx - 1];
	else
		return NULL;
}

void UserDialog::AddUser(SexyString sname)
{
	//Convert string to c-string
	char *name = new char[sname.length()];
	strcpy(name, sname.c_str());

	//Check if name is unique
	for (int i = 0; i < usercount; i++) {
		if (strcmp(userlist[i].mName, name) == 0) {
			//Name already in use
		}
	}

	//Create the new user
	User *u = new User(mApp, name, true);
	for (int i = 1; i < 255; i++) {
		if (i == 255) {
			//Too many users
		} else {
			char filename[MAX_PATH]; sprintf(filename, PATH_USER_FILE, i);
			if (GetFileAttributes(filename) == INVALID_FILE_ATTRIBUTES) {
				u->Save(filename);
				break;
			}
		}
	}
}

void UserDialog::RemoveUser(int id)
{
	//Check if a user is selected
	if (SelectedUser() == NULL)
		return;

	if (!DeleteFile(userlist[id].mFilename)) {
		//Can't remove user
	}
}

void UserDialog::SelectUser(SexyString sname)
{
	//Convert string to c-string
	char *name = new char[sname.length()];
	strcpy(name, sname.c_str());

	for (int i = 0; i < usercount; i++) {
		if (strcmp(name, userlist[i].mName) == 0) {
			mUserList->SetSelect(i + 1);
			break;
		}
	}
}

void UserDialog::RenameSelectedUser(SexyString sname)
{
	//Convert string to c-string
	char *name = new char[sname.length()];
	strcpy(name, sname.c_str());

	User *usr = SelectedUser();
	usr->mName = name;
	usr->Save();
}

void UserDialog::GetUsersFromDirectory()
{
	char szFiles[400];
	WIN32_FIND_DATA ffd;
	TCHAR szDir[MAX_PATH] = PATH_USER_FILES;
	HANDLE hFind = INVALID_HANDLE_VALUE;
	DWORD dwError=0;

	usercount = 0;

	//Count number of users
	hFind = FindFirstFile(szDir, &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		usercount++;
		while (FindNextFile(hFind, &ffd) != 0) {
			usercount++;
		}
	}

	userlist = new User[usercount];

	//Load users
	int i = 0;
	hFind = FindFirstFile(szDir, &ffd);
	if (hFind != INVALID_HANDLE_VALUE) {
		char filename[MAX_PATH]; sprintf(filename, "%s%s", PATH_USERS, ffd.cFileName);			
		userlist[i].Load(filename);
		i++;
		for (i = i; i < usercount; i++) {
			if (FindNextFile(hFind, &ffd) != 0) {
				char filename[MAX_PATH]; sprintf(filename, "%s%s", PATH_USERS, ffd.cFileName);			
				if (!userlist[i].Load(filename)) {
					i--;
					usercount--;
				}
			}
		}
	}
	FindClose(hFind);
}

void UserDialog::RefreshList()
{
	GetUsersFromDirectory();

	mUserList->RemoveAll();
	mUserList->AddLine(SZ_CLICK_TO_ADD_NEW_USER, false);
	for (int i = 0; i < usercount; i++) {
		if (userlist[i].mSuccessfulLoad) {
			mUserList->AddLine(userlist[i].mName, false);
		}
	}
	mUserList->SetSelect(1);
}

void UserDialog::NewUserDialog()
{
	UserNameDialog *dlg;
	int result;

	//Create the dialog and show it
	dlg = new UserNameDialog(SZ_NULL);
	dlg->mApp = mApp;
	//Disallowed names
	dlg->mDisallowedNamesCount = usercount;
	dlg->mDisallowedNames = new SexyString[usercount];
	for (int i = 0; i < usercount; i++) {
		dlg->mDisallowedNames[i] = userlist[i].mName;
	}

	dlg->Resize((mApp->mWidth / 2) - (350 / 2), mApp->mHeight / 5, 350, 200);
	mApp->AddDialog(UserNameDialog::DIALOG_ID, dlg);

	//When dialog finishes, check result
	result = dlg->WaitForResult(true);
	if (result == UserNameDialog::OK_BUTTON_ID) {
		//Add the new user
		AddUser(dlg->mUserName);

		RefreshList();
		SelectUser(dlg->mUserName);
	}
}

void UserDialog::RenameUserDialog()
{
	UserNameDialog *dlg;
	int result;

	//Check if a user is selected
	if (SelectedUser() == NULL)
		return;

	//Create the dialog and show it
	dlg = new UserNameDialog(SelectedUser()->mName);
	dlg->Resize((mApp->mWidth / 2) - (350 / 2), mApp->mHeight / 5, 350, 200);
	mApp->AddDialog(UserNameDialog::DIALOG_ID, dlg);

	//When dialog finishes, check result
	result = dlg->WaitForResult(true);
	if (result == UserNameDialog::OK_BUTTON_ID) {
		//Rename the selected user
		RenameSelectedUser(dlg->mUserName);

		RefreshList();
		SelectUser(dlg->mUserName);
	}
}

void UserDialog::Resize(int theX, int theY, int theWidth, int theHeight)
{
	Dialog::Resize(theX, theY, theWidth, theHeight);

	int x = theX + mContentInsets.mLeft;
	int w = mWidth - mContentInsets.mLeft - mContentInsets.mRight;
	int y = theY + mContentInsets.mTop + mSpaceAfterHeader + FONT_NORMAL->GetHeight();
	mOKButton->Resize(x, theY + mHeight - mContentInsets.mBottom - IMAGE_DIALOG_BTN->GetHeight(), w / 2 - 10, IMAGE_DIALOG_BTN->GetHeight());
	mCancelButton->Layout(LAY_Right | LAY_SameTop | LAY_SameSize, mOKButton, 20, 0, 0, 0);
	mRenameButton->Layout(LAY_Above | LAY_SameLeft | LAY_SameSize, mOKButton, 0, -10, 0, 0);
	mDeleteButton->Layout(LAY_Above | LAY_SameLeft | LAY_SameSize, mCancelButton, 0, -10, 0, 0);
	
	int h = mRenameButton->mY -theY - 10 - mContentInsets.mTop - mSpaceAfterHeader - FONT_NORMAL->GetHeight();
	mUserList->Resize(x, y, w - 16, h);
	mListScroll->Layout(LAY_SameTop | LAY_Right | LAY_SameHeight | LAY_SetWidth, mUserList, 0, 0, 16, 0);
	mListScrollbuttonU->Layout(LAY_SameTop | LAY_SameLeft | LAY_SameWidth | LAY_SetHeight, mListScroll, 0, 0, 0, 16);
	mListScrollbuttonD->Layout(LAY_SameTop | LAY_SameLeft | LAY_SameWidth | LAY_SetHeight, mListScroll, 0, mListScroll->mHeight - 16, 0, 16);
}

void UserDialog::AddedToManager(WidgetManager* theWidgetManager)
{
	Dialog::AddedToManager(theWidgetManager);
	theWidgetManager->AddWidget(mUserList);
	theWidgetManager->AddWidget(mListScroll);
	theWidgetManager->AddWidget(mListScrollbuttonU);
	theWidgetManager->AddWidget(mListScrollbuttonD);
	theWidgetManager->AddWidget(mRenameButton);
	theWidgetManager->AddWidget(mDeleteButton);
	theWidgetManager->AddWidget(mCancelButton);
	theWidgetManager->AddWidget(mOKButton);

	//Select current user
	if (mApp->mCurrentUser != NULL)
		SelectUser(mApp->mCurrentUser->mName);
}

void UserDialog::RemovedFromManager(WidgetManager* theWidgetManager)
{
	Dialog::RemovedFromManager(theWidgetManager);
	theWidgetManager->RemoveWidget(mListScrollbuttonU);
	theWidgetManager->RemoveWidget(mListScrollbuttonD);
	theWidgetManager->RemoveWidget(mListScroll);
	theWidgetManager->RemoveWidget(mUserList);
	theWidgetManager->RemoveWidget(mRenameButton);
	theWidgetManager->RemoveWidget(mDeleteButton);
	theWidgetManager->RemoveWidget(mCancelButton);
	theWidgetManager->RemoveWidget(mOKButton);
}

void UserDialog::ListClicked(int theId, int theIdx, int theClickCount)
{
	if (theIdx > 0) {
		mUserList->SetSelect(theIdx);
	} else {
		//New User
		NewUserDialog();
	}
}

void UserDialog::ButtonDepress(int theId)
{
	Dialog::ButtonDepress(theId);
	switch (theId) {
		case RENAME_BUTTON_ID:
			RenameUserDialog();
			break;
		case DELETE_BUTTON_ID:
			RemoveUser(mUserList->mSelectIdx - 1);
			RefreshList();
			break;
		case CANCEL_BUTTON_ID:
			if (SelectedUser() == NULL || mApp->mCurrentUser == NULL) {
				MessageDialog *msgbox = new MessageDialog(60, SZ_WHO_ARE_YOU, SZ_MUST_SELECT_USER,
					SZ_OK, SZ_NULL, MessageDialog::MSGBOX_SINGLE);
				msgbox->Resize(mApp);
				mApp->AddDialog(msgbox);
				msgbox->WaitForResult(true);
			} else {
				gSexyAppBase->KillDialog(this);
			}
			break;
		case OK_BUTTON_ID:
			if (SelectedUser() == NULL) {
				MessageDialog *msgbox = new MessageDialog(60, SZ_WHO_ARE_YOU, SZ_MUST_SELECT_USER,
					SZ_OK, SZ_NULL, MessageDialog::MSGBOX_SINGLE);
				msgbox->Resize(mApp);
				mApp->AddDialog(msgbox);
				msgbox->WaitForResult(true);
			} else {
				mApp->SetCurrentUser(new User(mApp, SelectedUser()->mFilename, false));
				gSexyAppBase->KillDialog(this);
			}
			break;
	}
}

void UserDialog::ButtonDownTick(int theId)
{
	Dialog::ButtonDownTick(theId);
	switch (theId) {
		case USERSCROLLU_SCROLLBUTTON_ID:
			mListScroll->SetThumbPosition(mListScroll->GetThumbPosition() - 1);
			break;
		case USERSCROLLD_SCROLLBUTTON_ID:
			mListScroll->SetThumbPosition(mListScroll->GetThumbPosition() + 1);
			break;
	}
}

void UserDialog::ScrollPosition(int theId, double thePosition)
{
	mUserList->ScrollPosition(theId, thePosition);
}

