#pragma once

#include "cocos2d.h"

namespace research
{
	namespace input
	{
		class GamePadTestScene : public cocos2d::Scene
		{
		public:
			GamePadTestScene();

			static const char* getTitle() { return "Input : GamePad Test"; }
			static cocos2d::Scene* create();

			bool init() override;
			void onEnter() override;
			void onExit() override;

		private:
			void updateKeyCodeView( int keycode );
			void clearKeyCodeView();

			void updateForExit( float dt );
			void onKeyPressed( cocos2d::EventKeyboard::KeyCode keycode, cocos2d::Event* /*event*/ );

		private:
			int mPressedKeyCount;
			cocos2d::EventListenerKeyboard* mKeyboardListener;
			cocos2d::EventListenerController* mControllerListener;
		};
	}
}