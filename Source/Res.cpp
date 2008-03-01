/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/BassMusicInterface.h"
#include "SexyAppFramework/ResourceManager.h"
#include "Res.h"

using namespace Sexy;

//Game Resources
Image* Sexy::IMAGE_TITLE_SCREEN;
Image* Sexy::IMAGE_LOADING_BAR;
Image* Sexy::IMAGE_BTN_MENU_ADVENTURE_UP;
Image* Sexy::IMAGE_BTN_MENU_ADVENTURE_OVER;
Image* Sexy::IMAGE_BTN_MENU_ADVENTURE_DOWN;
Image* Sexy::IMAGE_BTN_MENU_QUICK_UP;
Image* Sexy::IMAGE_BTN_MENU_QUICK_OVER;
Image* Sexy::IMAGE_BTN_MENU_QUICK_DOWN;
Image* Sexy::IMAGE_BTN_MENU_OPTIONS_UP;
Image* Sexy::IMAGE_BTN_MENU_OPTIONS_OVER;
Image* Sexy::IMAGE_BTN_MENU_OPTIONS_DOWN;
Image* Sexy::IMAGE_BTN_MENU_QUIT_UP;
Image* Sexy::IMAGE_BTN_MENU_QUIT_OVER;
Image* Sexy::IMAGE_BTN_MENU_QUIT_DOWN;

Image* Sexy::IMAGE_STAGE_FRAME;
Image* Sexy::IMAGE_FRAME;
Image* Sexy::IMAGE_NP_FRAME;
Image* Sexy::IMAGE_SCORE_FRAME;
Image* Sexy::IMAGE_AS_FRAME;
Image* Sexy::IMAGE_OPPONENT_FRAME;
Image* Sexy::IMAGE_BOARD_FRAME;
Image* Sexy::IMAGE_BTN_MENU;
Image* Sexy::IMAGE_READY;
Image* Sexy::IMAGE_WIN;
Image* Sexy::IMAGE_LOSE;
Image* Sexy::IMAGE_ROUND_POINT;
Image* Sexy::IMAGE_DIALOG;
Image* Sexy::IMAGE_DIALOG_BTN;
Image* Sexy::IMAGE_CHECKBOX;
Image* Sexy::IMAGE_SLIDER;
Image* Sexy::IMAGE_SLIDER_THUMB;
Image* Sexy::IMAGE_BLUE_SKY;
Image* Sexy::IMAGE_MAIN_MENU;

Image* Sexy::IMAGE_GSET_MODERN;
Image* Sexy::IMAGE_GSET_CLASSIC;
Image* Sexy::IMAGE_GSET_OTHER1;
Image* Sexy::IMAGE_GSET_OTHER2;
Image* Sexy::IMAGE_GSET_SECRET;
Image* Sexy::IMAGE_BG_MODERN;
Image* Sexy::IMAGE_BG_CLASSIC;
Image* Sexy::IMAGE_BG_OTHER1;
Image* Sexy::IMAGE_BG_OTHER2;
Image* Sexy::IMAGE_BG_SECRET;

Image* Sexy::IMAGE_MONST_1;

int Sexy::SOUND_GETSET;
int Sexy::SOUND_CHANGE;
int Sexy::SOUND_LAND;
int Sexy::SOUND_LEVELUP;
int Sexy::SOUND_STRIKE;
int Sexy::SOUND_CRUSHBAR;
int Sexy::SOUND_SMASH;
int Sexy::SOUND_LOST;
int Sexy::SOUND_SELECT;
Font* Sexy::FONT_HUX;
Font* Sexy::FONT_ARIAL;
Font* Sexy::FONT_NORMAL;

bool Sexy::ExtractTitleResources(ResourceManager *theManager)
{
	ResourceManager &aMgr = *theManager;
	try {
		IMAGE_TITLE_SCREEN = aMgr.GetImageThrow("IMAGE_TITLE_SCREEN");
		IMAGE_LOADING_BAR = aMgr.GetImageThrow("IMAGE_LOADING_BAR");

		FONT_ARIAL = aMgr.GetFontThrow("FONT_ARIAL");
		FONT_NORMAL = aMgr.GetFontThrow("FONT_NORMAL");
		FONT_HUX = aMgr.GetFontThrow("FONT_HUX");
	} catch(ResourceManagerException&) {
		return false;
	}
	return true;
}

