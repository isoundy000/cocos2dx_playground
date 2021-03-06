#pragma once

#include <functional>
#include <vector>

#include "2d/CCNode.h"

#include "cpg_GridIndexConverter.h"

NS_CC_BEGIN
	class Label;
	class Sprite;
	class Action;
NS_CC_END

namespace step_clickclick
{
	namespace game
	{
		class BlockView;

		class Stage;

		struct StageViewConfig
		{
			bool bShowPivot = false;
			bool bShowBackgroundGuide = false;
		};

		class StageView : public cocos2d::Node
		{
		public:
			using OnBlockCallback = std::function<void( int )>;

		private:
			StageView( const int width, const int height );

		public:
			static StageView* create( const int width, const int height, const OnBlockCallback& on_block_callback, const StageViewConfig config = StageViewConfig() );

			bool init( const StageViewConfig config, const OnBlockCallback& on_block_callback );

			void Setup( const Stage& stage_data );
			void UpdateBlock( const int linear_index, const int last_life, const int current_life );

		private:
			const int mStageWidth;
			const int mStageHeight;
			const cpg::GridIndexConverter mGridIndexConverter;
			std::vector<BlockView*> mBlockViews;
		};
	}
}
