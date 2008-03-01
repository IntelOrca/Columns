#include "GameData.h"

GameData *GameData::FromBuffer(Buffer *sav)
{
	GameData *data = new GameData();

	//Clear the structure
	memset(data, 0, sizeof(GameData));

	//Number of players
	data->mPlayers = sav->ReadLong();
	
	data->mHumanID = sav->ReadLong();
	data->mUTF = sav->ReadLong();
	data->mTime = sav->ReadLong();
	data->mPrestart = sav->ReadLong();
	data->mSeed = sav->ReadLong();

	data->mMode = (GAME_MODE)sav->ReadLong();
	data->mSet = (GSET)sav->ReadLong();
	data->mCurrentRound = sav->ReadLong();
	data->mStage = sav->ReadLong();
	data->mLevel = sav->ReadLong();
	data->mEnding = sav->ReadBoolean();
	data->mPlaying = sav->ReadBoolean();
	data->mPaused = sav->ReadBoolean();

	//Game Results
	data->mResults.Type = sav->ReadLong();
	data->mResults.PlayedRounds = sav->ReadLong();

	for (int r = 0; r < 3; r++) {
		data->mResults.Wins[r] = sav->ReadLong();
	}

	for (int r = 0; r < 3; r++) {
		RoundResults *round = &data->mResults.Rounds[r];
		round->Score = sav->ReadLong();
		round->Jewels = sav->ReadLong();
		round->Moves = sav->ReadLong();
		round->Chains = sav->ReadLong();
		round->MaxChain = sav->ReadLong();
		round->Level = sav->ReadLong();
		round->Time = sav->ReadLong();
	}

	//Boards
	for (int p = 0; p < data->mPlayers; p++) {
		data->mPlayer[p] = Board::FromBuffer(sav, data);
	}

	return data;
}

void GameData::SaveToBuffer(Buffer *sav)
{
	//Number of players
	sav->WriteLong(mPlayers);

	sav->WriteLong(mHumanID);
	sav->WriteLong(mUTF);
	sav->WriteLong(mTime);
	sav->WriteLong(mPrestart);
	sav->WriteLong(mSeed);

	sav->WriteLong(mMode);
	sav->WriteLong(mSet);
	sav->WriteLong(mCurrentRound);
	sav->WriteLong(mStage);
	sav->WriteLong(mLevel);
	sav->WriteBoolean(mEnding);
	sav->WriteBoolean(mPlaying);
	sav->WriteBoolean(mPaused);

	//Game Results
	sav->WriteLong(mResults.Type);
	sav->WriteLong(mResults.PlayedRounds);

	for (int r = 0; r < 3; r++) {
		sav->WriteLong(mResults.Wins[r]);
	}

	for (int r = 0; r < 3; r++) {
		RoundResults *round = &mResults.Rounds[r];
		sav->WriteLong(round->Score);
		sav->WriteLong(round->Jewels);
		sav->WriteLong(round->Moves);
		sav->WriteLong(round->Chains);
		sav->WriteLong(round->MaxChain);
		sav->WriteLong(round->Level);
		sav->WriteLong(round->Time);
	}

	//Boards
	for (int p = 0; p < mPlayers; p++) {
		mPlayer[p]->SaveToBuffer(sav);
	}
}