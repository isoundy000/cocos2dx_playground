#pragma once

#include "2d/CCScene.h"

namespace step_clickclick
{
	namespace animation
	{
		class PlayNStopScene : public cocos2d::Scene
		{
		private:
			PlayNStopScene();

		public:
			virtual ~PlayNStopScene();

			static const char* getTitle() { return "Animation : Play And Stop"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::Action* mNormalAction;
			cocos2d::Action* mRepeatAction;
		};
	}
}
