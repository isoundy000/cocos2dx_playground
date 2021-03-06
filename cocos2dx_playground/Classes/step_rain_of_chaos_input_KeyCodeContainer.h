#pragma once

#include <bitset>

#include "cocos/base/CCEventKeyboard.h"

namespace step_rain_of_chaos
{
	namespace input
	{
		const std::size_t KeyCodeContainerFirst = static_cast<std::size_t>( cocos2d::EventKeyboard::KeyCode::KEY_NONE );
		const std::size_t KeyCodeContainerSize = static_cast<std::size_t>( static_cast<int>( cocos2d::EventKeyboard::KeyCode::KEY_PLAY ) + 1 );
		using KeyCodeContainerT = std::bitset<KeyCodeContainerSize>;
	}
}