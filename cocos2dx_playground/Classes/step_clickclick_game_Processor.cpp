#include "step_clickclick_game_Processor.h"

#include <algorithm>
#include <cassert>

#include "audio/include/AudioEngine.h"

#include "step_clickclick_game_Stage.h"
#include "step_clickclick_game_StageView.h"

namespace step_clickclick
{
	namespace game
	{
		void Processor::Do( Stage* stage, StageView* stage_view, const int block_linear_index, int* out_score )
		{
			const auto& block_data = stage->GetBlockData( block_linear_index );
			const auto block_point_index = stage->ConvertLinearIndex2PointIndex( block_data.GetIndex() );
			int last_life = 0;

			if( eBlockType::Single == block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/damaged_001.ogg", false, 0.1f );

				bool has_neighbor = false;
				const int current_pivot_x = block_point_index.x - 1;
				const int current_pivot_y = block_point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || stage->GetWidth() <= tx
							|| 0 > ty || stage->GetHeight() <= ty )
						{
							continue;
						}

						if( tx != block_point_index.x && ty != block_point_index.y )
						{
							continue;
						}

						const auto& target_block_data = stage->GetBlockData( tx, ty );
						if( block_linear_index == target_block_data.GetIndex() )
						{
							continue;
						}

						if( !target_block_data.IsActive() )
						{
							continue;
						}

						has_neighbor = true;
						break;
					}
				}

				last_life = block_data.GetLife();
				if( has_neighbor )
				{
					++*out_score;

					stage->DecreaseBlockLife( block_data.GetIndex() );
					stage_view->UpdateBlock( block_data.GetIndex(), last_life, block_data.GetLife() );
				}
				else
				{
					*out_score = std::max( 0, *out_score - block_data.GetLife() );

					stage->DieBlock( block_data.GetIndex() );
					stage_view->UpdateBlock( block_data.GetIndex(), last_life, block_data.GetLife() );
				}
			}
			else if( eBlockType::Same == block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/jump_001.ogg", false, 0.1f );

				const int pivot_count = block_data.GetLife();

				const int current_pivot_x = block_point_index.x - 1;
				const int current_pivot_y = block_point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || stage->GetWidth() <= tx
							|| 0 > ty || stage->GetHeight() <= ty )
						{
							continue;
						}

						const auto& target_block_data = stage->GetBlockData( tx, ty );
						if( !target_block_data.IsActive() )
						{
							continue;
						}

						if( eBlockType::Same == target_block_data.GetType() && pivot_count != target_block_data.GetLife() )
						{
							continue;
						}

						last_life = target_block_data.GetLife();
						if( pivot_count != target_block_data.GetLife() )
						{
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
						}
						else
						{
							*out_score += 3;
							stage->DecreaseBlockLife( target_block_data.GetIndex() );
						}

						stage_view->UpdateBlock( target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
					}
				}
			}
			else if( eBlockType::Different == block_data.GetType() )
			{
				cocos2d::experimental::AudioEngine::play2d( "sounds/fx/coin_001.ogg", false, 0.2f );

				const int pivot_count = block_data.GetLife();

				const int current_pivot_x = block_point_index.x - 1;
				const int current_pivot_y = block_point_index.y - 1;
				for( int ty = current_pivot_y; ty < current_pivot_y + 3; ++ty )
				{
					for( int tx = current_pivot_x; tx < current_pivot_x + 3; ++tx )
					{
						if( 0 > tx || stage->GetWidth() <= tx
							|| 0 > ty || stage->GetHeight() <= ty )
						{
							continue;
						}

						const auto& target_block_data = stage->GetBlockData( tx, ty );
						if( !target_block_data.IsActive() )
						{
							continue;
						}

						last_life = target_block_data.GetLife();
						if( target_block_data.GetIndex() != block_data.GetIndex() && pivot_count == target_block_data.GetLife() )
						{
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
							stage->IncreaseBlockLife( target_block_data.GetIndex() );
						}
						else
						{
							*out_score += target_block_data.GetLife();
							stage->DieBlock( target_block_data.GetIndex() );
						}

						stage_view->UpdateBlock( target_block_data.GetIndex(), last_life, target_block_data.GetLife() );
					}
				}
			}
		}
	} // namespace game
} // namespace step_clickclick
