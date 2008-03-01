/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework/ButtonListener.h"
#include "SexyAppFramework/ButtonWidget.h"
#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/BassMusicInterface.h"
#include "SexyAppFramework/DSoundManager.h"
#include "SexyAppFramework/DSoundInstance.h"

#include "Monster.h"

#include "Grid.h"
#include "Strings.h"
#include "Core.h"
#include "Game.h"
#include "Res.h"
#include "User.h"
#include "GameApp.h"
#include "ResultsScreen.h"
#include "OptionsDialog.h"
#include "Highscore.h"

using namespace Sexy;

#pragma region Constructor/Destructor

Game::Game(GameApp* theApp)
{
	mApp = theApp;

	mStartedMusic = false;
	mGameOver = false;

	mMenuButton = NULL;
	mMonster = NULL;
}

/* Delete any things that
 * were initilized on startup
 */
Game::~Game()
{
	delete mMenuButton;
	delete mMonster;

	//Delete each board
	for (int i = 0; i < 3; i++) {
		if (mData->mPlayer[i] != NULL)
			delete mData->mPlayer[i];
	}

	delete mData;
}

#pragma endregion

#pragma region Input

bool keyControl = false;
void Game::KeyDown(KeyCode theKey)
{
	if (!mData->mPaused) {
		if (mData->mStdkeys[0] != NULL) {
			switch (theKey) {
				case KEYCODE_RETURN:	mData->mStdkeys[0]->Controller(CONTROL_SELECT);			break;
				case KEYCODE_LEFT:		mData->mStdkeys[0]->Controller(CONTROL_LEFT);			break;
				case KEYCODE_RIGHT:		mData->mStdkeys[0]->Controller(CONTROL_RIGHT);			break;
				case KEYCODE_UP:		mData->mStdkeys[0]->Controller(CONTROL_CHANGE);			break;
				case KEYCODE_DOWN:		mData->mStdkeys[0]->Controller(CONTROL_DOWN_START);		break;
				case 'Q':				mData->mStdkeys[0]->Controller(CONTROL_ATTACK_LEFT);	break;
			}
		}
		if (mData->mStdkeys[1] != NULL) {
			switch (theKey) {
				case 'A':	mData->mStdkeys[1]->Controller(CONTROL_LEFT);		break;
				case 'D':	mData->mStdkeys[1]->Controller(CONTROL_RIGHT);		break;
				case 'W':	mData->mStdkeys[1]->Controller(CONTROL_CHANGE);		break;
				case 'S':	mData->mStdkeys[1]->Controller(CONTROL_DOWN_START);	break;
			}
		}
	}
	if (theKey == KEYCODE_RETURN && mGameOver) {
		ShowResults();
	}

	if (theKey == KEYCODE_CONTROL)
		keyControl = true;
	MarkDirty();
}

void Game::KeyUp(KeyCode theKey)
{
	if (theKey == KEYCODE_CONTROL) {
		keyControl = false;
	}
	if (keyControl) {
		if (theKey == 'S') {
			#ifdef _DEBUG
				mData->mStdkeys[0]->mPrescore += 1000;
				mData->mStdkeys[0]->mScore += 1000;
				mData->mStdkeys[0]->mDestScore += 1000;
			#endif
		} else if (theKey == 'L') {
			//End game
			EndGame();
		}
	} else {
		if (theKey == 'P' || theKey == ' ')
			Pause();
		if (!mData->mPaused) {
			if (mData->mStdkeys[0] != NULL) {
				switch (theKey) {
					case KEYCODE_DOWN:			mData->mStdkeys[0]->Controller(CONTROL_DOWN_END);	break;
				}
			}
			if (mData->mStdkeys[1] != NULL) {
				switch (theKey) {
					case 'S':					mData->mStdkeys[1]->Controller(CONTROL_DOWN_END);	break;
				}
			}
		}
	}
	MarkDirty();
}

#pragma endregion

#pragma region GUI

