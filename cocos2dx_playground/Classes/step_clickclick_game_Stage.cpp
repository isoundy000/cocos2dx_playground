#include "step_clickclick_game_Stage.h"

#include <functional>
#include <new>
#include <numeric>
#include <random>

#include "2d/CCLabel.h"
#include "2d/CCLayer.h"
#include "2d/CCSprite.h"
#include "base/ccMacros.h"
#include "ui/UIButton.h"

USING_NS_CC;

namespace step_clickclick
{
	namespace game
	{
		void CheckOddNumber( const int number )
		{
			assert( 1 == ( number & 1 ) );
		}
		void CheckSize( const int pivot, const int number )
		{
			assert( pivot >= number );
		}
		int GetRandomInt( int min, int max )
		{
			static std::random_device rd;
			static std::mt19937 randomEngine( rd() );
			static std::uniform_int_distribution<> dist( min, max );
			return dist( randomEngine );
		}


		Stage::Pannel::Pannel( const int index, const int count, cocos2d::Node* const pannel_node, cocos2d::Label* const label_node ) :
			mIndex( index )
			, mPannelType( Stage::ePannelType::Different )
			, mActive( false )
			, mCount( count )
			, mPannelNode( pannel_node )
			, mLabelNode( label_node )
		{}

		void Stage::Pannel::Init( const int count )
		{
			mActive = true;
			mCount = count;
			mLabelNode->setString( std::to_string( mCount ) );
		}
		void Stage::Pannel::SetVisible( const bool visible )
		{
			mPannelNode->setVisible( visible );
			mLabelNode->setVisible( visible );
		}

		void Stage::Pannel::DecreaseAction()
		{
			mCount = std::max( 0, mCount - 1 );
			Action();
		}
		void Stage::Pannel::IncreaseAction()
		{
			mCount = std::min( 100, mCount + 1 );
			Action();
		}
		void Stage::Pannel::DieAction()
		{
			mCount = 0;
			Action();
		}
		void Stage::Pannel::Action()
		{
			mLabelNode->setString( std::to_string( mCount ) );

			if( 0 == mCount )
			{
				mActive = false;
				mPannelNode->setVisible( false );
			}
		}


		Stage::Stage() :
			mStageWidth( 7 )
			, mStageHeight( 7 )
			, mCenterX( mStageWidth / 2 )
			, mCenterY( mStageWidth / 2 )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, Pannels()
		{
			//
			// Must odd number
			//
			CheckOddNumber( mStageWidth );
			CheckOddNumber( mStageHeight );
		}

		Stage* Stage::create()
		{
			auto ret = new ( std::nothrow ) Stage();
			if( !ret || !ret->init() )
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

		bool Stage::init()
		{
			if( !Node::init() )
			{
				return false;
			}

			const Size tile_size( 32.f, 32.f );
			const Size margin_size( 2.f, 2.f );
			const Size stage_size(
				( mStageWidth * tile_size.width ) + ( ( mStageWidth - 1 ) * margin_size.width )
				,( mStageHeight * tile_size.height ) + ( ( mStageHeight - 1 ) * margin_size.height )
			);
			const Vec2 pivot_position( stage_size.width * -0.5f, stage_size.height * -0.5f );

			setContentSize( stage_size );

			// Pivot
			{
				auto pivot = Sprite::createWithSpriteFrameName( "helper_pivot.png" );
				pivot->setScale( 2.f );
				addChild( pivot, std::numeric_limits<int>::max() );
			}

			// Background Guide
			{
				auto pivot = LayerColor::create( Color4B( 0u, 0u, 0u, 100u ), getContentSize().width, getContentSize().height );
				pivot->setPosition( pivot_position 
);
				addChild( pivot, std::numeric_limits<int>::min() );
			}

			// Buttons
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					const int linear_index = mGridIndexConverter.To_Linear( tx, ty );

					// button
					auto button = ui::Button::create( "guide_01_1.png", "guide_01_2.png", "guide_01_4.png", ui::Widget::TextureResType::PLIST );
					button->setTag( linear_index );
					button->setScale9Enabled( true );
					button->setContentSize( tile_size );
					button->setPosition(
						pivot_position
						+ Vec2( tile_size.width * 0.5f, tile_size.height * 0.5f )
						+ Vec2( tx * ( tile_size.width + margin_size.width ), ty * ( tile_size.height + margin_size.height ) )
					);
					button->addTouchEventListener( CC_CALLBACK_2( Stage::onPannel, this ) );
					addChild( button );

					// label
					auto label = Label::createWithTTF( "0", "fonts/arial.ttf", 9, Size::ZERO, TextHAlignment::LEFT );
					label->setColor( Color3B::RED );
					label->setAnchorPoint( Vec2( 0.5f, 0.5f ) );
					label->setPosition(
						button->getPosition()
						//+ Vec2( button->getContentSize().width * 0.5f, button->getContentSize().height * 0.5f )
					);
					addChild( label, 1 );

					Pannels.emplace_back(
						linear_index
						, 0
						, button
						, label
					);
				}
			}

