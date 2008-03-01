/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef USERNAMEDIALOG_H_
#define USERNAMEDIALOG_H_

#include "SexyAppFramework/Dialog.h"
#include "SexyAppFramework/EditListener.h"

namespace Sexy
{
	class GameApp;
	class Graphics;
	class DialogButton;
	class EditWidget;
	class UserDialog;
	class UserNameDialog : public Dialog, EditListener
	{
	private:
		UserDialog *mUserDialog;
		EditWidget *mNameTextbox;
		DialogButton *mCancelButton;
		DialogButton *mOKButton;

	public:
		//Dialog ID
		static const int DIALOG_ID = 2;

		//Widget IDs
		enum WIDGET_ID
		{
			NAME_TEXTBOX_ID,
			CANCEL_BUTTON_ID,
			OK_BUTTON_ID,
		};

		//GameApp
		GameApp *mApp;

		//Disallowed user names
		int mDisallowedNamesCount;
		SexyString *mDisallowedNames;

		//User name
		SexyString mUserName;

		//Methods
		UserNameDialog(SexyString userName);
		~UserNameDialog();

		bool ValidateUserName();

		void Draw(Graphics* g);

		void Resize(int theX, int theY, int theWidth, int theHeight);
		void AddedToManager(WidgetManager* theWidgetManager);
		void RemovedFromManager(WidgetManager* theWidgetManager);
		void ButtonDepress(int theId);
	};
}

#endif