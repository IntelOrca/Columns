/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "board.h"
#include "ai.h"

using namespace Sexy;

Piece AIMind::RotateBlock(Piece *block)
{
	//Store one block as it will get overwitten
	int i = (int)block->jewel[0];
	//Shift all the jewels up one
	block->jewel[0] = block->jewel[1];
	block->jewel[1] = block->jewel[2];
	block->jewel[2] = (JEWEL)i;
	return *block;
}

bool AIMind::IsPanicking(Grid *grid)
{
	if (CountofNullJewels(grid) < 0)
		return true;
	else
		return false;
}

const AIProfile *AIMind::GetCurrentAIProfile()
{
	if (mPanicking)
		return &mBoard->mPF->Panic;
	else
		return &mBoard->mPF->Normal;
}

void AIMind::AIThink(Board *board)
{
	mBoard = board;

	if (board->mAIType == AI_TYPE_HUMAN) return;

	//Check if the ai is panicking or not
	mPanicking = IsPanicking(&mBoard->mGrid);
	const AIProfile *cf = GetCurrentAIProfile();

	//PREPARE AND COMBINE ALL THE STRATEGIES
	int score = 0, pscore = 0;
	//Initilize columns to ignore
	for (int i = 0; i < BOARD_WIDTH; i++) {
		mIgnoreColumn[i] = false;
	}
	
	if (cf->Strategy & AP_STRATEGY_COLOUR_SORTING)
		StrategyColourSorting();

	if (cf->Strategy & AP_STRATEGY_TOWERS_OUTSIDE)
		StrategyOutsideTowers();

	if (cf->Strategy & AP_STRATEGY_TOWERS_INSIDE)
		StrategyInsideTowers();

	//Lowest non zero score drop
	if (cf->Strategy & AP_STRATEGY_LOWESTSCORE)
		score = LowestScore();

	//Highest Score drop
	if (cf->Strategy & AP_STRATEGY_HIGHESTSCORE) {
		score = HighestScore();
	}
	if (cf->Strategy & (AP_STRATEGY_PAIR_HORIZONTAL | AP_STRATEGY_PAIR_VERTICAL | AP_STRATEGY_PAIR_DIAGONAL)) {
		//Matched pair drop, but only if no highest score
		if (score == 0)   
			pscore = PartialScore((short)(cf->Strategy & (	AP_STRATEGY_PAIR_HORIZONTAL | 
															AP_STRATEGY_PAIR_VERTICAL | 
															AP_STRATEGY_PAIR_DIAGONAL)));
	}

	int maxheight = MAX_TOWER_HEIGHT;
	if (cf->Strategy & AP_STRATEGY_LOWEST_COLUMN) {
	    //Lowest column drop if not already decided
		if (score + pscore == 0)
			mColumn = START_COLUMN - LowestColumn(); // will be overridden if RANDOM_COLUMN
			maxheight = ColumnHeight(&board->mGrid, mColumn);
	}

	if (cf->Strategy & AP_STRATEGY_RANDOM_COLUMN) {
	    //Random column drop if not already decided
		if (score + pscore == 0) 
			mColumn = START_COLUMN - RandomColumn(maxheight);
	}

	if (cf->Strategy & AP_STRATEGY_RANDOM_ROTATE) {
	    //Random rotate if not already decided
		if (score + pscore == 0) 
			mRotates = Rand(3);
	}

	//Work out if the down key is going to be used this go
	if (cf->Fastdown & AP_FASTDOWN_ALWAYS) {
		mDown = true;
	} else {
		mDown = false;
		if (cf->Fastdown & AP_FASTDOWN_RANDOM_2) {
			if (Rand(2) == 1)
				mDown = true;
		} else if (cf->Fastdown & AP_FASTDOWN_RANDOM_4) {
			if (Rand(4) == 1)
				mDown = true;
		} else if (cf->Fastdown & AP_FASTDOWN_RANDOM_8) {
			if (Rand(8) == 1)
				mDown = true;
		}
	}

	//Rotation modifications
	if (cf->Rotate & AP_ROTATE_RANDOM)
		mRotates += rand() % 3;
	if (cf->Rotate & AP_ROTATE_NERVOUS)
		mRotates += 3;	//Cause 3 extra rotates
	//if (!(cf->Rotate & AP_ROTATE_ALWAYS))
	//	mRotates = 0;		//Never rotate

}