void Game::InitGUI()
{
	//Initialize the Menu Button
	mMenuButton = new ButtonWidget(WIDGET_MENU_BUTTON, this);
	mMenuButton->mButtonImage = IMAGE_BTN_MENU;
	mMenuButton->Resize(682, 11, IMAGE_BTN_MENU->mWidth, IMAGE_BTN_MENU->mHeight);
	mMenuButton->mDoFinger = true;

	//Initialize the monster
	mMonster = new Monster(WIDGET_MONSTER);
	mMonster->Resize(340, 370, 116, 116);
	mMonster->mMonsterImage = IMAGE_MONST_1;
}

void Game::AddedToManager(WidgetManager *theWidgetManager)
{
	Widget::AddedToManager(theWidgetManager);

	//Add the menu button
	theWidgetManager->AddWidget(mMenuButton);
	theWidgetManager->AddWidget(mMonster);

	//Add the boards
	for (int p = 0; p < mData->mPlayers; p++) {
		theWidgetManager->AddWidget(mData->mPlayer[p]);
	}
}

void Game::RemovedFromManager(WidgetManager *theWidgetManager)
{
	Widget::RemovedFromManager(theWidgetManager);

	//Remove the menu button
	theWidgetManager->RemoveWidget(mMenuButton);
	theWidgetManager->RemoveWidget(mMonster);

	//Remove the boards
	for (int p = 0; p < mData->mPlayers; p++) {
		theWidgetManager->RemoveWidget(mData->mPlayer[p]);
	}
}

void Game::ButtonDepress(int theId)
{
	if (theId == WIDGET_MENU_BUTTON) {
		ShowMenu();
	}
}

void Game::ShowMenu()
{
	//Show the menu dialog
	mData->mPaused = true;
	OptionsDialog *dlg = new OptionsDialog(true);
	dlg->Resize(mWidth / 2 - 450 / 2, mHeight / 2 - 320 / 2);
	dlg->mApp = mApp;
	dlg->mGame = this;
	mApp->AddDialog(OptionsDialog::DIALOG_ID, dlg);
}

void Game::CloseMenu()
{
	mData->mPaused = false;
}

void Game::MainMenu()
{
	//Save the game if in progress
	SaveGame();

	StopMusic();
	mApp->CloseGame();
	mApp->ShowMenu();
}

void Game::ShowResults()
{
	mApp->CloseGame();
	mApp->ShowResults(mData->mResults);
}

#pragma endregion

#pragma region Boards and Gameplay

void Game::Init(GAME_MODE mode, GameResults results)
{
	//Initalize the GUI
	InitGUI();

	User *user = mApp->mCurrentUser;

	//Temp place for this
	//Load a game if there is one
	mLoadingSave = LoadGame(mode);

	if (!mLoadingSave) {
		//Create a new game data structure
		mData = new GameData();
		memset(mData, 0, sizeof(GameData));
	}

	//Set the mode and user
	mData->mMode = mode;

	if (mData->mMode == MODE_ADVENTURE) {
		mData->mResults = user->mAdventureResults;
	}

	//Pick a random number for our piece sequence
	mData->mSeed = GetTickCount();

	if (!mLoadingSave) {
		mData->mEnding = false;
	}

	//Null settings
	mData->mStdkeys[0] = NULL;
	mData->mStdkeys[1] = NULL;

	//Set each player and the board's properties
	switch (mode) {
		case MODE_ADVENTURE:
			if (!mLoadingSave) {
				//Number of players
				mData->mPlayers = 2;

				//Level attrubutes
				if (user != NULL) {
					mData->mStage = user->mStage;
					mData->mLevel = user->mLevel;
				}
			}

			//Make board 0 ai for debugging
			#if (AIVSAI == 1)
				InitBoard(0, BoardPoints[mData->mPlayers - 1].p1, AI_TYPE_EXPERIMENT);
				InitBoard(1, BoardPoints[mData->mPlayers - 1].p2, AI_TYPE_EXPERIMENT);
			#else
				InitBoard(0, BoardPoints[mData->mPlayers - 1].p1, AI_TYPE_HUMAN);
				InitBoard(1, BoardPoints[mData->mPlayers - 1].p2, AI_TYPE_EXPERIMENT);
			#endif

			//Set standard keys to board 0
			mData->mStdkeys[0] = mData->mPlayer[0];
			mData->mHumanID = 0;
			break;
		case MODE_QUICKPLAY:
			if (!mLoadingSave) {
				//Number of players
				mData->mPlayers = 1;
			}

			InitBoard(0, BoardPoints[mData->mPlayers - 1].p1, AI_TYPE_HUMAN);

			//Set standard keys to board 0
			mData->mStdkeys[0] = mData->mPlayer[0];
			mData->mHumanID = 0;
			break;
		case MODE_SECRET:
			if (!mLoadingSave) {
				//Number of players
				mData->mPlayers = 3;
			}

			InitBoard(0, BoardPoints[mData->mPlayers - 1].p1, AI_TYPE_EXPERIMENT);
			InitBoard(1, BoardPoints[mData->mPlayers - 1].p2, AI_TYPE_HUMAN);
			InitBoard(2, BoardPoints[mData->mPlayers - 1].p3, AI_TYPE_EXPERIMENT);

			//Set standard keys to board 1
			mData->mStdkeys[0] = mData->mPlayer[1];
			mData->mHumanID = 1;
	}

	//Set the side boards for attacking
	SetSideBoards();

	//Temp - Have this as default set
	mData->mSet = GSET_MODERN;

	if (mLoadingSave) {
		mData->mPaused = !mData->mEnding;
	}

	mData->mPrestart = 0;
}

