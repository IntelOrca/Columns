/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#define _USE_MATH_DEFINES

#include <math.h>
#include <SexyAppFramework\Buffer.h>

#include "Board.h"
#include "Res.h"
#include "Game.h"
#include "Levels.h"
#include "Random.h"

using namespace Sexy;

Board::Board(Game *theGame, GameData *data, int seed)
{
	//Initlize the mRandom number generator
	mGame = theGame;
	mData = data;
	mRandom = new Random(seed);
	mAIType = AI_TYPE_HUMAN;
	mDetectedLost = false;
	mScore = 0;
	mPrescore = 0;
	mDestScore = 0;
	mAttackScore = 0;
	mLevel = 1;
	mLevelOverflow = 0;
	mScoreOverflow = 0;
	mJewelsCrushed = 0;
	mMaxChain = 0;
	mChains = 0;
	mCrushbars = 0;
	mCrushbarNeeded = 0;
	mAttackNeeded = -1;
	mMoves = 0;
	mRadians = 0;
	mLeftBoard = NULL;
	mRightBoard = NULL;
	ANI2 = 0;
	bps.boardx = 0;
	bps.boardy = 0;
	bps.npx = 0;
	bps.npy = 0;
	mColours = 4;
	mMBSpeed = 60;
	mDownKey = false;

	if (mData->mMode == MODE_ADVENTURE) {
		const AdventureLevel *lvl = &ALevels[(mData->mLevel - 1) * A_LEVEL_PER_STAGE + (mData->mStage - 1)];
		mColours = lvl->Colours;
		mMBSpeed = lvl->MBSpeed;
		mPF = lvl->ai;
	}

	Clear();

	mDownKey = false;
	
	mNextpiece.jewel[0] = (JEWEL)RND(JEWEL_RED, JEWEL_RED + mColours - 1);
	mNextpiece.jewel[1] = (JEWEL)RND(JEWEL_RED, JEWEL_RED + mColours - 1);
	mNextpiece.jewel[2] = (JEWEL)RND(JEWEL_RED, JEWEL_RED + mColours - 1);
	mState = STATE_WAITING;
}

Board::~Board()
{

}

void Board::Update()
{
	if (mData->mPaused)
		return;

	mUpdateCnt++;
	
	//Increment ANI2
	if (mUpdateCnt % 5 == 0) {
		ANI2 = (ANI2 + 1) % 5;
	}

	//Increment visible mScore with catch up
	if (mUpdateCnt % 10 == 0) {
		if (mPrescore < mDestScore) {
			int il = mDestScore - mPrescore;
			if (il > 5000)
				mPrescore += 521;
			else if (il > 500)
					mPrescore += 51;
				else if (il > 100)
						mPrescore += 11;
					else if (il > 50)
							mPrescore += 5;
						else mPrescore++;
		}
	}

	//State
	if (mState & STATE_PLAYING) {
		if (mState == STATE_PLAYING) {
			if (mAIType != AI_TYPE_HUMAN) {
				if (mUpdateCnt % 20 == 0) {
					Controller(mAIMind.AIPlay(this));
				}
			}
			if (mDownKey) {
				if (mUpdateCnt % 3 == 0) {
					LowerPiece();
				}
			} else {
				if (mUpdateCnt % mMBSpeed == 0) {
					LowerPiece();
				}
			}
		} else if (mState & STATE_CRUSHING) {
			//Next crush mAni1 step
			if (mUpdateCnt % 7 == 0) {
				CrushAnimation();
			}
		} else if (mState & STATE_FALLING) {
			if (mUpdateCnt % 1 == 0) {
				DropFloatingJewels();
			}
		}
	} else if (mState & STATE_ENDING) {
		if (mState & STATE_CLEAR_RAISING) {
			if (mUpdateCnt % 9 == 0) {
				if (mAni1 < 7)
					mAni1++;
				else {
					if (mCrushbars < MAX_CRUSHBARS)
						RaiseCrushbar();
					else {
						ClearJewels();
						mState &= ~STATE_CLEAR_RAISING;
						mGame->PlayerEnded();
					}
				}
			}
		} else if (mState & STATE_CLEAR_LOWERING) {
			if (mUpdateCnt % 9 == 0) {
				if (mAni1 < 7)
					mAni1++;
				else {
					if (mCrushbars > 0)
						LowerCrushbar();
					else {
						mState &= ~STATE_CLEAR_LOWERING;
					}
				}
			}
		}
	}

	//case STATE_ENDING:
	//	if (mGTC % 9 == 0) {
	//		if (mAni1 < 7)
	//			mAni1++;
	//		else {
	//			if (mCrushbars < MAX_CRUSHBARS)
	//				RaiseCrushbar();
	//			else {
	//				mState = STATE_NULL;
	//				mGame->End();
	//			}
	//		}
	//	}
}