CONTROL AIMind::AIPlay(Board *board)
{
	mBoard = board;

	if (board->mAIType == AI_TYPE_HUMAN) return CONTROL_NONE;

	const AIProfile *cf;
	if (mPanicking)
		cf = &mBoard->mPF->Panic;
	else
		cf = &mBoard->mPF->Normal;

	bool movingcomplete = false;
	bool rotatecomplete = false;

	//Move and Rotate booleans
	if (mColumn == 0)
		movingcomplete = true;
	if (mRotates == 0)
		rotatecomplete = true;

	//Down key
	if (!mBoard->mDownKey) {
		if (mDown) {
			if ((cf->Fastdown & AP_FASTDOWN_WAITUNTILDONE && movingcomplete && rotatecomplete) ||
				!(cf->Fastdown & AP_FASTDOWN_WAITUNTILDONE)) {
				if (!(cf->Fastdown & AP_FASTDOWN_DELAY) || (cf->Fastdown & AP_FASTDOWN_DELAY && board->mMBPos.mY > 10)) {
					return CONTROL_DOWN_START;
				}
			}
		}
	}

	//Move and Rotate
	if (!(cf->Rotate & AP_ROTATE_SLOW && (board->mUpdateCnt % 80 != 0))) {
		if (mColumn < 0) {
			mColumn += 1;
			return CONTROL_RIGHT;
		} else if (mColumn > 0) {
			mColumn -= 1;
			return CONTROL_LEFT;
		} else if (mRotates > 0) {
			mRotates -= 1;
			return CONTROL_CHANGE;
		}
	}

	//Attack
	int delay = 0; 
	if (cf->Attack & AP_ATTACK_DELAY) {
		delay = 5 + Rand(5);
	}
	if (cf->Attack & AP_ATTACK_FORCE1 && board->mAttackScore >= 10 + delay)
		return CONTROL_ATTACK_LEFT;
	if (cf->Attack & AP_ATTACK_FORCE2 && board->mAttackScore >= 20 + delay)
		return CONTROL_ATTACK_LEFT;
	if (cf->Attack & AP_ATTACK_FORCE3 && board->mAttackScore >= 30)
		return CONTROL_ATTACK_LEFT;
	if (cf->Attack & AP_ATTACK_RETALIATION && board->mCrushbars > 0 && board->mAttackScore >= 10)
		return CONTROL_ATTACK_LEFT;

	//No action
	return CONTROL_NONE;

}

void AIMind::StrategyColourSorting()
{
	Piece block = mBoard->mCurrentpiece;
	int leftjewel = JEWEL_BLUE;
	int rightjewel = JEWEL_RED;
	int leftright = -1, ctleftjewel = 0, ctrightjewel = 0;
	for (int j = 0; j < 3; j++) {
		if (block.jewel[j] == rightjewel)
			ctrightjewel++;
		else if (block.jewel[j] == leftjewel)
			ctleftjewel++; 
	}
	if (ctleftjewel > 1)
		leftright = 0;	//left hand side of board
	else { 
		if (ctrightjewel > 1)
			leftright = 3;	//right hand side of board
		else {
			if (ctleftjewel > 0)
				leftright = 0;	//left hand side of board
			else {
				if (ctrightjewel > 0)
					leftright = 3;	//right hand side of board
				else
					leftright = -1;	//neither colour present
			}
		}
	}
	//Select left or right side of the board if there is space
	if (leftright >= 0) {
		if (ColumnHeight(&mBoard->mGrid, 0+leftright) <= TOWER_HEIGHT - 3 |
			ColumnHeight(&mBoard->mGrid, 1+leftright) <= TOWER_HEIGHT - 3 |
			ColumnHeight(&mBoard->mGrid, 2+leftright) <= TOWER_HEIGHT - 3 ) {
			mIgnoreColumn[3-leftright] = true;
			mIgnoreColumn[4-leftright] = true;
			mIgnoreColumn[5-leftright] = true;
		}				
	}
}

void AIMind::StrategyOutsideTowers()
{
	//Build outside towers if there is space
	if (ColumnHeight(&mBoard->mGrid, 0) <= TOWER_HEIGHT - 3 |
		ColumnHeight(&mBoard->mGrid, 1) <= TOWER_HEIGHT - 3 |
		ColumnHeight(&mBoard->mGrid, 4) <= TOWER_HEIGHT - 3 |
		ColumnHeight(&mBoard->mGrid, 5) <= TOWER_HEIGHT - 3 ) {
		mIgnoreColumn[2] = true;
		mIgnoreColumn[3] = true;
	}
}

