#pragma once

#include <functional>

#include "ui/UIWidget.h"

#include "Step01_Game_Terrain_Viewer.h"

namespace step01
{
	namespace tool
	{
		class TerrainViewer : public step01::game::terrain::Viewer
		{
		public:
			using TileSelectCallback = std::function<void( cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType touch_event_type )>;

		private:
			explicit TerrainViewer( const int width, const int height, const TileSelectCallback& tile_select_callback );

		public:
			static TerrainViewer* create( const int width, const int height, const TileSelectCallback& tile_select_callback );

		private:
			Node* MakeTile( const step01::game::terrain::TileData& tile_data, const int grid_x, const int grid_y ) override;

			const TileSelectCallback mTileSelectCallback;
		};
	}
}
