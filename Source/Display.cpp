/****************************
 * Columns                  *
 * Copyright Ted John 2008  *
 * http://tedtycoon.co.uk   *
 ****************************/

#include "SexyAppFramework/Font.h"
#include "SexyAppFramework/Graphics.h"
#include "SexyAppFramework/MemoryImage.h"
#include "GameApp.h"
#include "Game.h"
#include "Res.h"
#include "board.h"

#define IMAGE_MODERN	GetGSETImage(GSET_MAIN, mGame->mData->mSet)

using namespace Sexy;

Graphics *gg;
Rect GetJewelSprite(JEWEL j, int ani);

Rect GetJewelSprite(JEWEL j, int ani)
{
	if (j == JEWEL_SPECIAL)
		if (ani < 5)
			return Rect(ani * JEWEL_SIZE, j * JEWEL_SIZE, JEWEL_SIZE, JEWEL_SIZE);
	return Rect(0, j * JEWEL_SIZE, JEWEL_SIZE, JEWEL_SIZE);
}

void Board::Draw(Graphics *g)
{
	gg = g;

	//Clip anything that is drawn outside the board
	g->ClipRect(0, 0, BOARD_WIDTH * JEWEL_SIZE, VBOARD_HEIGHT * JEWEL_SIZE);

	if (mState & STATE_WAITING) {
		DrawReadyText();
	} else {
		DrawBoard();
		if (!mData->mPaused)
			DrawMovingPiece();
	}

	//Stop clipping
	g->ClearClipRect();
	if (mData->mPlaying) {
		if (!mData->mPaused) {
			DrawNextPiece();
			if (mState & STATE_SMASHING)
				DrawSmashedJewels();
		}
	}

	if (mState & STATE_GAME_OVER) {
		DrawGameOverText();
	}

	DrawScores();
}

void Board::DrawSmashedJewels()
{
	for (int i = 0; i < 3; i++) {
		JEWEL j = mCurrentpiece.jewel[i];

		if (mSmashJewel[i].mShowing) {
			gg->DrawImage(IMAGE_MODERN, mSmashJewel[i].mX, mSmashJewel[i].mY, GetJewelSprite(j, 0));
		}
	}
}

void Board::DrawBoard()
{
	//Draw the board backdrop
	gg->DrawImage(IMAGE_MODERN, 0, 0, Rect(418, 0, BOARD_WIDTH * JEWEL_SIZE, VBOARD_HEIGHT * JEWEL_SIZE));

	if (mGame->mData->mPaused) {
		gg->SetColor(Color::White);
		gg->SetFont(FONT_NORMAL);
		int x = gg->StringWidth("PAUSED");
		gg->DrawString("PAUSED", ((BOARD_WIDTH * JEWEL_SIZE) / 2) - (x / 2), ((VBOARD_HEIGHT * JEWEL_SIZE) / 2) - (FONT_NORMAL->mHeight / 2));
		return;
	}

	//Cycle through the board and draw each jewel
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++) {
			if (IsJewel(JGRID(x, y))) {
				//Draw jewel with mAni1 or not
				if (mState & STATE_CRUSHING) {
					if (mCrushAnimation < 0) {
						//Draw crushing/flashing jewel
						if (abs(mCrushAnimation) % 2 == 0 || !CGRID(x, y))
							DrawJewel(x, y);
					} else {
						if (CGRID(x, y))
							//Draw current crushing mAni1
							DrawCrush(x, y, mCrushAnimation);
						else
							//Draw jewel normally
							DrawJewel(x, y);
					}
				} else if (mState & STATE_FALLING) {
					if (CGRID(x, y)) {
						//Draw jewel partway down
						DrawJewel(x, y, mFallAmount);
					} else {
						//Draw jewel normally
						DrawJewel(x, y);
					}
				} else {
					//Draw jewel normally
					DrawJewel(x, y);
				}
			}
		}
	}

	//Draw crushbar
	gg->DrawImage(IMAGE_MODERN, 0, (VBOARD_HEIGHT * JEWEL_SIZE) - (mCrushbars * JEWEL_SIZE), Rect(190, 0, BOARD_WIDTH * JEWEL_SIZE, VBOARD_HEIGHT * JEWEL_SIZE));
}

void Board::DrawMovingPiece()
{
	//Draw the moving block only if in playing mState
	if (mState == STATE_PLAYING) {
		int x = mMBPos.mX * JEWEL_SIZE;
		int y = (mMBPos.mY * HJEWEL_SIZE) - HJEWEL_SIZE;
		DrawMovingJewel(mCurrentpiece.jewel[0], x, y);
		DrawMovingJewel(mCurrentpiece.jewel[1], x, y - JEWEL_SIZE);
		DrawMovingJewel(mCurrentpiece.jewel[2], x, y - (JEWEL_SIZE * 2));

		//Draw rectange around moving jewels
		gg->SetColor(Color::White);
		gg->DrawRect(x - 2, y - 76 - 2, 41, JEWEL_SIZE * 3 + 3);
		gg->DrawRect(x - 3, y - 76 - 3, 42 + 1, JEWEL_SIZE * 3 + 5);
	}
}