void AIMind::StrategyInsideTowers()
{
	//Build inside towers if there is space
	if (ColumnHeight(&mBoard->mGrid, 1) <= TOWER_HEIGHT - 3 |
		ColumnHeight(&mBoard->mGrid, 2) <= TOWER_HEIGHT - 3 |
		ColumnHeight(&mBoard->mGrid, 3) <= TOWER_HEIGHT - 3 |
		ColumnHeight(&mBoard->mGrid, 4) <= TOWER_HEIGHT - 3 ) {
			mIgnoreColumn[0] = true;
			mIgnoreColumn[5] = true;
	}
}

int AIMind::ColumnHeight(Grid *grid, int x)
{
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		if (!IsJewel(grid->jewel[x][y]) && grid->jewel[x][y] != JEWEL_CRUSHBAR)
			return y;
	}
	return -1;
}

int AIMind::CountofNullJewels(Grid *grid)
{
	int nulls = 0;
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < VBOARD_HEIGHT; y++) {
			if (grid->jewel[x][y] == JEWEL_NULL)
				nulls++;
		}
	}
	return nulls;

}

int AIMind::LowestColumn()
{
	int i = 0,
		h = BOARD_HEIGHT;
	for (int x = 0; x < BOARD_WIDTH; x++) {
		if (!mIgnoreColumn[x]) {
			int ch = ColumnHeight(&mBoard->mGrid, x);
			if (h > ch) {
				i = x;
				h = ch;
			}
		}
	}
	return i;
}

int AIMind::RandomColumn(int maxcolumnheight)
{
	int h, collist[6], index = 0;
	for (int x = 0; x < BOARD_WIDTH; x++) {
		if (!mIgnoreColumn[x]) {
			h = ColumnHeight(&mBoard->mGrid, x);
			if (h <= maxcolumnheight) {
				collist[index] = x;		//Keep list of columns not close to top
				index++;
			}
		}
	}
	if (index == 0)
		return LowestColumn();	//If all columns high, chose lowest
	return collist[Rand(index)];	//Otherwise choose randomly from list
}

int AIMind::HighestScore()
{
	Piece block = mBoard->mCurrentpiece;

	//Check to see which possible move gives the best score
	int x = 0, rotates = 0, xscore = 0;

	for (int r = 0; r < 3; r++) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (!mIgnoreColumn[i]) {
				int j = mBoard->mGrid.CalculateMoveScore(block, i);
				if (j > xscore) {
					x = i;
					xscore = j;
					rotates = r;
				}
			}
		}
		RotateBlock(&block);
	}

	mColumn = START_COLUMN - x;
	mRotates = rotates;
	return xscore;
}

int AIMind::LowestScore()
{
	Piece block = mBoard->mCurrentpiece;

	//Check to see which possible move gives the lowest non zero score
	int x = 0, rotates = 0, xscore = 0;

	for (int r = 0; r < 3; r++) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (!mIgnoreColumn) {
				int j = mBoard->mGrid.CalculateMoveScore(block, i);
				if (j > 0 && xscore == 0) {
					x = i;
					xscore = j;
					rotates = r;
				}
				if (j > 0 && j < xscore) {
					x = i;
					xscore = j;
					rotates = r;
				}
			}
		}
		RotateBlock(&block);
	}

	mColumn = START_COLUMN - x;
	mRotates = rotates;
	return xscore;
}

int AIMind::PartialScore(short direction)
{
	//Calculates a score for matching pairs (but not trios) of jewels
	Piece block = mBoard->mCurrentpiece;
	if (block.jewel[0] == block.jewel[1] && block.jewel[0] == block.jewel[2])
		return -1; //falling jewel will self crush

	//Check to see which possible move gives the best partial score
	int x = 0, rotates = 0, pscore = 0;
	bool pscorevalid = false;
	for (int r = 0; r < 3; r++) {
		for (int i = 0; i < BOARD_WIDTH; i++) {
			if (!mIgnoreColumn[i]) {
				int j = mBoard->mGrid.CalculatePartialScore(block, i, direction);
				if (j < pscore && j >=0) //pscore is invalid if it is the same value on every iteration
					pscorevalid = true;
				if (j > pscore) {
					x = i;
					pscore = j;
					rotates = r;
					if (block.jewel[0] == block.jewel[1] || block.jewel[1] == block.jewel[2]) 
						pscore-=2; //subtract the score of the falling piece
				}
			}
		}
		RotateBlock(&block);
	}

	mColumn = START_COLUMN - x;
	mRotates = rotates;
	if (pscorevalid)
		return pscore;
	else
		return 0; //no new pairs found
}