void Board::UpdateF(float theFrac)
{
	if (mState & STATE_SMASHING) {
		CalculateSmashingSpread(theFrac);
	}

	if (mState & STATE_WAITING ||
		mState & STATE_GAME_OVER) {
		CalculateTextY(theFrac);
	}
}

void Board::Controller(CONTROL control)
{
	switch (control) {
		case CONTROL_SELECT:
			if (mState & STATE_WAITING)
				mState &= ~STATE_WAITING;
			break;
		case CONTROL_CHANGE:
			RotateBlock();
			break;
		case CONTROL_LEFT:
			MoveBlockLeft();
			break;
		case CONTROL_RIGHT:
			MoveBlockRight();
			break;
		case CONTROL_DOWN_START:
			mDownKey = true;
			break;
		case CONTROL_DOWN_END:
			mDownKey = false;
			break;
		case CONTROL_ATTACK_LEFT:
			Attack(0);
			break;
		case CONTROL_ATTACK_RIGHT:
			Attack(1);
			break;
		case CONTROL_CHEAT_SPECIAL:
			mNextpiece.jewel[0] = JEWEL_SPECIAL;
			mNextpiece.jewel[1] = JEWEL_SPECIAL;
			mNextpiece.jewel[2] = JEWEL_SPECIAL;
			break;
	}
}

bool Board::IsInTrouble()
{
	int jewel_count = 0;
	for (int y = 5; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (IsJewel(mGrid.jewel[x][y])) {
				jewel_count++;
			}
		}
	}

	if (jewel_count > 8) {
		return true;
	} else {
		return false;
	}
}

void Board::Start()
{
	NewPiece();
}

void Board::End()
{
  	mAni1 = 0;
	mState = STATE_ENDING;

	//Smash the jewel if there is one
	if (mMBPos.mX >= 0 && mMBPos.mY >= 0) {
		SmashJewel();
	}

	//Make the crushbars go up
	RaiseAndClear();
}

/* Clears the whole board ready for a new
 * game.
 */
void Board::Clear()
{
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			JGRID(x, y) = JEWEL_NULL;
		}
	}
}

void Board::ClearJewels()
{
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = 0; y < BOARD_HEIGHT; y++) {
			if (JGRID(x, y) != JEWEL_CRUSHBAR)
				JGRID(x, y) = JEWEL_NULL;
		}
	}
}

/* Creates a new piece and sets it
 * up at the top of the board.
 */
