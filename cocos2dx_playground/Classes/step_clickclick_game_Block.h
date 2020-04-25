#pragma once

#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		class Block
		{
		public:
			Block( const int index, const int life );

			void Reset( eBlockType type, const int life );

			void DecreaseAction();
			void IncreaseAction();
			void DieAction();

			int GetIndex() const { return mIndex; }
			eBlockType GetType() const { return mBlockType; }
			bool IsActive() const { return mActive; }
			int GetLife() const { return mLife; }

		private:
			int mIndex;
			eBlockType mBlockType;
			bool mActive;
			int mLife;
		};
	}
}
