/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef MESSAGE_DIALOG_H_
#define MESSAGE_DIALOG_H_

#include "SexyAppFramework/Dialog.h"

namespace Sexy
{
	class GameApp;
	class Game;
	class Graphics;
	class DialogButton;
	class MessageDialog : public Dialog
	{
	public:
		enum
		{
			MSGBOX_SINGLE,
			MSGBOX_LEFT_RIGHT,
		};

		enum
		{
			ID_SINGLE	= 1000,
			ID_LEFT		= 1000,
			ID_RIGHT	= 1001,
		};

	private:
		DialogButton	*mLeftButton;			//Left button
		DialogButton	*mRightButton;			//Right button
	public:
		MessageDialog(int id, const SexyString& title, const SexyString& content,
			const SexyString& leftButton, const SexyString& rightButton, int msgboxType);
		~MessageDialog();
		int GetPreferredHeight(int theWidth);
		void Resize(GameApp *app);
		void Resize(int theX, int theY, int theWidth, int theHeight);
		void AddedToManager(WidgetManager* theWidgetManager);
		void RemovedFromManager(WidgetManager* theWidgetManager);
	};
}

#endif