#pragma once

#include <vector>

#include "cocos2d.h"

namespace CPG
{
	namespace Input
	{
		using KeyMapSp = std::shared_ptr<class KeyMap>;
		using AnalyzerSp = std::shared_ptr<class Analyzer>;

		class KeyViewer : public cocos2d::Node
		{
		private:
			KeyViewer();

		public:
			static KeyViewer* create( const KeyMapSp& key_map );

			bool init( const KeyMapSp& key_map );

			void setup( const CPG::Input::AnalyzerSp input_analyzer );

		private:
			struct KeyViewData
			{
				KeyViewData( int _key_index, cocos2d::Sprite* _sprite ) : key_index( _key_index ), sprite( _sprite ) {}

				int key_index;
				cocos2d::Sprite* sprite;
			};
			std::vector<KeyViewData> key_views;
		};
	}
}