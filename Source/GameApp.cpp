/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework/WidgetManager.h"
#include "SexyAppFramework/Common.h"
#include "SexyAppFramework/ResourceManager.h"
#include "SexyAppFramework/BassMusicInterface.h"
#include "PakLib/PakInterface.h"

#include "Strings.h"
#include "GameApp.h"
#include "User.h"
#include "TitleScreen.h"
#include "UserDialog.h"
#include "StageSelect.h"
#include "Menu.h"
#include "Game.h"
#include "ResultsScreen.h"
#include "HighscoreScreen.h"
#include "Res.h"

using namespace Sexy;

#pragma region Constructor/Destructor

GameApp::GameApp()
{
	//mProdName is used for internal purposes to indicate the game that we're working on
	mProdName = APP_NAME;

	//For internal uses, indicates the current product version
	mProductVersion = APP_VERSION;

	//This is the text that appears in the title bar of the application window
	mTitle = StringToSexyStringFast(mProdName + " - " + mProductVersion);

	//Indicates the registry location where all registry keys will be read from
	//and written to. This is stored under the HKEY_CURRENT_USER tree on 
	//Windows systems.
	mRegKey = APP_REG_KEY;

	//Set the application width/height in terms of pixels here.
	mWidth = APP_WIDTH;
	mHeight = APP_HEIGHT;

	//mDebugKeysEnabled = true;

	//Keep mFrame time at a constant rate for window and fullscreen
	mFrameTime = 10;
	mAutoEnable3D = true;
	mVSyncUpdates = true;

	mLostFocus = false;

	mCurrentUser = NULL;

	mTitleScreen = NULL;
	mMenu = NULL;
	mGame = NULL;
	mHighscores = NULL;
	mResults = NULL;
}

GameApp::~GameApp()
{
	CloseTitle();
	CloseGame();
	CloseMenu();
	CloseHighscores();
	CloseResults();

	//Delete any widgets
	delete mTitleScreen;
	delete mMenu;
	delete mGame;
	delete mHighscores;
	delete mResults;

	mResourceManager->DeleteResources("Title");
	mResourceManager->DeleteResources("Game");
	CloseMusicTracks(mMusicInterface);
}

#pragma endregion

#pragma region Initialization and Loading

void GameApp::Init()
{
	SexyAppBase::Init();
	LoadResourceManifest();

	if (!mResourceManager->LoadResources("Title"))
	{
		mLoadingFailed = true;
		//This will display an informative error message indicating exactly
		//what went wrong in the resource loading process.
		ShowResourceError(true);
		return;
	}

	if (!ExtractTitleResources(mResourceManager))
	{
		mLoadingFailed = true;
		ShowResourceError(true);
		return;
	}
	InitMusicTracks(mMusicInterface);
}

void GameApp::LoadingThreadProc()
{
	//Show the title dialog
	ShowTitle();

	//Start loading in the resources
	mNumLoadingThreadTasks = mResourceManager->GetNumResources("Game");
	mResourceManager->StartLoadResources("Game");

	//Load all the resources
	while (mResourceManager->LoadNextResource())
	{
		//Increment loading progress
		mCompletedLoadingThreadTasks++;
		if (mShutdown)
			return;

		//Make the title screen repaint
		mTitleScreen->MarkDirty();
	}

	//Check if there was an error loading
	if (mResourceManager->HadError() || !ExtractGameResources(mResourceManager))
	{
		ShowResourceError(false);
		mLoadingFailed = true;
		return;
	}
}

void GameApp::LoadingThreadCompleted()
{
	SexyAppBase::LoadingThreadCompleted();

	//Return if loading failed
	if (mLoadingFailed)
		return;

	//Tell title screen to show continue link
	mTitleScreen->LoadingComplete();
	mTitleScreen->MarkDirty();

	//Load startup user and highscores
	LoadStartupUser();
	LoadHighscores();
}

void GameApp::TitleScreenIsFinished()
{
	//Close the title and show the menu
	CloseTitle();
	ShowMenu();
}

#pragma endregion

#pragma region Save and Load

void GameApp::ReadFromRegistry()
{
	SexyAppBase::ReadFromRegistry();
}

void GameApp::WriteToRegistry()
{
	SexyAppBase::WriteToRegistry();
	//Save the current user selection
	if (mCurrentUser != NULL)
		RegistryWriteString("CurUser", mCurrentUser->mName);
}

void GameApp::LoadStartupUser()
{
	SetCurrentUser(NULL);

	//Read the current user from the last session
	User *usr;
	char *str;
	std::string lastuser;

	//Read last user registry
	if (RegistryReadString("CurUser", &lastuser)) {
		//Turn string into char array
		str = new char[lastuser.length()];
		strcpy(str, lastuser.c_str());
	} else {
		str = "";
	}

	//Find user in user directory
	usr = FindUserFromDirectory(this, str);
	if (usr != NULL) {
		//Set the current user
		SetCurrentUser(usr);
	} else {
		//Force to make a new user
	}
}

