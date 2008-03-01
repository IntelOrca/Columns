/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef AI_H_
#define AI_H_

#include "Grid.h"

#pragma region Profiles

#define AP_NULL	0

namespace Sexy
{
	enum AP_FASTDOWN
	{
		AP_FASTDOWN_ALWAYS = 1,
		AP_FASTDOWN_IFNOROTATE = 2,
		AP_FASTDOWN_RANDOM_2 = 4,	//1 in 2 chance of fastdown
		AP_FASTDOWN_RANDOM_4 = 8,   //1 in 4 
		AP_FASTDOWN_RANDOM_8 = 16,  //1 in 8
		AP_FASTDOWN_DELAY = 32,
		AP_FASTDOWN_WAITUNTILDONE = 64,
	};

	enum AP_ROTATE
	{
		AP_ROTATE_ALWAYS = 1,
		AP_ROTATE_RANDOM = 2,		//Rotates randomly
		AP_ROTATE_NERVOUS = 4,		//Does unnecessary rotations
		AP_ROTATE_SLOW = 8,			//Rotates slowly
	};

	enum AP_ATTACK
	{
		AP_ATTACK_NEVER = 1,
		AP_ATTACK_FORCE1 = 2,
		AP_ATTACK_FORCE2 = 4,
		AP_ATTACK_FORCE3 = 8,
		AP_ATTACK_RETALIATION = 16,
		AP_ATTACK_DEFENCE = 32,
		AP_ATTACK_DELAY = 64,
		AP_ATTACK_SPECIAL = 128
	};
	enum AP_STRATEGY				//Frequency as later option to mix strategies 	
	{
		AP_STRATEGY_LOWEST_COLUMN = 1,		// col   
		AP_STRATEGY_RANDOM_COLUMN = 2,		// col
		AP_STRATEGY_RANDOM_ROTATE = 4,		// rotate
		AP_STRATEGY_LOWESTSCORE = 8,		// both
		AP_STRATEGY_HIGHESTSCORE = 16,		// both
		AP_STRATEGY_PAIR_HORIZONTAL = 32,	// both
		AP_STRATEGY_PAIR_VERTICAL = 64,		// both
		AP_STRATEGY_PAIR_DIAGONAL = 128,	// both
		AP_STRATEGY_TOWERS_OUTSIDE = 256,	// col 
		AP_STRATEGY_TOWERS_INSIDE = 512,	// col
		AP_STRATEGY_COLOUR_SORTING = 1024,	// col
	};

	struct AIProfile
	{
		char Fastdown;
		char Rotate;
		char Attack;
		short Strategy;
	};

	struct AICharacter
	{
		AIProfile Normal;
		AIProfile Panic;
	};

	const AICharacter AIChars[] = {
		////////////////////////////////////////////
		//Level 1 /////////////////////////////////
		////////////////////////////////////////////
		  //Normal
		{{AP_FASTDOWN_ALWAYS | AP_FASTDOWN_WAITUNTILDONE,
		  AP_ROTATE_ALWAYS, 
		  AP_ATTACK_DELAY | AP_ATTACK_FORCE1,
		  AP_STRATEGY_HIGHESTSCORE | AP_STRATEGY_PAIR_HORIZONTAL | AP_STRATEGY_PAIR_DIAGONAL | AP_STRATEGY_LOWEST_COLUMN | AP_STRATEGY_RANDOM_ROTATE},
		//Panic
		 {AP_FASTDOWN_ALWAYS | AP_FASTDOWN_WAITUNTILDONE,
		  AP_ROTATE_ALWAYS,
		  AP_ATTACK_FORCE1,
		  AP_STRATEGY_HIGHESTSCORE | AP_STRATEGY_PAIR_HORIZONTAL | AP_STRATEGY_PAIR_DIAGONAL | AP_STRATEGY_LOWEST_COLUMN}},
	};
}

#pragma endregion

#define TOWER_HEIGHT		10
#define MAX_TOWER_HEIGHT	9	//Set maximum column height for random column drop or height of lowest column

namespace Sexy
{
	enum AI_TYPE
	{
		AI_TYPE_HUMAN,
		AI_TYPE_EXPERIMENT,
	};

	class Board;
	class AIMind
	{
	private:
		Board *mBoard;

	public:
		char mRotates;
		char mColumn;
		bool mPanicking;
		bool mDown;
		bool mIgnoreColumn[BOARD_WIDTH];

	public:
		//Main Starting Functions
		void AIThink(Board *board);
		CONTROL AIPlay(Board *board);

		Piece RotateBlock(Piece *block);
		int ColumnHeight(Grid *grid, int x);
		int CountofNullJewels(Grid *grid);
		bool IsPanicking(Grid *grid);
		const AIProfile *GetCurrentAIProfile();
		void StrategyColourSorting();
		void StrategyOutsideTowers();
		void StrategyInsideTowers();
		int LowestColumn();
		int HighestScore();
		int LowestScore();
		int PartialScore(short direction);
		int RandomColumn(int maxcolumnheight);
	};
}

#endif