bool Sexy::ExtractGameResources(ResourceManager *theManager)
{
	ResourceManager &aMgr = *theManager;
	try {
		IMAGE_BTN_MENU_ADVENTURE_UP = aMgr.GetImageThrow("IMAGE_BTN_MENU_ADVENTURE_UP");
		IMAGE_BTN_MENU_ADVENTURE_OVER = aMgr.GetImageThrow("IMAGE_BTN_MENU_ADVENTURE_OVER");
		IMAGE_BTN_MENU_ADVENTURE_DOWN = aMgr.GetImageThrow("IMAGE_BTN_MENU_ADVENTURE_DOWN");
		IMAGE_BTN_MENU_QUICK_UP = aMgr.GetImageThrow("IMAGE_BTN_MENU_QUICK_UP");
		IMAGE_BTN_MENU_QUICK_OVER = aMgr.GetImageThrow("IMAGE_BTN_MENU_QUICK_OVER");
		IMAGE_BTN_MENU_QUICK_DOWN = aMgr.GetImageThrow("IMAGE_BTN_MENU_QUICK_DOWN");
		IMAGE_BTN_MENU_OPTIONS_UP = aMgr.GetImageThrow("IMAGE_BTN_MENU_OPTIONS_UP");
		IMAGE_BTN_MENU_OPTIONS_OVER = aMgr.GetImageThrow("IMAGE_BTN_MENU_OPTIONS_OVER");
		IMAGE_BTN_MENU_OPTIONS_DOWN = aMgr.GetImageThrow("IMAGE_BTN_MENU_OPTIONS_DOWN");
		IMAGE_BTN_MENU_QUIT_UP = aMgr.GetImageThrow("IMAGE_BTN_MENU_QUIT_UP");
		IMAGE_BTN_MENU_QUIT_OVER = aMgr.GetImageThrow("IMAGE_BTN_MENU_QUIT_OVER");
		IMAGE_BTN_MENU_QUIT_DOWN = aMgr.GetImageThrow("IMAGE_BTN_MENU_QUIT_DOWN");

		IMAGE_STAGE_FRAME = aMgr.GetImageThrow("IMAGE_STAGE_FRAME");
		IMAGE_FRAME = aMgr.GetImageThrow("IMAGE_FRAME");
		IMAGE_NP_FRAME = aMgr.GetImageThrow("IMAGE_NP_FRAME");
		IMAGE_SCORE_FRAME = aMgr.GetImageThrow("IMAGE_SCORE_FRAME");
		IMAGE_AS_FRAME = aMgr.GetImageThrow("IMAGE_AS_FRAME");
		IMAGE_OPPONENT_FRAME = aMgr.GetImageThrow("IMAGE_OPPONENT_FRAME");
		IMAGE_BOARD_FRAME = aMgr.GetImageThrow("IMAGE_BOARD_FRAME");
		IMAGE_BTN_MENU = aMgr.GetImageThrow("IMAGE_BTN_MENU");
		IMAGE_READY = aMgr.GetImageThrow("IMAGE_READY");
		IMAGE_WIN = aMgr.GetImageThrow("IMAGE_WIN");
		IMAGE_LOSE = aMgr.GetImageThrow("IMAGE_LOSE");
		IMAGE_ROUND_POINT = aMgr.GetImageThrow("IMAGE_ROUND_POINT");

		IMAGE_GSET_MODERN = aMgr.GetImageThrow("IMAGE_GSET_MODERN");
		IMAGE_GSET_CLASSIC = aMgr.GetImageThrow("IMAGE_GSET_CLASSIC");
		IMAGE_GSET_OTHER1 = aMgr.GetImageThrow("IMAGE_GSET_OTHER1");
		IMAGE_GSET_OTHER2 = aMgr.GetImageThrow("IMAGE_GSET_OTHER2");
		IMAGE_GSET_SECRET = aMgr.GetImageThrow("IMAGE_GSET_SECRET");
		IMAGE_BG_MODERN = aMgr.GetImageThrow("IMAGE_BG_MODERN");
		IMAGE_BG_CLASSIC = aMgr.GetImageThrow("IMAGE_BG_CLASSIC");
		IMAGE_BG_OTHER1 = aMgr.GetImageThrow("IMAGE_BG_OTHER1");
		IMAGE_BG_OTHER2 = aMgr.GetImageThrow("IMAGE_BG_OTHER2");
		IMAGE_BG_SECRET = aMgr.GetImageThrow("IMAGE_BG_SECRET");

		IMAGE_MAIN_MENU = aMgr.GetImageThrow("IMAGE_MAIN_MENU");
		IMAGE_BLUE_SKY = aMgr.GetImageThrow("IMAGE_BLUE_SKY");

		IMAGE_DIALOG = aMgr.GetImageThrow("IMAGE_DIALOG");
		IMAGE_DIALOG_BTN = aMgr.GetImageThrow("IMAGE_DIALOG_BTN");
		IMAGE_CHECKBOX = aMgr.GetImageThrow("IMAGE_CHECKBOX");
		IMAGE_SLIDER = aMgr.GetImageThrow("IMAGE_SLIDER");
		IMAGE_SLIDER_THUMB = aMgr.GetImageThrow("IMAGE_SLIDER_THUMB");

		IMAGE_MONST_1 = aMgr.GetImageThrow("IMAGE_MONST_1");

		SOUND_LAND = aMgr.GetSoundThrow("SOUND_LAND");
		SOUND_CHANGE = aMgr.GetSoundThrow("SOUND_CHANGE");
		SOUND_LEVELUP = aMgr.GetSoundThrow("SOUND_LEVELUP");
		SOUND_STRIKE = aMgr.GetSoundThrow("SOUND_STRIKE");
		SOUND_CRUSHBAR = aMgr.GetSoundThrow("SOUND_CRUSHBAR");
		SOUND_SMASH = aMgr.GetSoundThrow("SOUND_SMASH");
		SOUND_LOST = aMgr.GetSoundThrow("SOUND_LOST");
		SOUND_SELECT = aMgr.GetSoundThrow("SOUND_SELECT");
	} catch(ResourceManagerException&) {
		return false;
	}
	return true;
}

