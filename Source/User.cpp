/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework\SexyAppBase.h"
#include "SexyAppFramework\Buffer.h"
#include "Strings.h"
#include "GameApp.h"
#include "User.h"
#include "Core.h"

using namespace Sexy;

User::User()
{
	
}

User::User(GameApp *theApp, char *name, bool create)
{
	mApp = theApp;
	memset(mFilename, 0, MAX_PATH);
	mHighestQuickplayScore = 0;
	mJewelsCrushed = 0;
	mMoves = 0;
	mTime = 0;
	mStage = 1;
	mLevel = 1;
	mStagesUnlocked = 1;
	mCompletedAdventure = false;

	memset(&mAdventureResults, 0, sizeof(GameResults));

	for (int i = 0; i < 3; i++) {
		mHasSavedGame[i] = false;
	}

	if (create)
		New(name);
	else
		Load(name);
}

void User::New(char *name)
{
	mName = name;
}

bool User::Load(char *filename)
{
	Buffer *sav = new Buffer();
	gSexyAppBase->ReadBufferFromFile(filename, sav);

	//Save filename
	strcpy(mFilename, filename);

	//Read code
	uchar code[4];
	sav->ReadBytes(code, 4);
	if (*((int*)(code)) == 0x46554354) {
		//Read general things
		mName = ReadString(sav);					//Read name
		mStage = sav->ReadByte();
		mLevel = sav->ReadByte();
		mStagesUnlocked = sav->ReadByte();
		mCompletedAdventure = sav->ReadBoolean();

		//Read adventure results
		mAdventureResults.Type = sav->ReadLong();
		mAdventureResults.PlayedRounds = sav->ReadLong();

		for (int r = 0; r < 3; r++) {
			mAdventureResults.Wins[r] = sav->ReadLong();
		}

		for (int r = 0; r < 3; r++) {
			RoundResults *round = &mAdventureResults.Rounds[r];
			round->Score = sav->ReadLong();
			round->Jewels = sav->ReadLong();
			round->Moves = sav->ReadLong();
			round->Chains = sav->ReadLong();
			round->MaxChain = sav->ReadLong();
			round->Level = sav->ReadLong();
			round->Time = sav->ReadLong();
		}

		//Read statistics
		mMoves = sav->ReadLong();					//Number of moves
		mJewelsCrushed = sav->ReadLong();			//Crushed jewels
		mHighestQuickplayScore = sav->ReadLong();	//Highest quickplay score
		mTime = sav->ReadLong();					//Read time

		GameData *d1, *d2;

		//Read saved games
		for (int i = 0; i < 3; i++) {
			mHasSavedGame[i] = sav->ReadBoolean();
			if (mHasSavedGame[i]) {
				mSavedGame[i] = GameData::FromBuffer(sav);
				d1 = mSavedGame[i];
				d2 = mSavedGame[i]->mPlayer[0]->mData;
			} else {
				memset(&mSavedGame[i], 0, sizeof(GameData));
			}
		}

		mSuccessfulLoad = true;
	} else {
		mSuccessfulLoad = false;
	}

	delete sav;
	return mSuccessfulLoad;
}

void User::Save(char *filename)
{
	Buffer *sav = new Buffer();

	//Save filename
	strcpy(mFilename, filename);
	
	//Write code
	sav->WriteLong(0x46554354);

	//Write general things
	sav->WriteString(mName);					//Write user name
	sav->WriteByte(mStage);
	sav->WriteByte(mLevel);
	sav->WriteByte(mStagesUnlocked);
	sav->WriteBoolean(mCompletedAdventure);

	//Write adventure results
	sav->WriteLong(mAdventureResults.Type);
	sav->WriteLong(mAdventureResults.PlayedRounds);

	for (int r = 0; r < 3; r++) {
		sav->WriteLong(mAdventureResults.Wins[r]);
	}

	for (int r = 0; r < 3; r++) {
		RoundResults *round = &mAdventureResults.Rounds[r];
		sav->WriteLong(round->Score);
		sav->WriteLong(round->Jewels);
		sav->WriteLong(round->Moves);
		sav->WriteLong(round->Chains);
		sav->WriteLong(round->MaxChain);
		sav->WriteLong(round->Level);
		sav->WriteLong(round->Time);
	}

	//Write statistics
	sav->WriteLong(mMoves);						//Number of moves
	sav->WriteLong(mJewelsCrushed);				//Crushed jewels
	sav->WriteLong(mHighestQuickplayScore);		//Highest quickplay score
	sav->WriteLong(mTime);						//Write time

	//Write saved games
	for (int i = 0; i < 3; i++) {
		sav->WriteBoolean(mHasSavedGame[i]);
		if (mHasSavedGame[i]) {
			mSavedGame[i]->SaveToBuffer(sav);
		}
	}

	//Save file
	gSexyAppBase->WriteBufferToFile(filename, sav);

	delete sav;
}

void User::Save()
{
	if (strlen(mFilename) > 0) {
		Save(mFilename);
	}
}

User::~User()
{

}

namespace Sexy
{
	User *FindUserFromDirectory(GameApp *mApp, char *name)
	{
		User *fusr = NULL;
		User *usr = NULL;
		WIN32_FIND_DATA ffd;
		TCHAR szDir[MAX_PATH] = PATH_USER_FILES;
		HANDLE hFind = INVALID_HANDLE_VALUE;
		DWORD dwError=0;

		//Find the first file in the directory
		hFind = FindFirstFile(szDir, &ffd);
		if (hFind != INVALID_HANDLE_VALUE) {
			char filename[MAX_PATH]; sprintf(filename, "%s%s", PATH_USERS, ffd.cFileName);
			//Load the user
			usr = new User(mApp, filename, false);
			//Store this user incase other one doesn't exist
			fusr = usr;
			if (strcmp(usr->mName, name) != 0) {
				while (FindNextFile(hFind, &ffd) != 0) {
					char filename[MAX_PATH]; sprintf(filename, "%s%s", PATH_USERS, ffd.cFileName);
					usr = new User(mApp, filename, false);
					if (usr->mSuccessfulLoad) {
						//Check name matches
						if (strcmp(usr->mName, name) == 0) {
							//First user is no longer needed
							delete fusr;
							//Exit the loop
							break;
						} else {
							//Clean up the memory
							delete usr;
							usr = NULL;
						}
					} else {
						delete usr;
						usr = NULL;
					}
				}
			}
		}
		FindClose(hFind);
		if (usr != NULL) {
			return usr;
		} else if (fusr != NULL) {
			return fusr;
		}
		return NULL;
	}
}