#ifndef MONSTER_H_
#define MONSTER_H_

#include <SexyAppFramework/Widget.h>

namespace Sexy
{
	class Graphics;
	class Image;

	class Monster : public Widget
	{
	public:
		static const int MONST_X = 8;
		static const int MONST_Y = 8;
		enum AnimationStates
		{
			ANI_STATE_NONE,
			ANI_STATE_APPEARING,
			ANI_STATE_DISAPPEARING,
			ANI_STATE_PLAYING,
			ANI_STATE_WINNING,
			ANI_STATE_LOSING,
		};

		int		mId;
		Image	*mMonsterImage;
		int		mAnimationState;

	public:
		Monster(int id);
		~Monster();

		void Update();
		void Draw(Graphics *g);

		void Appear();
		void Disappear();
		void Playing();
		void Winning();
		void Losing();
		void Hide();
	};
}

#endif