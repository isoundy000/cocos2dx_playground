#include "CPG_Input_KeyCodeCollector.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		KeyCodeCollector::KeyCodeCollector() : container()
		{}

		void KeyCodeCollector::onKeyPressed( EventKeyboard::KeyCode keycode )
		{
			container[static_cast<std::size_t>( keycode )] = true;
		}

		void KeyCodeCollector::onKeyReleased( EventKeyboard::KeyCode keycode )
		{
			container[static_cast<std::size_t>( keycode )] = false;
		}
	}
}