void Board::NewPiece()
{
	mDestScore = mScore;
	if (HasLost()) {
		mDetectedLost = true;
		mGame->PlayASound(SOUND_LOST);
		End();
		return;
	}

	//Set current piece to next piece
	mCurrentpiece.jewel[0] = mNextpiece.jewel[0];
	mCurrentpiece.jewel[1] = mNextpiece.jewel[1];
	mCurrentpiece.jewel[2] = mNextpiece.jewel[2];

	//Next piece
	mNextpiece.jewel[0] = (JEWEL)RND(JEWEL_RED, JEWEL_RED + mColours - 1);
	mNextpiece.jewel[1] = (JEWEL)RND(JEWEL_RED, JEWEL_RED + mColours - 1);
	mNextpiece.jewel[2] = (JEWEL)RND(JEWEL_RED, JEWEL_RED + mColours - 1);

	//Increase mLevel
	if (mData->mMode == MODE_QUICKPLAY)
		CheckLevel();

	mMBPos.mX = START_COLUMN;
	mMBPos.mY = 0;
	mState = STATE_PLAYING;

	//If attack was sent while crushing/falling
	//receive the attack now
	if (mCrushbarNeeded > 0)
		ReceiveAttack(mCrushbarNeeded, false);

	//If attacked during crush/falling
	//attack now
	if (mAttackNeeded != -1)
		Attack(mAttackNeeded);

	//AI : Decide where to place piece
	mAIMind.AIThink(this);
}

/* Lowers the crushbar by one.
 */
void Board::CheckLevel()
{
	if (mScore-mScoreOverflow >= SPLevels[mLevel-mLevelOverflow].score) {
		while (mScore-mScoreOverflow >= SPLevels[mLevel-mLevelOverflow].score) {
			//Increment the level counter
			mLevel++;

			if (mLevel-mLevelOverflow >= LEVELCOUNT)
			{
				mLevelOverflow += 2;
				mScoreOverflow += SPLevels[20].score - SPLevels[19].score;
			}

			//Set the speed and colours to the level properties
			mMBSpeed = SPLevels[mLevel-mLevelOverflow].speed;
			mColours = SPLevels[mLevel-mLevelOverflow].colours;

			//If the player gets a prize (special)
			if (SPLevels[mLevel-mLevelOverflow].special) {
				//Set the next piece to a special
				mNextpiece.jewel[0] = JEWEL_SPECIAL;
				mNextpiece.jewel[1] = JEWEL_SPECIAL;
				mNextpiece.jewel[2] = JEWEL_SPECIAL;
			}
		}

		//Play the level up sound
		mGame->PlayASound(SOUND_LEVELUP);
	}
}

/* Places the current moving
 * block.
 */
void Board::PlacePiece()
{
	//Print mScore
	int movs = mGrid.CalculateMoveScore(mCurrentpiece, mMBPos.mX);
	mScore += movs;

	//Play landing sound
	mGame->PlayASound(GSET_LAND);
	int x = mMBPos.mX;
	int y = CP_TO_GRID(mMBPos.mY);
	JGRID(x, y) = mCurrentpiece.jewel[0];
	JGRID(x, y + 1) = mCurrentpiece.jewel[1];
	JGRID(x, y + 2) = mCurrentpiece.jewel[2];

	//Reset the MB Positions
	mMBPos.mX = -1;
	mMBPos.mY = -1;

	//Increment number of mMoves
	mMoves++;

	//AI stop going down
	if (mAIType == AI_TYPE_EXPERIMENT)
		mDownKey = false;

	//Reset number of mCrushes
	mCrushes = 0;
}

void Board::LandPiece()
{
	//Landed, jump to next stage
	PlacePiece();
	if (SetCrushedJewels() > 0)
		StartCrushing();
	else {
		NewPiece();
	}
}

/* Lowers the moving block half
 * a square down.
 */
void Board::LowerPiece()
{
	int *ypos = &mMBPos.mY;
	//Check if ypos is an odd number
	if (*ypos % 2 == 0) {
		//Lower block
		*ypos += 1;
	} else {
		//Check if there is a jewel below
		int x = mMBPos.mX;
		int y = CP_TO_GRID(*ypos);
		if ((!IsJewel(mGrid.jewel[x][y - 1]) && y > 0) && mGrid.jewel[x][y - 1] != JEWEL_CRUSHBAR) {
			//Lower block
			*ypos += 1;
		} else {
			LandPiece();
		}
	}
}

/* mMoves the current moving block
 * left if it is possible.
 */