void Game::InitBoard(int player, BoardPositions pos, AI_TYPE aitype)
{
	Board *board = mData->mPlayer[player];
	if (!mLoadingSave) {
		board = new Board(this, mData, mData->mSeed);
		board->mAIType = aitype;

		//Temp.
		if (mData->mMode == MODE_ADVENTURE)
			board->mColours = 6;
		else
			board->mColours = 4;
	} else {
		//Set the game pointer in the board
		board->mGame = this;
	}

	//Board positions
	board->Resize(pos.boardx, pos.boardy, BOARD_WIDTH * JEWEL_SIZE, VBOARD_HEIGHT * JEWEL_SIZE);
	board->bps = pos;

	if (!mLoadingSave) {
		//If it's a computer set them ready
		if (aitype != AI_TYPE_HUMAN) {
			board->mState = STATE_NULL;
		}
	}

	mData->mPlayer[player] = board;
}

void Game::SetSideBoards()
{
	switch (mData->mPlayers) {
		case 1:
			//Set player 0's sides to NULL
			mData->mPlayer[0]->mLeftBoard = NULL;
			mData->mPlayer[0]->mRightBoard = NULL;
			break;
		case 2:
			//Set player 0's sides to player 1
			mData->mPlayer[0]->mLeftBoard = mData->mPlayer[1];
			mData->mPlayer[0]->mRightBoard = mData->mPlayer[1];

			//Set player 1's sides to player 0
			mData->mPlayer[1]->mLeftBoard = mData->mPlayer[0];
			mData->mPlayer[1]->mRightBoard = mData->mPlayer[0];
			break;
		case 3:
			//Set player 0's left side to player 2 and right side to player 1
			mData->mPlayer[0]->mLeftBoard = mData->mPlayer[2];
			mData->mPlayer[0]->mRightBoard = mData->mPlayer[1];

			//Set player 1's left side to player 0 and right side to player 2
			mData->mPlayer[1]->mLeftBoard = mData->mPlayer[0];
			mData->mPlayer[1]->mRightBoard = mData->mPlayer[2];

			//Set player 2's left side to player 1 and right side to player 0
			mData->mPlayer[2]->mLeftBoard = mData->mPlayer[1];
			mData->mPlayer[2]->mRightBoard = mData->mPlayer[0];
			break;
	}
}

/* GetSet sequence played after everyone
 * has pressed enter.
 */
void Game::GetSet()
{
	//Play the GET SET sound
	mGss = mApp->mSoundManager->GetSoundInstance(SOUND_GETSET);
	mGss->Play(false, true);
}

/* Start the game
 */
void Game::Play()
{
	//Play the music and call the start
	//routine for each player.
	PlayMusic();
	for (int i = 0; i < mData->mPlayers; i++) {
		mData->mPlayer[i]->Start();
	}
	mData->mPlaying = true;
}

