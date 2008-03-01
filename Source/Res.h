/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef RES_H_
#define RES_H_

namespace Sexy
{
	class MusicInterface;
	class ResourceManager;
	class Image;
	class Font;

	//GSets
	enum GSET
	{
		GSET_MODERN,
		GSET_CLASSIC,
		GSET_OTHER1,
		GSET_OTHER2,
		GSET_SECRET,
		GSET_COUNT,
	};

	enum GSET_RESOURCE
	{
		GSET_MAIN,
		GSET_BG,
		GSET_GETSET,
		GSET_CHANGE,
		GSET_LAND,
		GSET_STRIKE,
		GSET_CRUSHBAR,
		GSET_SMASH,
	};

	//Musics
	enum MUSIC
	{
		MUSIC_TITLE_SCREEN,
		MUSIC_MAIN_MENU,
		MUSIC_MODERN,
		MUSIC_CLASSIC,
	};

	//Game Resources
	Image *GetGSETImage(GSET_RESOURCE img, GSET set);
	int GetGSETSound(GSET_RESOURCE snd, GSET set);
	int GetGSETMusic(GSET_RESOURCE msc, GSET set);

	bool ExtractTitleResources(ResourceManager *theMgr);
	bool ExtractGameResources(ResourceManager *theMgr);
	void InitMusicTracks(MusicInterface *mi);
	void CloseMusicTracks(MusicInterface *mi);

	extern Image* IMAGE_TITLE_SCREEN;
	extern Image* IMAGE_LOADING_BAR;
	extern Image* IMAGE_BTN_MENU_ADVENTURE_UP;
	extern Image* IMAGE_BTN_MENU_ADVENTURE_OVER;
	extern Image* IMAGE_BTN_MENU_ADVENTURE_DOWN;
	extern Image* IMAGE_BTN_MENU_QUICK_UP;
	extern Image* IMAGE_BTN_MENU_QUICK_OVER;
	extern Image* IMAGE_BTN_MENU_QUICK_DOWN;
	extern Image* IMAGE_BTN_MENU_OPTIONS_UP;
	extern Image* IMAGE_BTN_MENU_OPTIONS_OVER;
	extern Image* IMAGE_BTN_MENU_OPTIONS_DOWN;
	extern Image* IMAGE_BTN_MENU_QUIT_UP;
	extern Image* IMAGE_BTN_MENU_QUIT_OVER;
	extern Image* IMAGE_BTN_MENU_QUIT_DOWN;
	extern Image* IMAGE_STAGE_FRAME;
	extern Image* IMAGE_FRAME;
	extern Image* IMAGE_NP_FRAME;
	extern Image* IMAGE_SCORE_FRAME;
	extern Image* IMAGE_AS_FRAME;
	extern Image* IMAGE_OPPONENT_FRAME;
	extern Image* IMAGE_BOARD_FRAME;
	extern Image* IMAGE_READY;
	extern Image* IMAGE_WIN;
	extern Image* IMAGE_LOSE;
	extern Image* IMAGE_ROUND_POINT;
	extern Image* IMAGE_BTN_MENU;
	extern Image* IMAGE_DIALOG;
	extern Image* IMAGE_DIALOG_BTN;
	extern Image* IMAGE_CHECKBOX;
	extern Image* IMAGE_SLIDER;
	extern Image* IMAGE_SLIDER_THUMB;
	extern Image* IMAGE_BLUE_SKY;
	extern Image* IMAGE_MAIN_MENU;

	extern Image* IMAGE_GSET_MODERN;
	extern Image* IMAGE_GSET_CLASSIC;
	extern Image* IMAGE_GSET_OTHER1;
	extern Image* IMAGE_GSET_OTHER2;
	extern Image* IMAGE_GSET_SECRET;
	extern Image* IMAGE_BG_MODERN;
	extern Image* IMAGE_BG_CLASSIC;
	extern Image* IMAGE_BG_OTHER1;
	extern Image* IMAGE_BG_OTHER2;
	extern Image* IMAGE_BG_SECRET;

	extern Image* IMAGE_MONST_1;

	extern int SOUND_GETSET;
	extern int SOUND_CHANGE;
	extern int SOUND_LAND;
	extern int SOUND_LEVELUP;
	extern int SOUND_STRIKE;
	extern int SOUND_CRUSHBAR;
	extern int SOUND_SMASH;
	extern int SOUND_LOST;
	extern int SOUND_SELECT;
	extern Font* FONT_HUX;
	extern Font* FONT_ARIAL;
	extern Font* FONT_NORMAL;
}

#endif