void Board::DrawNextPiece()
{
	//Draw the next piece
	gg->SetColor(Color::Black);
	gg->FillRect(bps.npx - mX,  0 + bps.npy - mY, JEWEL_SIZE, JEWEL_SIZE * 3);
	gg->DrawImage(IMAGE_MODERN, bps.npx - mX,  0 + bps.npy - mY, GetJewelSprite(mNextpiece.jewel[2], ANI2));
	gg->DrawImage(IMAGE_MODERN, bps.npx - mX, 38 + bps.npy - mY, GetJewelSprite(mNextpiece.jewel[1], ANI2));
	gg->DrawImage(IMAGE_MODERN, bps.npx - mX, 76 + bps.npy - mY, GetJewelSprite(mNextpiece.jewel[0], ANI2));
}

void Board::DrawScores()
{

}

void Board::DrawReadyText()
{
	//Draw the board backdrop
	gg->DrawImage(IMAGE_MODERN, 0, 0, Rect(418, 0, BOARD_WIDTH * JEWEL_SIZE, VBOARD_HEIGHT * JEWEL_SIZE));

	//gg->SetColorizeImages(true);
	//gg->SetColor(Color(0, 0, 255)));

	//Draw ready image at calculated pos
	float x = (BOARD_WIDTH   * 38 / 2) - (IMAGE_READY->mWidth / 2);
	float y = (VBOARD_HEIGHT * 38 / 3) + mAni1;
	gg->DrawImageF(IMAGE_READY, x, y);
}

void Board::DrawGameOverText()
{
	gg->SetColorizeImages(true);

	//Draw ready image at calculated pos
	float x = (BOARD_WIDTH   * 38 / 2) - (IMAGE_READY->mWidth / 2);
	float y = (VBOARD_HEIGHT * 38 / 3) + mAni1;

	if (mDetectedLost) {
		gg->SetColor(Color(255, 0, 0));
		gg->DrawImageF(IMAGE_LOSE, x, y);
	} else {
		gg->SetColor(Color(0, 255, 0));
		gg->DrawImageF(IMAGE_WIN, x, y);
	}

	gg->SetColorizeImages(false);
}

void Board::DrawJewel(int x, int y)
{
	gg->DrawImage(IMAGE_MODERN, x * JEWEL_SIZE, (12 - y) * JEWEL_SIZE, GetJewelSprite(JGRID(x, y), ANI2));
}

void Board::DrawJewel(int x, int y, int fall)
{
	gg->DrawImage(IMAGE_MODERN, x * JEWEL_SIZE, (12 - y) * JEWEL_SIZE + fall, GetJewelSprite(JGRID(x, y), 0));
}

void Board::DrawMovingJewel(JEWEL j, int x, int y)
{
	gg->DrawImage(IMAGE_MODERN, x, y, GetJewelSprite(j, ANI2));
}

void Board::DrawCrush(int x, int y, float crush)
{
	gg->DrawImageF(IMAGE_MODERN, x * JEWEL_SIZE, (12 - y) * JEWEL_SIZE, Rect(crush * JEWEL_SIZE, 7 * JEWEL_SIZE, JEWEL_SIZE, JEWEL_SIZE));
}

//GREYSCALE CODE
//MemoryImage *jImg = new MemoryImage((SexyAppBase*)mGame->mApp);
//jImg->Create(JEWEL_SIZE, JEWEL_SIZE);
//
////Paste image to memory image
//jImg->NormalBlt(IMAGE_MODERN, 0, 0, GetJewelSprite(JGRID(x, y), ANI2), Color::White);
//
////Convert to greyscale
//ulong *hsl = new ulong(1);
//ulong *buffer = jImg->GetBits();
//for (int i = 0; i < JEWEL_SIZE * JEWEL_SIZE; i++) {
//	mGame->mApp->RGBToHSL(&buffer[i], hsl, 4);
//	*(((uchar*)(hsl)) + 0) = 0;
//	*(((uchar*)(hsl)) + 1) = 0;
//	mGame->mApp->HSLToRGB(hsl, &buffer[i], 4);
//
//}
//jImg->BitsChanged();
//
//gg->DrawImage(jImg, x * JEWEL_SIZE, (12 - y) * JEWEL_SIZE);
//GREYSCALE CODE