void Game::Update()
{
	if (!mData->mPlaying && !mData->mEnding) {
		if (mData->mPrestart == 0) {
			//Check if everyones ready
			bool allready = true;
			for (int i = 0; i < mData->mPlayers; i++) {
				if (mData->mPlayer[i]->mState & STATE_WAITING)
					allready = false;
			}
			if (allready) {
				//Everyones ready
				mData->mPrestart = 1;
				MarkDirty();
			}
		} else if (mData->mPrestart == 2) {
			//Start the getset sequence/music
			mData->mPrestart = 3;
			GetSet();
		} else if (mData->mPrestart == 3) {
			//Wait for getset music to stop
			if (!mGss->IsPlaying()) {
				//Start the game
				mData->mPrestart = 4;
				Play();
			}
		}
	}

	if (mData->mEnding) {
		//Check if all players have lowered their crushbars
		bool allended = true;
		for (int i = 0; i < mData->mPlayers; i++) {
			if (mData->mPlayer[i]->mState != STATE_ENDING) {
				allended = false;
			}
		}

		if (allended) {
			EndGame();
		}
	}

	if (!mData->mPaused) {
		if (mData->mPlaying) {
			mData->mUTF = (mData->mUTF + 1) % 100;
			if (mData->mUTF % 999 == 0) {
				//Add a second onto the mTime
				mData->mTime++;
			}
		}

		//Update each player's board
		for (int i = 0; i < mData->mPlayers; i++) {
			//mData->mPlayer[i]->GameTurn();
		}

		if (mData->mMode == MODE_ADVENTURE) {
			//Check if player board is in trouble
			bool humanIT = mData->mPlayer[0]->IsInTrouble();
			bool aiIT = mData->mPlayer[1]->IsInTrouble();
			if (humanIT) {
				if (aiIT) {
					if (mMonster->mAnimationState != Monster::ANI_STATE_LOSING) {
						mMonster->Losing();
					}
				} else {
					if (mMonster->mAnimationState != Monster::ANI_STATE_WINNING) {
						mMonster->Winning();
					}
				}
			} else {
				if (aiIT) {
					if (mMonster->mAnimationState != Monster::ANI_STATE_LOSING) {
						mMonster->Losing();
					}
				} else {
					if (mMonster->mAnimationState != Monster::ANI_STATE_PLAYING) {
						mMonster->Playing();
					}
				}
			}
		}
	} else {
		mMonster->Hide();
	}

	MarkDirty();
}

void Game::PlayerEnded()
{
	if (!mData->mEnding) {
		//Check if everyone has ended
		bool allended = true;
		for (int i = 0; i < mData->mPlayers; i++) {
			if (mData->mPlayer[i]->mState & STATE_ENDING) {
				if (mData->mPlayer[i]->mState & STATE_CLEAR_RAISING) {
					allended = false;
				}
			} else {
				allended = false;
				mData->mPlayer[i]->End();
			}
		}

		if (allended) {
			//Make all players bars lower
			for (int i = 0; i < mData->mPlayers; i++) {
				mData->mPlayer[i]->LowerAndClear();
			}

			mData->mEnding = true;
		}
	}
}

