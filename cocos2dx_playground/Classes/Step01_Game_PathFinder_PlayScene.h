#pragma once

#include "cocos2d.h"
#include "Step01_Game_StageDataContainer.h"
#include "Step01_Game_TerrainData.h"

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			class Viewer;
		}

		namespace pathfinder
		{
			class PlayScene : public cocos2d::Scene
			{
			private:
				PlayScene();

			public:
				static const char* getTitle() { return "Game : Path Finder - Play Scene"; }
				static cocos2d::Scene* create();

				bool init() override;
				void onEnter() override;
				void update( float dt ) override;
				void onExit() override;

			private:
				bool loadStage( std::size_t stage_index );
				bool goNextStage();
				void updateTerrainViewer();

				void MovePlayer( const int move_x, const int move_y );

				void updateForExit( float dt );
				void startExitProcess( float wait_time );

				void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

			private:
				cocos2d::EventListenerKeyboard* mKeyboardListener;

				StageDataContainer mStageDataContainer;
				TerrainData mTerrainData;
				terrain::Viewer* mTerrainViewer;
				StageDataContainer::Container::size_type mCurrentStageIndex;
				TerrainPoint mPlayerPoint;
				bool mbPlayerLive;
				float mElapsedTime;
			};
		} // namespace pathfinder
	}
}
