/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef GRID_H_
#define GRID_H_

#include "Core.h"

#define BOARD_WIDTH		6
#define BOARD_HEIGHT	16
#define VBOARD_HEIGHT	13

enum JEWEL
{
	JEWEL_NULL = -1,
	JEWEL_RED,
	JEWEL_YELLOW,
	JEWEL_BLUE,
	JEWEL_GREEN,
	JEWEL_PURPLE,
	JEWEL_ORANGE,
	JEWEL_SPECIAL,
	JEWEL_SPECIAL_ATTACK,
	JEWEL_SPECIAL_LOWER,
	JEWEL_CRUSHBAR,
};

struct Piece
{
	JEWEL jewel[3];
};

struct Grid
{
	JEWEL jewel[BOARD_WIDTH][BOARD_HEIGHT];
	bool crushed[BOARD_WIDTH][BOARD_HEIGHT];
	bool falling[BOARD_WIDTH];

	int CalculateMoveScore(Piece piece, int x);
	int CalculatePartialScore(Piece piece, int x, short direction);
	int SetCrushedJewels();
	int SetMatchedJewels(short direction);
	void DestroyCrushedJewels();
	void DropFloatingJewels();
	bool MarkFloatingJewels();
};

enum CONTROL
{
	CONTROL_NONE,
	CONTROL_SELECT,
	CONTROL_CHANGE,
	CONTROL_LEFT,
	CONTROL_RIGHT,
	CONTROL_DOWN_START,
	CONTROL_DOWN_END,
	CONTROL_ATTACK_LEFT,
	CONTROL_ATTACK_RIGHT,
	CONTROL_CHEAT_SPECIAL,
};

enum DIRECTION
{
	DIRECTION_HORIZONTAL = 32,
	DIRECTION_VERTICAL = 64,
	DIRECTION_DIAGONAL = 128,
};

struct BoardPositions
{
	int boardx,
		boardy,
		npx,
		npy;
};

//Board positions
struct {BoardPositions p1, p2, p3;} const BoardPoints[] = {
	{{294, 53, 243, 53}, {0, 0, 0, 0}, {0, 0, 0, 0}},	//1 player
	{{107, 53, 348, 53}, {465, 53, 414, 53}, {0, 0, 0, 0}}, //2 players
	{{11, 53, 242, 53}, {287, 53, 246, 216}, {561, 53, 521, 53}}, //3 players
};

bool AllMatch(JEWEL a, JEWEL b, JEWEL c);
bool PairMatch(JEWEL a, JEWEL b, JEWEL c);
bool IsJewel(JEWEL j);

#endif