void Game::EndGame()
{
	if (!mData->mPlaying)
		return;
	else
		mData->mPlaying = false;

	//Stop the music
	StopMusic();

	//Highscore
	Highscore hs;
	hs.Name = mApp->mCurrentUser->mName;
	hs.Score = mData->mStdkeys[0]->mScore;
	hs.Moves = mData->mStdkeys[0]->mMoves;
	hs.Time = mData->mTime;

	//Results
	GameResults results;

	//If its a player 1 quick play then show the lose dialog
	if (mData->mMode == MODE_ADVENTURE) {
		results = mData->mResults;

		results.Type = 0;
		results.PlayedRounds++;

		//Wins
		int winner = FindWinner();
		if (winner == -1) {
			results.Wins[0]++;
			results.Wins[1]++;
		} else {
			results.Wins[winner]++;
		}

		//Player's statistics
		results.Rounds[0].Time = mData->mTime;
		results.Rounds[0].Score = mData->mStdkeys[0]->mScore;
		results.Rounds[0].Jewels = mData->mStdkeys[0]->mJewelsCrushed;
		results.Rounds[0].Moves = mData->mStdkeys[0]->mMoves;
		results.Rounds[0].MaxChain = mData->mStdkeys[0]->mMaxChain;
		results.Rounds[0].Chains = mData->mStdkeys[0]->mChains;
		results.Rounds[0].Level = mData->mStdkeys[0]->mLevel;
	} else if (mData->mMode == MODE_QUICKPLAY) {
		//Highscores
		AddQuickplayHighscore(hs);

		memset(&results, 0, sizeof(GameResults));

		results.Type = 1;
		results.PlayedRounds = 1;
		results.Rounds[0].Time = mData->mTime;
		results.Rounds[0].Score = mData->mStdkeys[0]->mScore;
		results.Rounds[0].Jewels = mData->mStdkeys[0]->mJewelsCrushed;
		results.Rounds[0].Moves = mData->mStdkeys[0]->mMoves;
		results.Rounds[0].MaxChain = mData->mStdkeys[0]->mMaxChain;
		results.Rounds[0].Chains = mData->mStdkeys[0]->mChains;
		results.Rounds[0].Level = mData->mStdkeys[0]->mLevel;
	}

	//Save statistics
	if (mApp->mCurrentUser != NULL) {
		//Save statistics
		mApp->mCurrentUser->mTime += mData->mTime;
		mApp->mCurrentUser->mJewelsCrushed += mData->mStdkeys[0]->mJewelsCrushed;
		mApp->mCurrentUser->mMoves += mData->mStdkeys[0]->mMoves;
		if (mApp->mCurrentUser->mHighestQuickplayScore < mData->mStdkeys[0]->mScore)
			mApp->mCurrentUser->mHighestQuickplayScore = mData->mStdkeys[0]->mScore;

		//Delete any saved game
		mApp->mCurrentUser->mHasSavedGame[mData->mMode] = false;

		if (mData->mMode == MODE_ADVENTURE) {
			//Save adventure results
			mApp->mCurrentUser->mAdventureResults = results;
		}

		mApp->mCurrentUser->Save();
	}

	//Save highscores
	SaveHighscores();

	mData->mResults = results;

	mGameOver = true;

	for (int p = 0; p < mData->mPlayers; p++) {
		mData->mPlayer[p]->mState = STATE_GAME_OVER;
	}
}

int Game::FindWinner()
{
	int losers = 0;
	bool *loser = new bool[mData->mPlayers];
	for (int i = 0; i < mData->mPlayers; i++) {
		loser[i] = mData->mPlayer[i]->mDetectedLost;
		if (loser[i])
			losers++;
	}
	if (losers == mData->mPlayers) {
		return -1;
	} else {
		for (int i = 0; i < mData->mPlayers; i++) {
			if (!loser[i])
				return i;
		}
	}
}

void Game::Pause()
{
	//Pause the game only if player 1 is in the
	//following states.
	if (mData->mStdkeys[0]->mState & STATE_PLAYING) {
		if (mData->mPaused) {
			//Unpause
			mData->mPaused = false;
			
			ResumeMusic();
		} else {
			//Pause
			mData->mPaused = true;
			PauseMusic();
		}
	}
}

#pragma endregion

#pragma region Save and Load

bool Game::SaveGame()
{
	//Get the current user
	User *user = mApp->mCurrentUser;

	//Check if the game needs saving
	if (mData->mPlaying) {
		//Set that there is a saved game
		user->mHasSavedGame[mData->mMode] = true;
		user->mSavedGame[mData->mMode] = mData;

		//Save the user's profile
		user->Save();

		//Return true - saved the game
		return true;
	} else {
		//Set that there isn't a saved game
		user->mHasSavedGame[mData->mMode] = false;

		//Save the user's profile
		user->Save();

		//Return false - no save needed
		return false;
	}
}

bool Game::LoadGame(GAME_MODE mode)
{
	//Get the current user
	User *user = mApp->mCurrentUser;

	//Check if there is a saved game
	if (user->mHasSavedGame[mode]) {
		//Ask the user if the game should be loaded or deleted

		//Load the saved game
		mData = user->mSavedGame[mode];

		//Return true - loaded a saved game
		return true;
	}

	//Return false - No game to load
	return false;
}

