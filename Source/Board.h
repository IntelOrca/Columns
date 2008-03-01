/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#ifndef BOARD_H_
#define BOARD_H_

#include "SexyAppFramework/Widget.h"

#include "AI.h"
#include "Grid.h"

///////////////////////////////////
//Macros /////////////////////////
///////////////////////////////////
#define JEWEL_SIZE		38
#define HJEWEL_SIZE		19
#define CRUSHING_ANIS	5
#define CRUSH_START_ANI	-6
#define PIXELS_PER_TICK	7.6f

#define START_COLUMN	2
#define MAX_CRUSHBARS	VBOARD_HEIGHT

#define JGRID(x, y)		mGrid.jewel[x][y]
#define CGRID(x, y)		mGrid.crushed[x][y]
#define FGRID(x)		mGrid.falling[x]
#define RND(low, high)	mRandom->Next(low, high)
#define CP_TO_GRID(y)	12 - (y / 2)

#define ANI				mAni1
#define ANI2			mAni2

/* States used for determining
 * what the gameturn routine is
 * meant to be calling.
 */
enum State
{
	STATE_NULL				= 0,

	STATE_WAITING			= 2,
	STATE_GETSET			= 4,
	STATE_PLAYING			= 8,
	STATE_CRUSHING			= 16,
	STATE_FALLING			= 32,
	STATE_SMASHING			= 64,
	STATE_ENDING			= 128,
	STATE_CLEAR_RAISING		= 256,
	STATE_CLEAR_LOWERING	= 512,
	STATE_GAME_OVER			= 1024,
};

namespace Sexy
{
	struct SmashedJewel
	{
		float mX, mY;
		float mVX, mVY;
		bool mShowing;

		SmashedJewel()
		{}
		SmashedJewel(float x, float y, float vx, float vy)
		{mX = x; mY = y; mVX = vx; mVY = vy; mShowing = true; }
	};

	struct GameData;
	class Game;
	class Buffer;
	class Random;
	class Board : public Widget
	{
	public:
		///////////////////////////////////
		//Declarations ///////////////////
		///////////////////////////////////
		Game *mGame;			//Pointer to the game
		GameData *mData;		//Pointer to the game data

		BoardPositions bps;		//Board position
		Board *mLeftBoard;		//Pointer to player to the left
		Board *mRightBoard;		//Pointer to player to the right

		const AICharacter *mPF;	//AI Character
		AI_TYPE mAIType;		//AI type
		AIMind mAIMind;			//AI mind (AI Storage)

		int mState;				//Current state
		Grid mGrid;				//Jewels in the grid
		Piece mNextpiece;		//The next piece
		Piece mCurrentpiece;	//Current piece
		Point mMBPos;			//Current piece position
		Random *mRandom;		//Random jewel sequence
		bool mDetectedLost;		//Lost on last lose detection

		int mCrushAnimation;			//The animation used for crushing
		float mFallAmount;				//How far the jewels have fallen
		float mRadians;					//Raidians used for ready sign (SIN)
		SmashedJewel mSmashJewel[3];	//Start position of each jewel
		int mAni1;						//Misc. Animations 1
		int mAni2;						//Misc. Animations 2

		int mCrushes;			//Current chain reaction
		int mCrushbars;			//Crushbar count
		int mAttackNeeded;		//Needs to attack as soon as it can
		int mCrushbarNeeded;	//Needs a crushbar as soon as it can

		int mLevel;				//Current Level
		int mStage;				//Current Stage
		int mScore;				//Current score
		int mPrescore;			//Current displayed score
		int mDestScore;			//Destination displayed score
		int mLevelOverflow;		//Endless level overflow
		int mScoreOverflow;		//Endless level score overflow
		int mJewelsCrushed;		//Total number of jewels crushed
		int mMoves;				//Total number of moves
		int mChains;			//Total number of chain reactions
		int mMaxChain;			//The maximum chain reaction
		int mAttackScore;		//Attack score

		int mColours;			//Number of colours
		int mMBSpeed;			//Speed of moving piece
		bool mDownKey;			//Fastdown or not

		///////////////////////////////////
		//Starting/Init methods //////////
		///////////////////////////////////
		Board(Game *theGame, GameData *data, int seed);
		void Start();
		void End();
		~Board();

		///////////////////////////////////
		//Widget methods //////////////////
		///////////////////////////////////
		void Update();
		void UpdateF(float theFrac);
		void Draw(Graphics *g);

		///////////////////////////////////
		//Piece methods //////////////////
		///////////////////////////////////
		void NewPiece();
		void LowerPiece();
		void PlacePiece();

		///////////////////////////////////
		//Crushing & Falling methods /////
		///////////////////////////////////
		int SetCrushedJewels();
		void StartCrushing();
		void CrushAnimation();
		void DestroyCrushedJewels();
		bool MarkFloatingJewels();
		void DropFloatingJewels();

		///////////////////////////////////
		//Control methods ////////////////
		///////////////////////////////////
		void Controller(CONTROL control);
		void RotateBlock();
		void MoveBlockLeft();
		void MoveBlockRight();
		void LandPiece();

		///////////////////////////////////
		//Attacking methods //////////////
		///////////////////////////////////
		void Attack(int side);
		void ReceiveAttack(int hits, bool dp);
		void RaiseCrushbar();
		void LowerCrushbar();

		void RaiseAndClear();
		void LowerAndClear();
		void SmashJewel();

		///////////////////////////////////
		//Misc. methods //////////////////
		///////////////////////////////////
		bool HasLost();
		void Clear();
		void ClearJewels();
		void CheckLevel();
		bool IsInTrouble();

		void CalculateTextY(float theFrac);
		void CalculateSmashingSpread(float theFrac);

		static Board *FromBuffer(Buffer *sav, GameData *data);
		void SaveToBuffer(Buffer *sav);

		///////////////////////////////////
		//Draw methods ///////////////////
		///////////////////////////////////
		void DrawBoard();
		void DrawMovingPiece();
		void DrawNextPiece();
		void DrawScores();
		void DrawJewel(int x, int y, int fall);
		void DrawCrush(int x, int y, float crush);
		void DrawMovingJewel(JEWEL j, int x, int y);
		void DrawReadyText();
		void DrawGameOverText();
		void DrawSmashedJewels();
		void DrawJewel(int x, int y);
	};
}

#endif