void Board::MoveBlockLeft()
{
	if (mState == STATE_PLAYING) {
		//Make sure piece is not on a side
		if (mMBPos.mX > 0) {
			//Make sure piece is not next to a jewel
			if (!IsJewel(JGRID(mMBPos.mX - 1, CP_TO_GRID(mMBPos.mY)))) {
				mMBPos.mX -= 1;
			}
		}
	}
}

/* mMoves the current moving block
 * right if it is possible.
 */
void Board::MoveBlockRight()
{
	if (mState == STATE_PLAYING) {
		//Make sure piece is not on a side
		if (mMBPos.mX < BOARD_WIDTH - 1) {
			//Make sure piece is not next to a jewel
			if (!IsJewel(JGRID(mMBPos.mX + 1, CP_TO_GRID(mMBPos.mY)))) {
				mMBPos.mX += 1;
			}
		}
	}
}

/* Rotate the current moving block's
 * jewels around.
 */
void Board::RotateBlock()
{
	if (mState == STATE_PLAYING) {
		//Play rotate sound
		mGame->PlayASound(GSET_CHANGE);
		//Store one block as it will get overwitten
		int i = (int)mCurrentpiece.jewel[0];
		//Shift all the jewels up one
		mCurrentpiece.jewel[0] = mCurrentpiece.jewel[1];
		mCurrentpiece.jewel[1] = mCurrentpiece.jewel[2];
		mCurrentpiece.jewel[2] = (JEWEL)i;
	}
}

/* Raises the crushbar by one block.
 */
void Board::RaiseCrushbar()
{
	//Play the crushbar sound
	mGame->PlayASound(GSET_CRUSHBAR);

	//Shift all the jewels up one block
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = BOARD_HEIGHT - 1; y > 0; y--) {
			mGrid.jewel[x][y] = mGrid.jewel[x][y - 1];
		}

		//Set the bottom row to curshbar
		mGrid.jewel[x][0] = JEWEL_CRUSHBAR;
	}

	//Increment the crushbar counter
	mCrushbars++;
}

/* Lowers the crushbar by one block.
 */
void Board::LowerCrushbar()
{
	if (mCrushbars > 0) {
		//Play the crushbar sound
		mGame->PlayASound(GSET_CRUSHBAR);

		//Shift all the jewels down one block
		for (int x = 0; x < BOARD_WIDTH; x++) {
			for (int y = 0; y < BOARD_HEIGHT - 1; y++) {
				mGrid.jewel[x][y] = mGrid.jewel[x][y + 1];
			}
		}

		//Decrement the crushbar counter
		mCrushbars--;
	}
}

/* Get attacked with the number of hits and break the current jewel
 * if it is not busy.
 */
void Board::ReceiveAttack(int hits, bool dp)
{
	//Check if the board is busy
	if (mState != STATE_PLAYING) {
		//If it is busy, then make sure the player gets attacked when it's free
		mCrushbarNeeded = hits;
	} else {
		//For each 10 attack points raise the crushbar
		for (int i = 0; i < hits; i++) {
			RaiseCrushbar();
		}

		//If it was during play then do a nice spread effect
		if (dp) {
			SmashJewel();
		}
		mCrushbarNeeded = 0;
	}
}

/* Attacks the player on the specified side if
 * the conditions are right to do so.
 */
void Board::Attack(int side)
{
	//Checks if the board is currently busy
	if (mState != STATE_PLAYING) {
		//If busy then attack when it isn't
		mAttackNeeded = side;
		return;
	} else {
		mAttackNeeded = -1;
	}

	Board *b = NULL;
	int hits = mAttackScore / 10;

	//Check if the attack score is large enough
	if (hits > 0) {
		//Find a side that has a playing board
		if (side == 0) {
			if (mLeftBoard != NULL)
				b = mLeftBoard;
			else if (mRightBoard != NULL)
				b = mRightBoard;
		} else {
			if (mLeftBoard != NULL)
				b = mLeftBoard;
			else if (mRightBoard != NULL)
				b = mRightBoard;
		}

		//Check if we found a board to attack
		if (b != NULL) {
			//Send attack
			b->ReceiveAttack(hits, true);

			//Lower mCrushbars if any
			for (int i = 0; i < hits; i++) {
				LowerCrushbar();
			}

			//Decrease the attack score
			mAttackScore -= hits * 10;
		}
	}
}