#pragma endregion

#pragma region Drawing

void Game::Draw(Graphics *g)
{
	//Draw the level background and mFrame
	g->DrawImage(GetGSETImage(GSET_BG, mData->mSet), 0, 0);
	
	//Display each player
	for (int i = 0; i < mData->mPlayers; i++) {
		DrawNPBorder(i, g);
		DrawASBorder(i, g);
		DrawBoardBorder(i, g);
		DrawScore(i, g);
		//mData->mPlayer[i]->DisplayBoard(g);
	}

	g->DrawImage(IMAGE_FRAME, 0, 0);

	//Draw the game mode
	g->SetFont(FONT_NORMAL);
	g->SetColor(Color::White);

	if (mData->mMode == MODE_ADVENTURE) {
		g->DrawString(SZ_ADVENTURE, 102 - (g->StringWidth(SZ_ADVENTURE) / 2), 36);

		//Draw player 1's current level and stage
		char cLevel[20]; sprintf(cLevel, "level : %d - %d", mData->mLevel, mData->mStage);
		g->DrawString(cLevel, 400 - (g->StringWidth(cLevel) / 2), 34);

		DrawOpponent(g);
	}
	else if (mData->mMode == MODE_QUICKPLAY) {
		g->DrawString(SZ_QUICKPLAY, 102 - (g->StringWidth(SZ_QUICKPLAY) / 2), 36);
		//Draw player 1's current level
		char cLevel[20]; sprintf(cLevel, "level : %d", mData->mStdkeys[0]->mLevel);
		g->DrawString(cLevel, 400 - (g->StringWidth(cLevel) / 2), 34);
	}

	//Draw round points
	for (int p = 0; p < 2; p++) {
		int wins = mData->mResults.Wins[p];
		for (int w = 0; w < min(wins, 2); w++) {
			g->DrawImage(IMAGE_ROUND_POINT, 347 + (66 * p), 264 + (40 * w));
		}
	}

	//Draw the game mTime
	DrawTime(g);

	//Used for getset sequence
	if (mData->mPrestart == 1) {
		mData->mPrestart = 2;
	}
}

void Game::DrawNPBorder(int p, Graphics* g)
{
	switch (p) {
		case 0:
			g->DrawImage(IMAGE_NP_FRAME, BoardPoints[mData->mPlayers - 1].p1.npx - 9, BoardPoints[mData->mPlayers - 1].p1.npy - 6);
			break;
		case 1:
			g->DrawImage(IMAGE_NP_FRAME, BoardPoints[mData->mPlayers - 1].p2.npx - 9, BoardPoints[mData->mPlayers - 1].p2.npy - 6);
			break;
		case 2:
			g->DrawImage(IMAGE_NP_FRAME, BoardPoints[mData->mPlayers - 1].p3.npx - 9, BoardPoints[mData->mPlayers - 1].p3.npy - 6);
			break;
	}
}

void Game::DrawBoardBorder(int p, Graphics *g)
{
	switch (p) {
		case 0:
			g->DrawImage(IMAGE_BOARD_FRAME, BoardPoints[mData->mPlayers - 1].p1.boardx - 4, BoardPoints[mData->mPlayers - 1].p1.boardy - 4);
			break;
		case 1:
			g->DrawImage(IMAGE_BOARD_FRAME, BoardPoints[mData->mPlayers - 1].p2.boardx - 4, BoardPoints[mData->mPlayers - 1].p2.boardy - 4);
			break;
		case 2:
			g->DrawImage(IMAGE_BOARD_FRAME, BoardPoints[mData->mPlayers - 1].p3.boardx - 4, BoardPoints[mData->mPlayers - 1].p3.boardy - 4);
			break;
	}
}

