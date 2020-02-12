#include "Step01_Game_Terrain_Constant.h"

#include <array>

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			const std::array<TileData, static_cast<std::size_t>( eTileType::SIZE )> TileTable = { {
				{ eTileType::damage, true, false, "step01_game_tile_damage.png", "Damage Tile" }
				,{ eTileType::road, true, false, "step01_game_tile_road.png", "Road Tile" }
				,{ eTileType::entrance, true, true, "step01_game_tile_entrance.png", "Entrance" }
				,{ eTileType::exit, true, true, "step01_game_tile_exit.png", "Exit" }
				,{ eTileType::magic_circle_on, true, true, "step01_game_tile_magic_circle_01.png", "Switch" }
				,{ eTileType::magic_circle_off, false, true, "step01_game_tile_magic_circle_02.png", "" }
			} };

			const TileData& TileType2TileData( const eTileType tile_type )
			{
				if( eTileType::FIRST > tile_type || eTileType::SIZE <= tile_type )
				{
					static const TileData dummy{ eTileType::damage, false, false, "step01_game_tile_damage.png", "W T F" };
					return dummy;
				}

				return TileTable[static_cast<std::size_t>( tile_type )];
			}
			bool TileType2UniqueFlag( const eTileType tile_type )
			{
				if( eTileType::FIRST > tile_type || eTileType::SIZE <= tile_type )
				{
					return false;
				}

				return TileTable[static_cast<std::size_t>( tile_type )].bUnique;
			}
		}
	}
}