			return true;
		}

		void Stage::Setup( const int width, const int height )
		{
			CheckOddNumber( width );
			CheckOddNumber( height );
			CheckSize( mStageWidth, width );
			CheckSize( mStageHeight, height );

			for( auto& p : Pannels )
			{
				p.SetVisible( false );
			}

			const int current_pivot_x = mCenterX - ( width / 2 );
			const int current_pivot_y = mCenterY - ( height / 2 );
			for( int ty = current_pivot_y; ty < current_pivot_y + height; ++ty )
			{
				for( int tx = current_pivot_x; tx < current_pivot_x + width; ++tx )
				{
					const int linear_index = mGridIndexConverter.To_Linear( tx, ty );

					Pannels[linear_index].Init( GetRandomInt( 3, 9 ) );
					Pannels[linear_index].SetVisible( true );
				}
			}
		}

		void Stage::onPannel( Ref* sender, ui::Widget::TouchEventType touch_event_type )
		{
			if( ui::Widget::TouchEventType::BEGAN != touch_event_type )
			{
				return;
			}

			auto button_node = static_cast<Node*>( sender );

			if( ePannelType::Single == Pannels[button_node->getTag()].GetType() )
			{
				Pannels[button_node->getTag()].DecreaseAction();
			}
			else if( ePannelType::Together == Pannels[button_node->getTag()].GetType() )
			{
				const int pivot_count = Pannels[button_node->getTag()].GetCount();
				const auto point_index = mGridIndexConverter.To_Point( button_node->getTag() );

				const int current_pivot_x = point_index.x - 1;
				const int current_pivot_y = point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || mStageWidth <= tx
							|| 0 > ty || mStageHeight <= ty )
						{
							continue;
						}

						const int linear_index = mGridIndexConverter.To_Linear( tx, ty );
						if( !Pannels[linear_index].IsActive() )
						{
							continue;
						}

						if( pivot_count != Pannels[linear_index].GetCount() )
						{
							Pannels[linear_index].IncreaseAction();
						}
						else
						{
							Pannels[linear_index].DecreaseAction();
						}
					}
				}
			}
			else if( ePannelType::Different == Pannels[button_node->getTag()].GetType() )
			{
				const int pivot_count = Pannels[button_node->getTag()].GetCount();
				const auto point_index = mGridIndexConverter.To_Point( button_node->getTag() );

				const int current_pivot_x = point_index.x - 1;
				const int current_pivot_y = point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || mStageWidth <= tx
							|| 0 > ty || mStageHeight <= ty )
						{
							continue;
						}

						const int linear_index = mGridIndexConverter.To_Linear( tx, ty );
						if( !Pannels[linear_index].IsActive() )
						{
							continue;
						}

						if( linear_index != button_node->getTag() && pivot_count == Pannels[linear_index].GetCount() )
						{
							Pannels[linear_index].IncreaseAction();
						}
						else
						{
							Pannels[linear_index].DieAction();
						}
					}
				}
			}
		}
	} // namespace game
} // namespace step_clickclick
