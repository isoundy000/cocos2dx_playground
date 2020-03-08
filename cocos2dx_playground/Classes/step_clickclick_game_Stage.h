#pragma once

#include <vector>

#include "2d/CCNode.h"
#include "ui/UIWidget.h"

#include "cpg_GridIndexConverter.h"
#include "step_clickclick_game_Constant.h"

namespace step_clickclick
{
	namespace game
	{
		class Stage : public cocos2d::Node
		{
		public:
			class Pannel
			{
			public:
				Pannel( const int index, const int life );

				void Init( ePannelType type, const int life );
				void DecreaseAction();
				void IncreaseAction();
				void DieAction();

				int GetIndex() const { return mIndex; }
				ePannelType GetType() const { return mPannelType; }
				bool IsActive() const { return mActive; }
				int GetLife() const { return mLife; }

			private:
				int mIndex;
				ePannelType mPannelType;
				bool mActive;
				int mLife;
			};

		private:
			Stage( const int width, const int height );

		public:
			static Stage* create( const int width, const int height );

			bool init() override;

			void Setup( const int width, const int height );

			int GetWidth() const { return mStageWidth; }
			int GetHeight() const { return mStageHeight; }
			const std::vector<Pannel>& GetPannelDatas() const { return Pannels; }
			const Pannel& GetPannelData( const int linear_index ) const;
			void IncreasePannelLife( const int linear_index );
			void DecreasePannelLife( const int linear_index );
			void DiePannelLife( const int linear_index );

		private:
			const int mStageWidth;
			const int mStageHeight;
			const int mCenterX;
			const int mCenterY;
			const cpg::GridIndexConverter mGridIndexConverter;
			std::vector<Pannel> Pannels;
		};
	}
}