void Sexy::InitMusicTracks(MusicInterface *mi)
{
	mi->LoadMusic(MUSIC_TITLE_SCREEN,	"music/title.mp3");
	mi->LoadMusic(MUSIC_MAIN_MENU,		"music/mainmenu.mp3");
	mi->LoadMusic(MUSIC_MODERN,			"music/modern.mp3");
	mi->LoadMusic(MUSIC_CLASSIC,		"music/classic.mp3");
}

void Sexy::CloseMusicTracks(MusicInterface *mi)
{
	mi->UnloadAllMusic();
}

Image *Sexy::GetGSETImage(GSET_RESOURCE img, GSET set)
{
	switch (set) {
		case GSET_MODERN:
			switch (img) {
				case GSET_MAIN:		return IMAGE_GSET_MODERN;
				case GSET_BG:		return IMAGE_BG_MODERN;
			}
			break;
		case GSET_CLASSIC:
			switch (img) {
				case GSET_MAIN:		return IMAGE_GSET_CLASSIC;
				case GSET_BG:		return IMAGE_BG_CLASSIC;
			}
			break;
		case GSET_OTHER1:
			switch (img) {
				case GSET_MAIN:		return IMAGE_GSET_OTHER1;
				case GSET_BG:		return IMAGE_BG_OTHER1;
			}
			break;
		case GSET_OTHER2:
			switch (img) {
				case GSET_MAIN:		return IMAGE_GSET_OTHER2;
				case GSET_BG:		return IMAGE_BG_OTHER2;
			}
			break;
		case GSET_SECRET:
			switch (img) {
				case GSET_MAIN:		return IMAGE_GSET_SECRET;
				case GSET_BG:		return IMAGE_BG_SECRET;
			}
			break;
	}
	return NULL;
}

int Sexy::GetGSETSound(GSET_RESOURCE snd, GSET set)
{
	switch (set) {
		case GSET_MODERN:
			switch (snd) {
				case GSET_GETSET:	return SOUND_GETSET;
				case GSET_CHANGE:	return SOUND_CHANGE;
				case GSET_LAND:		return SOUND_LAND;
				case GSET_CRUSHBAR:	return SOUND_CRUSHBAR;
				case GSET_SMASH:	return SOUND_SMASH;
				case GSET_STRIKE:	return SOUND_STRIKE;
			}
			break;
		case GSET_CLASSIC:
			switch (snd) {
				case GSET_GETSET:	return SOUND_GETSET;
				case GSET_CHANGE:	return SOUND_CHANGE;
				case GSET_LAND:		return SOUND_LAND;
				case GSET_CRUSHBAR:	return SOUND_CRUSHBAR;
				case GSET_SMASH:	return SOUND_SMASH;
				case GSET_STRIKE:	return SOUND_STRIKE;
			}
			break;
		case GSET_OTHER1:
			switch (snd) {
				case GSET_GETSET:	return SOUND_GETSET;
				case GSET_CHANGE:	return SOUND_CHANGE;
				case GSET_CRUSHBAR:	return SOUND_CRUSHBAR;
				case GSET_SMASH:	return SOUND_SMASH;
				case GSET_STRIKE:	return SOUND_STRIKE;
			}
			break;
		case GSET_OTHER2:
			switch (snd) {
				case GSET_GETSET:	return SOUND_GETSET;
				case GSET_CHANGE:	return SOUND_CHANGE;
				case GSET_CRUSHBAR:	return SOUND_CRUSHBAR;
				case GSET_SMASH:	return SOUND_SMASH;
				case GSET_STRIKE:	return SOUND_STRIKE;
			}
			break;
		case GSET_SECRET:
			switch (snd) {
				case GSET_GETSET:	return SOUND_GETSET;
				case GSET_CHANGE:	return SOUND_CHANGE;
				case GSET_CRUSHBAR:	return SOUND_CRUSHBAR;
				case GSET_SMASH:	return SOUND_SMASH;
				case GSET_STRIKE:	return SOUND_STRIKE;
			}
			break;
	}
	return 0;
}

int Sexy::GetGSETMusic(GSET_RESOURCE msc, GSET set)
{
	return 0;
}