#include "step_mole_game_StageView.h"

#include <functional>
#include <new>
#include <numeric>

#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "2d/CCSpriteFrameCache.h"
#include "ui/UIButton.h"

#include "step_clickclick_game_Stage.h"

USING_NS_CC;

namespace step_mole
{
	namespace game
	{
		StageView::StageView( const StageConfig stage_config ) :
			mStageConfig( stage_config )
		{}

		StageView* StageView::create( const StageConfig stage_config, const StageViewConfig config )
		{
			auto ret = new ( std::nothrow ) StageView( stage_config );
			if( !ret || !ret->init( config ) )
			{
				delete ret;
				ret = nullptr;
				return nullptr;
			}
			else
			{
				ret->autorelease();
			}

			return ret;
		}

		bool StageView::init( const StageViewConfig config )
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size StageSize(
				mStageConfig.BlockSize.width * mStageConfig.BlockCount_Horizontal
				, mStageConfig.BlockSize.width * mStageConfig.BlockCount_Vercital
			);
			const Size StageMargin( 4.f, 4.f );
			const Size TotalSize(
				StageMargin
				+ StageSize
				+ StageMargin
			);

			setContentSize( TotalSize );

			// Pivot
			if( config.bShowPivot )
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 4.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			//
			// Background
			//
			if( config.bShowBackgroundGuide )
			{
				auto background_guide = LayerColor::create( Color4B( 255, 0, 255, 150 ), getContentSize().width, getContentSize().height );
				addChild( background_guide, -1 );
			}

			auto content_root_node = Node::create();
			content_root_node->setPosition( StageMargin.width, StageMargin.height );
			addChild( content_root_node );

			//
			// Terrain View
			//
			{
				auto tile_sprite_frame_0 = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_tile_0.png" );
				auto tile_sprite_frame_1 = SpriteFrameCache::getInstance()->getSpriteFrameByName( "step_mole_tile_1.png" );
				CCASSERT( tile_sprite_frame_0, "Sprite Frame Not Found" );

				const auto block_scale = mStageConfig.BlockSize.height / tile_sprite_frame_0->getRect().size.height;

				bool first_tile_indicator = true;
				bool current_tile_indicator = true;
				for( int by = 0; mStageConfig.BlockCount_Vercital > by; ++by )
				{
					current_tile_indicator = first_tile_indicator;

					for( int bx = 0; mStageConfig.BlockCount_Horizontal > bx; ++bx )
					{
						auto block_sprite = Sprite::createWithSpriteFrame( current_tile_indicator ? tile_sprite_frame_0 : tile_sprite_frame_1 );
						block_sprite->setAnchorPoint( Vec2::ZERO );
						block_sprite->setScale( block_scale );
						block_sprite->setPosition(
							bx * mStageConfig.BlockSize.width
							, by * mStageConfig.BlockSize.height
						);
						content_root_node->addChild( block_sprite );

						current_tile_indicator = !current_tile_indicator;
					}

					first_tile_indicator = !first_tile_indicator;
				}
			}

			//
			// Objects
			//
			{
				const Vec2 offset( mStageConfig.BlockSize.width * 0.5f, mStageConfig.BlockSize.height * 0.5f );

				for( int by = 0; mStageConfig.BlockCount_Vercital > by; ++by )
				{
					for( int bx = 0; mStageConfig.BlockCount_Horizontal > bx; ++bx )
					{
						auto block_sprite = Sprite::createWithSpriteFrameName( "actor001_idle_01.png" );
						block_sprite->setPosition(
							offset
							+ Vec2( bx * mStageConfig.BlockSize.width, by * mStageConfig.BlockSize.height )
						);
						content_root_node->addChild( block_sprite, 1 );
					}
				}
			}

			//
			// Click Area
			//
			{
				auto click_area = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
				click_area->setScale9Enabled( true );
				click_area->setContentSize( StageSize );
				click_area->addTouchEventListener( CC_CALLBACK_2( StageView::onStageClick, this ) );
				click_area->setPosition( Vec2(
					TotalSize.width * 0.5f
					, TotalSize.height * 0.5f
				) );
				addChild( click_area );
			}

			return true;
		}

		void StageView::onStageClick( Ref* /*sender*/, ui::Widget::TouchEventType /*touch_event_type*/ )
		{
			CCLOG( "On Stage Click" );
		}
	} // namespace game
} // namespace step_mole
