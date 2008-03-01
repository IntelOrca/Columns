#include <SexyAppFramework/Graphics.h>
#include <SexyAppFramework/Image.h>
#include "Res.h"
#include "Monster.h"

using namespace Sexy;

Monster::Monster(int id)
{
	mId = id;
	mMonsterImage = NULL;
	mUpdateCnt = 0;
}

Monster::~Monster()
{

}

void Monster::Update()
{
	//Update the counter
	mUpdateCnt++;
}

void Monster::Draw(Graphics *g)
{
	//Draw the frame
	g->DrawImage(IMAGE_OPPONENT_FRAME, 0, 0);

	if (mMonsterImage != NULL && mAnimationState != ANI_STATE_NONE) {
		if (mAnimationState == ANI_STATE_APPEARING) {
			
		} else if (mAnimationState == ANI_STATE_DISAPPEARING) {
			
		} else {
			int row = 0;
			if (mAnimationState == ANI_STATE_WINNING)
				row = 1;
			else if (mAnimationState == ANI_STATE_LOSING)
				row = 2;

			//Update animation
			g->DrawImageCel(mMonsterImage, MONST_X, MONST_Y, mMonsterImage->GetAnimCel(mUpdateCnt), row);
		}
	}
}

void Monster::Appear()
{
	mAnimationState = ANI_STATE_APPEARING;
	mUpdateCnt = 0;
}

void Monster::Disappear()
{
	mAnimationState = ANI_STATE_DISAPPEARING;
	mUpdateCnt = 0;
}

void Monster::Playing()
{
	mAnimationState = ANI_STATE_PLAYING;
	mUpdateCnt = 0;
}

void Monster::Winning()
{
	mAnimationState = ANI_STATE_WINNING;
	mUpdateCnt = 0;
}

void Monster::Losing()
{
	mAnimationState = ANI_STATE_LOSING;
	mUpdateCnt = 0;
}

void Monster::Hide()
{
	mAnimationState = ANI_STATE_NONE;
	mUpdateCnt = 0;
}