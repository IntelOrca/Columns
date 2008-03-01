/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef GAME_H_
#define GAME_H_

#include <SexyAppFramework/SexyAppBase.h>
#include <SexyAppFramework/Widget.h>

#include "Res.h"
#include "ResultsScreen.h"
#include "GameData.h"

namespace Sexy
{
	class SoundInstance;
	class ButtonWidget;
	class GameApp;
	class Board;
	class Monster;
	struct GameResults;
	class Game : public Widget, ButtonListener
	{
		private:
			//Widgets
			ButtonWidget *mMenuButton;
			Monster *mMonster;

			SoundInstance *mGss;

			bool mStartedMusic;
			bool mGameOver;

			enum WIDGET_ID
			{
				WIDGET_MENU_BUTTON,
				WIDGET_MONSTER,
			};
		public:
			GameApp		*mApp;

			bool mLoadingSave;
			GameData *mData;

		public:
			Game(GameApp* theApp);
			~Game();
			void Init(GAME_MODE mode, GameResults results);
			void GetSet();
			void Play();
			int FindWinner();
			void PlayerEnded();
			void EndGame();
			void MainMenu();
			void ShowMenu();
			void CloseMenu();
			void Pause();
			void PlayMusic();
			void ResumeMusic();
			void PauseMusic();
			void StopMusic();

			void PlayASound(GSET_RESOURCE s);
			void PlayASound(int s);
			void PlayStrikeSound(int currentStrike);

			void DialogButtonDepress(int theDialogId, int theButtonId);
			void ButtonDepress(int theId);
			void AddedToManager(WidgetManager *theWidgetManager);
			void RemovedFromManager(WidgetManager *theWidgetManager);
			void KeyDown(KeyCode theKey);
			void KeyUp(KeyCode theKey);
			void Draw(Graphics* g);
			void Update();
			void DrawScore(int player, Graphics* g);
			void DrawNPBorder(int player, Graphics* g);
			void DrawBoardBorder(int p, Graphics* g);
			void DrawTime(Graphics* g);
			void DrawASBorder(int p, Graphics *g);
			void DrawOpponent(Graphics *g);
			void InitGUI();
			void InitBoard(int player, BoardPositions pos, AI_TYPE aitype);
			void SetSideBoards();
			bool SaveGame();
			bool LoadGame(GAME_MODE mode);

			void ShowResults();
	};
}

#endif