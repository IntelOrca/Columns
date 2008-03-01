/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "Core.h"
#include "Strings.h"
#include "Highscore.h"

namespace Sexy
{
	Highscore TopAdventureHighscores[MAX_ADVENTURE_SCORES];
	Highscore TopQuickplayHighscores[MAX_QUICKPLAY_SCORES];
	Highscore TopSecretHighscores[MAX_SECRET_SCORES];

	void AddAdventureHighscore(int level, Highscore hs)
	{
		if (hs.Score > TopAdventureHighscores[level].Score) {
			TopAdventureHighscores[level] = hs;
		}
	}

	void AddQuickplayHighscore(Highscore hs)
	{
		//Add the highscore by score
		for (int i = 0; i < MAX_QUICKPLAY_SCORES; i++) {
			if (hs.Score > TopQuickplayHighscores[i].Score) {
				//Beaten this score, move all score down one
				for (int j = MAX_QUICKPLAY_SCORES - 1; j > i; j--) {
					TopQuickplayHighscores[j] = TopQuickplayHighscores[j - 1];
				}
				TopQuickplayHighscores[i] = hs;
				break;
			}
		}
	}

	void AddSecretHighscore(Highscore hs)
	{
		//Add the highscore by score
		for (int i = 0; i < MAX_SECRET_SCORES; i++) {
			if (hs.Score > TopSecretHighscores[i].Score) {
				//Beaten this score, move all score down one
				for (int j = i; j < MAX_SECRET_SCORES - 1; j++) {
					TopSecretHighscores[j + 1] = TopSecretHighscores[j];
				}
				TopSecretHighscores[i] = hs;
				break;
			}
		}
	}

	void LoadHighscores()
	{
		Buffer *hs = new Buffer();
		ClearHighscores();

		//Read file
		if (!gSexyAppBase->FileExists(PATH_HIGHSCORE_FILE)) {
			SaveHighscores();
			return;
		}

		gSexyAppBase->ReadBufferFromFile(PATH_HIGHSCORE_FILE, hs);

		//Read Adventure times
		for (int i = 0; i < MAX_ADVENTURE_SCORES; i++) {
			//Highscore highs;
			TopAdventureHighscores[i].Name = ReadString(hs);
			TopAdventureHighscores[i].Score = hs->ReadLong();
			TopAdventureHighscores[i].Moves = hs->ReadLong();
			TopAdventureHighscores[i].Time = hs->ReadLong();
		}

		//Read Quickplay highscores
		for (int i = 0; i < MAX_QUICKPLAY_SCORES; i++) {
			TopQuickplayHighscores[i].Name = ReadString(hs);
			TopQuickplayHighscores[i].Score = hs->ReadLong();
			TopQuickplayHighscores[i].Moves = hs->ReadLong();
			TopQuickplayHighscores[i].Time = hs->ReadLong();
		}

		//Read Secret highscores
		for (int i = 0; i < MAX_SECRET_SCORES; i++) {
			TopSecretHighscores[i].Name = ReadString(hs);
			TopSecretHighscores[i].Score = hs->ReadLong();
			TopSecretHighscores[i].Moves = hs->ReadLong();
			TopSecretHighscores[i].Time = hs->ReadLong();
		}

		delete hs;
	}

	void SaveHighscores()
	{
		Buffer *hs = new Buffer();

		//Save Adventure times
		for (int i = 0; i < MAX_ADVENTURE_SCORES; i++) {
			hs->WriteString(TopAdventureHighscores[i].Name);
			hs->WriteLong(TopAdventureHighscores[i].Score);
			hs->WriteLong(TopAdventureHighscores[i].Moves);
			hs->WriteLong(TopAdventureHighscores[i].Time);
		}

		//Save Quickplay highscores
		for (int i = 0; i < MAX_QUICKPLAY_SCORES; i++) {
			hs->WriteString(TopQuickplayHighscores[i].Name);
			hs->WriteLong(TopQuickplayHighscores[i].Score);
			hs->WriteLong(TopQuickplayHighscores[i].Moves);
			hs->WriteLong(TopQuickplayHighscores[i].Time);
		}

		//Save Secret highscores
		for (int i = 0; i < MAX_SECRET_SCORES; i++) {
			hs->WriteString(TopSecretHighscores[i].Name);
			hs->WriteLong(TopSecretHighscores[i].Score);
			hs->WriteLong(TopSecretHighscores[i].Moves);
			hs->WriteLong(TopSecretHighscores[i].Time);
		}

		//Save file
		gSexyAppBase->WriteBufferToFile(PATH_HIGHSCORE_FILE, hs);

		delete hs;
	}

	void ClearHighscores()
	{
		//Clear Adventure highscores
		for (int i = 0; i < MAX_ADVENTURE_SCORES; i++) {
			TopAdventureHighscores[i].Name = "";
			TopAdventureHighscores[i].Score = 0;
			TopAdventureHighscores[i].Time = 0;
		}

		//Clear Quickplay highscores
		for (int i = 0; i < MAX_QUICKPLAY_SCORES; i++) {
			TopQuickplayHighscores[i].Name = "";
			TopQuickplayHighscores[i].Score = 0;
			TopQuickplayHighscores[i].Time = 0;
		}

		//Clear Secret highscores
		for (int i = 0; i < MAX_SECRET_SCORES; i++) {
			TopSecretHighscores[i].Name = "";
			TopSecretHighscores[i].Score = 0;
			TopSecretHighscores[i].Time = 0;
		}
	}
}