void Game::DrawASBorder(int p, Graphics *g)
{
	if (mData->mPlayers > 1) {
		switch (p) {
			case 0:
				g->DrawImage(IMAGE_AS_FRAME, BoardPoints[mData->mPlayers - 1].p1.npx - 9, BoardPoints[mData->mPlayers - 1].p1.npy - 6 + (38 * 4));
				break;
			case 1:
				g->DrawImage(IMAGE_AS_FRAME, BoardPoints[mData->mPlayers - 1].p2.npx - 9, BoardPoints[mData->mPlayers - 1].p2.npy - 6 + (38 * 4));
				break;
			case 2:
				g->DrawImage(IMAGE_AS_FRAME, BoardPoints[mData->mPlayers - 1].p3.npx - 9, BoardPoints[mData->mPlayers - 1].p3.npy - 6 + (38 * 4));
				break;
		}
	}
}

void Game::DrawScore(int p, Graphics* g)
{
	char s[10], j[10];
	switch (mData->mPlayers) {
		case 1:
			//Score
			sprintf(s, "%d", mData->mPlayer[p]->mPrescore);
			sprintf(j, "%d", mData->mPlayer[p]->mJewelsCrushed);

			//Draw score and jewels
			g->SetColor(Color::White);
			g->SetFont(FONT_NORMAL);
			g->DrawImage(IMAGE_SCORE_FRAME, 530, 180);
			g->DrawString("Score :", 530, 180);
			g->DrawString(s, 539, 204);

			g->DrawImage(IMAGE_SCORE_FRAME, 530, 180 + 50);
			g->DrawString("Jewels :", 530, 180 + 50);
			g->DrawString(j, 539, 204 + 50);
			break;
		default:
			g->SetColor(Color::White);
			g->SetFont(FONT_NORMAL);
			sprintf(j, "%d", mData->mPlayer[p]->mAttackScore);
			switch (p) {
				case 0:
					g->DrawString(j, BoardPoints[mData->mPlayers - 1].p1.npx - 9 + 8, BoardPoints[mData->mPlayers - 1].p1.npy - 6 + (38 * 4) + 35);
					break;
				case 1:
					g->DrawString(j, BoardPoints[mData->mPlayers - 1].p2.npx - 9 + 8, BoardPoints[mData->mPlayers - 1].p2.npy - 6 + (38 * 4) + 35);
					break;
				case 2:
					g->DrawString(j, BoardPoints[mData->mPlayers - 1].p3.npx - 9 + 8, BoardPoints[mData->mPlayers - 1].p3.npy - 6 + (38 * 4) + 35);
					break;
			}
			break;
	}
}

void Game::DrawTime(Graphics *g)
{
	char cTime[10];
	strcpy(cTime, GetTime(mData->mTime));
	g->DrawString(cTime, 400 - (g->StringWidth(cTime) / 2), 564);
}

void Game::DrawOpponent(Graphics *g)
{
	//g->DrawImage(IMAGE_OPPONENT_FRAME, 340, 370);
	//g->DrawImageCel(IMAGE_MONST_1, 340 + 8, 370 + 8, IMAGE_MONST_1->GetAnimCel(mUpdateCnt), 0);
}

#pragma endregion

#pragma region Music and Sound

void Game::PlayMusic()
{
	//Play the level music
	mApp->mMusicInterface->PlayMusic(MUSIC_MODERN, 0, false);
	mStartedMusic = true;
}

void Game::ResumeMusic()
{
	if (mStartedMusic) {
		mApp->mMusicInterface->ResumeMusic(MUSIC_MODERN);
	} else {
		PlayMusic();
	}
}

void Game::PauseMusic()
{
	if (mStartedMusic) {
		mApp->mMusicInterface->PauseMusic(MUSIC_MODERN);
	}
}

void Game::StopMusic()
{
	//Stop the level music
	mApp->mMusicInterface->StopMusic(MUSIC_MODERN);
	mStartedMusic = false;
}

void Game::PlayStrikeSound(int currentStrike)
{
	//Play the strike sound, change pitch
	SoundInstance *strikeSound = mApp->mSoundManager->GetSoundInstance(SOUND_STRIKE);

	strikeSound->AdjustPitch((currentStrike - 1) * 1.5f);
	strikeSound->Play(false, true);
}

void Game::PlayASound(GSET_RESOURCE s)
{
	//Play a level sound
	mApp->PlaySample(GetGSETSound(s, mData->mSet));
}

void Game::PlayASound(int s)
{
	//Play a level sound
	mApp->PlaySample(s);
}

#pragma endregion