void Board::RaiseAndClear()
{
	mState |= STATE_CLEAR_RAISING;
}

void Board::LowerAndClear()
{
	mState |= STATE_CLEAR_LOWERING;
}

void Board::SmashJewel()
{
	float vx, vy, angle[3];

	//Random jewel angles
	angle[0] = 90 + (Rand() % 45);		//90 - 135
	angle[1] = 45 + (Rand() % 45);		//45 - 90
	angle[2] = 45 + (Rand() % 90);		//45 - 135

	for (int i = 0; i < 3; i++) {
		angle[i] = ((int)angle[i] % 360) * M_PI / 180.0f;
		vx = cosf(angle[i]) * 4.0f;
		vy = -sinf(angle[i]) * 2.0f;
		mSmashJewel[i] = SmashedJewel(mMBPos.mX * 38, mMBPos.mY * 19 - (i * 38), vx, vy);
	}

	//Reset the moving piece position
	mMBPos.mX = -1;
	mMBPos.mY = -1;

	//Set to smashing state even though it's not a proper smash
	mState |= STATE_SMASHING;

	//Play the smash sound
	mGame->PlayASound(GSET_SMASH);
}

/* Checks to see if any jewels
 * are above the visable board
 * line.
 */
bool Board::HasLost()
{
	if (IsJewel(JGRID(START_COLUMN, VBOARD_HEIGHT - 1)))
		return true;
	for (int x = 0; x < BOARD_WIDTH; x++) {
		for (int y = VBOARD_HEIGHT; y < BOARD_HEIGHT; y++) {
			if (IsJewel(JGRID(x, y))) {
				return true;
			}
		}
	}
	return false;
}

/* Starts the crushing stage.
 */
void Board::StartCrushing()
{
	mCrushes += 1;
	mState |= STATE_CRUSHING;
}

/* Called each time a crush
 * mAni1 tick has gone.
 * Increments the mAni1
 * or processes the next stage.
 */
void Board::CrushAnimation()
{
	if (mCrushAnimation == -1) {
		//Add chain reaction scores
		if (mCrushes > 1) {
			if (mMaxChain < mCrushes)
				mMaxChain = mCrushes - 1;
			mChains += mCrushes - 1;
		}

		//Play chain reaction sound
		mGame->PlayStrikeSound(mCrushes);
	}
	if (mCrushAnimation >= CRUSHING_ANIS - 1) {
		mState &= ~STATE_CRUSHING;

		//Actually destory the jewels
		DestroyCrushedJewels();
		//Check if there are pieces to drop
		if (MarkFloatingJewels()) {
			//Change the mState to falling
			mState |= STATE_FALLING;
		} else {
			//No pieces to drop, call new piece
			NewPiece();
		}
	} else {
		//Increment the mAni1 cycle
		mCrushAnimation++;
	}
}

/* Sets all the jewels that
 * are marked to nothing.
 */
void Board::DestroyCrushedJewels()
{
	for (int x = 0; x < BOARD_WIDTH; x++) {
			for (int y = 0; y < BOARD_HEIGHT; y++) {
				if (CGRID(x, y)) {
					mJewelsCrushed++;
					if (mAttackScore < 30)
						mAttackScore++;
				}
			}
	}
	mGrid.DestroyCrushedJewels();
}

/* Returns and marks the jewels
 * that need crushing. (I.E. jewels
 * that are in a row of three.)
 */
