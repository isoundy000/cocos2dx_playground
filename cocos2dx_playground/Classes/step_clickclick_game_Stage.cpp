#include "step_clickclick_game_Stage.h"

#include <cassert>
#include <chrono>
#include <new>
#include <random>

namespace step_clickclick
{
	namespace game
	{
		namespace
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
		}


		Stage::Block::Block( const int index, const int life ) :
			mIndex( index )
			, mBlockType( eBlockType::Different )
			, mActive( false )
			, mLife( life )
		{}

		void Stage::Block::Init( eBlockType type, const int life )
		{
			mBlockType = type;
			mActive = true;
			mLife = life;
		}
		void Stage::Block::DecreaseAction()
		{
			mLife = std::max( 0, mLife - 1 );
			mActive = ( 0 < mLife );
		}
		void Stage::Block::IncreaseAction()
		{
			mLife = std::min( 100, mLife + 1 );
			mActive = ( 0 < mLife );
		}
		void Stage::Block::DieAction()
		{
			mLife = 0;
			mActive = false;
		}



		Stage::Stage( const int width, const int height ) :
			mStageWidth( width )
			, mStageHeight( height )
			, mCenterX( mStageWidth / 2 )
			, mCenterY( mStageWidth / 2 )
			, mGridIndexConverter( mStageWidth, mStageHeight )
			, mBlocks()
		{
			//
			// Must odd number
			//
			CheckOddNumber( mStageWidth );
			CheckOddNumber( mStageHeight );
		}

		StageUp Stage::create( const int width, const int height )
		{
			StageUp ret( new ( std::nothrow ) Stage( width, height ) );
			if( !ret || !ret->init() )
			{
				ret.reset();
				return nullptr;
			}

			return ret;
		}

		bool Stage::init()
		{
			for( int ty = 0; ty < mStageHeight; ++ty )
			{
				for( int tx = 0; tx < mStageWidth; ++tx )
				{
					const int linear_index = mGridIndexConverter.To_Linear( tx, ty );

					mBlocks.emplace_back(
						linear_index
						, 0
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

			//
			// Clear
			//
			for( auto& p : mBlocks )
			{
				p.DieAction();
			}

			std::vector<eBlockType> block_type_list;
			{
				const int block_count = width * height;
				block_type_list.resize( block_count, eBlockType::Single );

				// init
				const int together_count = block_count * 0.3f;
				const int different_count = block_count * 0.2f;
				auto cur = block_type_list.begin();
				for( int i = 0; i < together_count; ++i )
				{
					*cur = eBlockType::Same;
					++cur;
				}
				for( int i = 0; i < different_count; ++i )
				{
					*cur = eBlockType::Different;
					++cur;
				}

				// shuffle
				const unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
				std::default_random_engine random_engine( seed );
				shuffle( block_type_list.begin(), block_type_list.end(), random_engine );
				shuffle( block_type_list.begin(), block_type_list.end(), random_engine );
			}

			const int current_pivot_x = mCenterX - ( width / 2 );
			const int current_pivot_y = mCenterY - ( height / 2 );
			auto t_type = block_type_list.cbegin();
			int linear_index = 0;
			for( int ty = current_pivot_y; ty < current_pivot_y + height; ++ty )
			{
				for( int tx = current_pivot_x; tx < current_pivot_x + width; ++tx )
				{
					linear_index = mGridIndexConverter.To_Linear( tx, ty );

					mBlocks[linear_index].Init( *t_type, GetRandomInt( 3, 9 ) );
					++t_type;
				}
			}
		}

		const Stage::Block& Stage::GetBlockData( const int linear_index ) const
		{
			if( 0 > linear_index || static_cast<int>( mBlocks.size() ) <= linear_index )
			{
				static const Block dummy( -1, 0 );
				return dummy;
			}

			return mBlocks[linear_index];
		}
		void Stage::IncreaseBlockLife( const int linear_index )
		{
			if( 0 > linear_index || static_cast<int>( mBlocks.size() ) <= linear_index )
			{
				return;
			}

			mBlocks[linear_index].IncreaseAction();
		}
		void Stage::DecreaseBlockLife( const int linear_index )
		{
			if( 0 > linear_index || static_cast<int>( mBlocks.size() ) <= linear_index )
			{
				return;
			}

			mBlocks[linear_index].DecreaseAction();
		}
		void Stage::DieBlock( const int linear_index )
		{
			if( 0 > linear_index || static_cast<int>( mBlocks.size() ) <= linear_index )
			{
				return;
			}

			mBlocks[linear_index].DieAction();
		}

		bool Stage::HasActiveBlock() const
		{
			for( const auto p : mBlocks )
			{
				if( !p.IsActive() )
				{
					continue;
				}

				return true;
			}

			return false;
		}
	} // namespace game
} // namespace step_clickclick