void GameApp::SetCurrentUser(User *user)
{
	//Delete any existing current user
	if (mCurrentUser != NULL)
		delete mCurrentUser;

	//Set the current user
	mCurrentUser = user;
}

#pragma endregion

#pragma region Dialogs

/* Creates a new title dialog
 * and shows it.
 */
void GameApp::ShowTitle()
{
	//Delete any existing title screen
	if (mTitleScreen != NULL)
		delete mTitleScreen;

	//Initialize a new title screen
	mTitleScreen = new TitleScreen(this);
	mTitleScreen->Init();
	mTitleScreen->Resize(0, 0, mWidth, mHeight);

	//Add to widget manager
	mWidgetManager->AddWidget(mTitleScreen);
}

/* Creates a new title dialog
 * and shows it.
 */
void GameApp::CloseTitle()
{
	//Remove the title screen from the widget manager
	if (mTitleScreen != NULL)
		mWidgetManager->RemoveWidget(mTitleScreen);
}

void GameApp::ShowMenu()
{
	//Delete any existing menu
	if (mMenu != NULL)
		delete mMenu;

	//Initialize a new menu
	mMenu = new Menu(this);
	mMenu->Init();
	mMenu->Resize(0, 0, mWidth, mHeight);

	//Add to widget manager
	mWidgetManager->AddWidget(mMenu);
}

void GameApp::CloseMenu()
{
	//Remove the menu from the widget manager
	if (mMenu != NULL)
		mWidgetManager->RemoveWidget(mMenu);
}

void GameApp::ShowGame(GAME_MODE mode, GSET set, GameResults results)
{
	//BUG: Memory failure when following code uncommented
	//Delete any existing game
	//if (mGame != NULL)
		//delete mGame;

	//Initialize a new game
	mGame = new Game(this);
	mGame->Init(mode, results);
	mGame->mData->mSet = set;
	mGame->Resize(0, 0, mWidth, mHeight);

	//Add to widget manager and set focus to it
	mWidgetManager->AddWidget(mGame);
	mWidgetManager->SetFocus(mGame);
}

void GameApp::ShowGame(GAME_MODE mode, GSET set)
{
	//Create a new game results structure
	GameResults results;
	if (mode == MODE_ADVENTURE) {
		results = mCurrentUser->mAdventureResults;
	} else {
		memset(&results, 0, sizeof(GameResults));
	}

	//Show the game with the new structure
	ShowGame(mode, set, results);
}

void GameApp::CloseGame()
{
	//Remove the game from the widget manager
	if (mGame != NULL)
		mWidgetManager->RemoveWidget(mGame);
}

void GameApp::ShowResults(GameResults results)
{
	//Delete any existing results screen
	if (mResults != NULL)
		delete mResults;

	//Initialize a new results screen
	mResults = new ResultsScreen(this);
	mResults->Init(results);
	mResults->Resize(0, 0, mWidth, mHeight);

	//Add to widget manager and set focus to it
	mWidgetManager->AddWidget(mResults);
	mWidgetManager->SetFocus(mResults);
}

void GameApp::CloseResults()
{
	//Remove the results screen from the widget manager
	if (mResults != NULL)
		mWidgetManager->RemoveWidget(mResults);
}

void GameApp::ShowHighscores()
{
	//Delete any existing highscore screen
	if (mHighscores != NULL)
		delete mHighscores;

	//Initialize a new game
	mHighscores = new HighscoreScreen(this);
	mHighscores->Init();
	mHighscores->Resize(0, 0, mWidth, mHeight);

	//Add to widget manager and set focus to it
	mWidgetManager->AddWidget(mHighscores);
	mWidgetManager->SetFocus(mHighscores);
}

void GameApp::CloseHighscores()
{
	//Remove the highscore screen from the widget manager
	if (mHighscores != NULL)
		mWidgetManager->RemoveWidget(mHighscores);
}

#pragma endregion

#pragma region Misc.

void GameApp::GotFocus()
{
	if (mLostFocus) {
		//Unpause
		if (mGame != NULL) {
			if (mGame->mData->mPaused)
				mGame->Pause();
		}
	}
	mLostFocus = false;
}

void GameApp::LostFocus()
{
	//Pause
	mLostFocus = true;
	if (mGame != NULL) {
		if (!mGame->mData->mPaused)
			mGame->Pause();
		else
			mLostFocus = false;
	}
}

#pragma endregion