int Board::SetCrushedJewels()
{
	//Start the mAni1 start cycle
	mCrushAnimation = CRUSH_START_ANI;
	return mGrid.SetCrushedJewels();
}

/* Called each time a falling tick
 * has gone. Increments the mAni1
 * cycle or processes the next task.
 */
void Board::DropFloatingJewels()
{
	if (mFallAmount >= JEWEL_SIZE) {
		//Drop marked jewels
		mGrid.DropFloatingJewels();

		//Drop them again until theres no more spaces
		if (MarkFloatingJewels()) {
			mState |= STATE_FALLING;
		} else {
			mState &= ~STATE_FALLING;

			if (SetCrushedJewels() > 0)
				StartCrushing();
			else
				NewPiece();
		}
	} else {
		//Increment the mAni1 cycle
		mFallAmount += PIXELS_PER_TICK;
	}
}

/* Return and mark all the floating
 * jewels.
 */
bool Board::MarkFloatingJewels()
{
	mFallAmount = 0.0f;
	return mGrid.MarkFloatingJewels();
}

void Board::CalculateTextY(float theFrac)
{
	const int pixelrange = 60;
    mRadians = (mRadians + (M_PI * 0.4f * theFrac) / pixelrange);
	if (mRadians > 6.28f)
		mRadians = 0.0f;
	mAni1 = (sin(mRadians) * (pixelrange / 2));
}

void Board::CalculateSmashingSpread(float theFrac)
{
	int not_showing = 0;

	for (int i = 0; i < 3; i++) {
		SmashedJewel *j = &mSmashJewel[i];
		j->mX += j->mVX * theFrac;
		j->mY += j->mVY * theFrac;
		j->mVY += 0.3f;

		//Check if it is off the screen
		if (j->mX > mGame->mWidth || j->mY > mGame->mHeight) {
			j->mShowing = false;
			not_showing++;
		}
	}

	//Check if all the pieces are off the screen
	if (not_showing >= 3) {
		mState &= ~STATE_SMASHING;
		if (mState & STATE_PLAYING) {
			//New piece
			NewPiece();
		}
	}
}

Board *Board::FromBuffer(Buffer *sav, GameData *data)
{
	Board *lb = new Board(NULL, data, -1);

	lb->mAIType = (AI_TYPE)sav->ReadLong();

	//Read AI Mind structure
	lb->mAIMind.mRotates = sav->ReadByte();
	lb->mAIMind.mColumn = sav->ReadByte();
	lb->mAIMind.mPanicking = sav->ReadBoolean();
	for (int i = 0; i < BOARD_WIDTH; i++)
		lb->mAIMind.mIgnoreColumn[i] = sav->ReadBoolean();

	lb->mState = (State)sav->ReadLong();
	sav->ReadBytes((uchar*)&lb->mGrid, sizeof(Grid));
	sav->ReadBytes((uchar*)&lb->mNextpiece, sizeof(Piece));
	sav->ReadBytes((uchar*)&lb->mCurrentpiece, sizeof(Piece));
	lb->mMBPos.mX = sav->ReadLong();
	lb->mMBPos.mY = sav->ReadLong();
	lb->mRandom = new Random(sav->ReadLong());
	lb->mDetectedLost = sav->ReadBoolean();
	lb->mUpdateCnt = sav->ReadLong();

	for (int i = 0; i < 3; i++) {
		lb->mSmashJewel[i].mX = sav->ReadLong();
		lb->mSmashJewel[i].mY = sav->ReadLong();
		lb->mSmashJewel[i].mVX = sav->ReadLong();
		lb->mSmashJewel[i].mVY = sav->ReadLong();
		lb->mSmashJewel[i].mShowing = sav->ReadBoolean();
	}

	lb->mCrushAnimation = sav->ReadLong();
	*((int*)(&lb->mFallAmount)) = sav->ReadLong();
	lb->mAni1 = sav->ReadLong();
	lb->mAni2 = sav->ReadLong();
	lb->mCrushes = sav->ReadLong();
	lb->mCrushbars = sav->ReadLong();
	lb->mAttackNeeded = sav->ReadLong();
	lb->mCrushbarNeeded = sav->ReadLong();
	lb->mLevel = sav->ReadLong();
	lb->mStage = sav->ReadLong();
	lb->mScore = sav->ReadLong();
	lb->mPrescore = sav->ReadLong();
	lb->mDestScore = sav->ReadLong();
	lb->mLevelOverflow = sav->ReadLong();
	lb->mScoreOverflow = sav->ReadLong();
	lb->mJewelsCrushed = sav->ReadLong();
	lb->mMoves = sav->ReadLong();
	lb->mChains = sav->ReadLong();
	lb->mMaxChain = sav->ReadLong();
	lb->mAttackScore = sav->ReadLong();
	lb->mColours = sav->ReadLong();
	lb->mMBSpeed = sav->ReadLong();

	return lb;
}

