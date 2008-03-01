/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef USERDIALOG_H_
#define USERDIALOG_H_

#include "SexyAppFramework/Dialog.h"
#include "SexyAppFramework/ListListener.h"
#include "SexyAppFramework/ScrollListener.h"

namespace Sexy
{
	class GameApp;
	class Graphics;
	class DialogButton;
	class ListWidget;
	class ScrollbarWidget;
	class ScrollbuttonWidget;
	class UserNameDialog;
	class User;
	class UserDialog : public Dialog, ListListener, ScrollListener
	{
	private:
		int usercount;
		User *userlist;

	public:
		static const int DIALOG_ID = 1;
		enum WIDGET_ID
		{
			USERSCROLL_SCROLLBAR_ID,
			USERSCROLLU_SCROLLBUTTON_ID,
			USERSCROLLD_SCROLLBUTTON_ID,
			USER_LIST_ID,
			RENAME_BUTTON_ID,
			DELETE_BUTTON_ID,
			CANCEL_BUTTON_ID,
			OK_BUTTON_ID,
		};

		GameApp *mApp;
		ListWidget *mUserList;
		ScrollbuttonWidget *mListScrollbuttonU;
		ScrollbuttonWidget *mListScrollbuttonD;
		ScrollbarWidget *mListScroll;
		DialogButton *mRenameButton;
		DialogButton *mDeleteButton;
		DialogButton *mCancelButton;
		DialogButton *mOKButton;

		UserDialog(std::string theHeader, std::string theBody);
		~UserDialog();

		User *SelectedUser();
		void AddUser(SexyString sname);
		void RemoveUser(int id);
		void SelectUser(SexyString sname);
		void RenameSelectedUser(SexyString sname);
		void GetUsersFromDirectory();
		void RefreshList();

		void NewUserDialog();
		void RenameUserDialog();

		void Resize(int theX, int theY, int theWidth, int theHeight);
		void AddedToManager(WidgetManager* theWidgetManager);
		void RemovedFromManager(WidgetManager* theWidgetManager);
		void ListClicked(int theId, int theIdx, int theClickCount);
		void ButtonDepress(int theId);
		void ButtonDownTick(int theId);
		void ScrollPosition(int theId, double thePosition);
	};
}

#endif