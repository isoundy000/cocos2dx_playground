#pragma once

#include "2d/CCScene.h"

namespace step_mole
{
	namespace game_test
	{
		class ObjectActionScene : public cocos2d::Scene
		{
		private:
			ObjectActionScene();

		public:
			static const char* getTitle() { return "Game Test : Object Action"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateLifeTimeView();
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;

			int mCurrentLifeTime;
		};
	}
}