void Board::SaveToBuffer(Buffer *sav)
{
	sav->WriteLong(mAIType);

	//Write AI Mind structure
	sav->WriteByte(mAIMind.mRotates);
	sav->WriteByte(mAIMind.mColumn);
	sav->WriteBoolean(mAIMind.mPanicking);
	for (int i = 0; i < BOARD_WIDTH; i++)
		sav->WriteBoolean(mAIMind.mIgnoreColumn[i]);

	sav->WriteLong(mState);
	sav->WriteBytes((uchar*)&mGrid, sizeof(Grid));
	sav->WriteBytes((uchar*)&mNextpiece, sizeof(Piece));
	sav->WriteBytes((uchar*)&mCurrentpiece, sizeof(Piece));
	sav->WriteLong(mMBPos.mX);
	sav->WriteLong(mMBPos.mY);
	sav->WriteLong(mRandom->mLastValue);
	sav->WriteBoolean(mDetectedLost);
	sav->WriteLong(mUpdateCnt);
	for (int i = 0; i < 3; i++) {
		sav->WriteLong(mSmashJewel[i].mX);
		sav->WriteLong(mSmashJewel[i].mY);
		sav->WriteLong(mSmashJewel[i].mVX);
		sav->WriteLong(mSmashJewel[i].mVY);
		sav->WriteBoolean(mSmashJewel[i].mShowing);
	}

	sav->WriteLong(mCrushAnimation);
	sav->WriteLong(*((int*)(&mFallAmount)));
	sav->WriteLong(mAni1);
	sav->WriteLong(mAni2);
	sav->WriteLong(mCrushes);
	sav->WriteLong(mCrushbars);
	sav->WriteLong(mAttackNeeded);
	sav->WriteLong(mCrushbarNeeded);
	sav->WriteLong(mLevel);
	sav->WriteLong(mStage);
	sav->WriteLong(mScore);
	sav->WriteLong(mPrescore);
	sav->WriteLong(mDestScore);
	sav->WriteLong(mLevelOverflow);
	sav->WriteLong(mScoreOverflow);
	sav->WriteLong(mJewelsCrushed);
	sav->WriteLong(mMoves);
	sav->WriteLong(mChains);
	sav->WriteLong(mMaxChain);
	sav->WriteLong(mAttackScore);
	sav->WriteLong(mColours);
	sav->WriteLong(mMBSpeed);
}

/////////////////////////////////////
//Diagram of the grid and its indexes
//   _______________
//   || | | | | | ||  12   1
//   || | | | | | ||  11   3
//   || | | | | | ||  10   5
//   || | | | | | ||   9   7
//   || | | | | | ||   8   9
//   || | | | | | ||   7  11
//   || | | | | | ||   6  13
//   || | | | | | ||   5  15
//   || | | | | | ||   4  17
//   || | | | | | ||   3  29
//   || | | | | | ||   2  31
//   || | | | | | ||   1  33
//   || | | | | | ||   0  35
//   ---------------
//     0 1 2 3 4 5 
/////////////////////////////////////