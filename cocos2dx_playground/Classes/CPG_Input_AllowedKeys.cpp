#include "CPG_Input_AllowedKeys.h"

#include "CPG_InputKeyMap.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		namespace
		{
			void load_Allowed_keys( AllowedKeys::AllowedInfos& _container )
			{
				const std::initializer_list<bool> temp_container( {
					  false		// KEY_NONE,
					, false		// KEY_PAUSE,
					, false		// KEY_SCROLL_LOCK,
					, false		// KEY_PRINT,
					, false		// KEY_SYSREQ,
					, false		// KEY_BREAK,
					, true		// KEY_ESCAPE,
					, true		// KEY_BACKSPACE,
					, true		// KEY_TAB,
					, true		// KEY_BACK_TAB,
					, true		// KEY_RETURN,
					, true		// KEY_CAPS_LOCK,
					, true		// KEY_SHIFT,
					, true		// KEY_RIGHT_SHIFT,
					, true		// KEY_CTRL,
					, true		// KEY_RIGHT_CTRL,
					, true		// KEY_ALT,
					, true		// KEY_RIGHT_ALT,
					, true		// KEY_MENU,
					, true		// KEY_HYPER,
					, true		// KEY_INSERT,
					, true		// KEY_HOME,
					, true		// KEY_PG_UP,
					, true		// KEY_DELETE,
					, true		// KEY_END,
					, true		// KEY_PG_DOWN,
					, true		// KEY_LEFT_ARROW,
					, true		// KEY_RIGHT_ARROW,
					, true		// KEY_UP_ARROW,
					, true		// KEY_DOWN_ARROW,
					, true		// KEY_NUM_LOCK,
					, true		// KEY_KP_PLUS,
					, true		// KEY_KP_MINUS,
					, true		// KEY_KP_MULTIPLY,
					, true		// KEY_KP_DIVIDE,
					, true		// KEY_KP_ENTER,
					, true		// KEY_KP_HOME,
					, true		// KEY_KP_UP,
					, true		// KEY_KP_PG_UP,
					, true		// KEY_KP_LEFT,
					, true		// KEY_KP_FIVE,
					, true		// KEY_KP_RIGHT,
					, true		// KEY_KP_END,
					, true		// KEY_KP_DOWN,
					, true		// KEY_KP_PG_DOWN,
					, true		// KEY_KP_INSERT,
					, true		// KEY_KP_DELETE,
					, true		// KEY_F1,
					, true		// KEY_F2,
					, true		// KEY_F3,
					, true		// KEY_F4,
					, true		// KEY_F5,
					, true		// KEY_F6,
					, true		// KEY_F7,
					, true		// KEY_F8,
					, true		// KEY_F9,
					, true		// KEY_F10,
					, true		// KEY_F11,
					, true		// KEY_F12,
					, false		// KEY_SPACE,
					, true		// KEY_EXCLAM,
					, true		// KEY_QUOTE,
					, true		// KEY_NUMBER,
					, true		// KEY_DOLLAR,
					, true		// KEY_PERCENT,
					, true		// KEY_CIRCUMFLEX,
					, true		// KEY_AMPERSAND,
					, true		// KEY_APOSTROPHE,
					, true		// KEY_LEFT_PARENTHESIS,
					, true		// KEY_RIGHT_PARENTHESIS,
					, true		// KEY_ASTERISK,
					, true		// KEY_PLUS,
					, true		// KEY_COMMA,
					, true		// KEY_MINUS,
					, true		// KEY_PERIOD,
					, true		// KEY_SLASH,
					, true		// KEY_0,
					, true		// KEY_1,
					, true		// KEY_2,
					, true		// KEY_3,
					, true		// KEY_4,
					, true		// KEY_5,
					, true		// KEY_6,
					, true		// KEY_7,
					, true		// KEY_8,
					, true		// KEY_9,
					, true		// KEY_COLON,
					, true		// KEY_SEMICOLON,
					, true		// KEY_LESS_THAN,
					, true		// KEY_EQUAL,
					, true		// KEY_GREATER_THAN,
					, true		// KEY_QUESTION,
					, true		// KEY_AT,
					, true		// KEY_CAPITAL_A,
					, true		// KEY_CAPITAL_B,
					, true		// KEY_CAPITAL_C,
					, true		// KEY_CAPITAL_D,
					, true		// KEY_CAPITAL_E,
					, true		// KEY_CAPITAL_F,
					, true		// KEY_CAPITAL_G,
					, true		// KEY_CAPITAL_H,
					, true		// KEY_CAPITAL_I,
					, true		// KEY_CAPITAL_J,
					, true		// KEY_CAPITAL_K,
					, true		// KEY_CAPITAL_L,
					, true		// KEY_CAPITAL_M,
					, true		// KEY_CAPITAL_N,
					, true		// KEY_CAPITAL_O,
					, true		// KEY_CAPITAL_P,
					, true		// KEY_CAPITAL_Q,
					, true		// KEY_CAPITAL_R,
					, true		// KEY_CAPITAL_S,
					, true		// KEY_CAPITAL_T,
					, true		// KEY_CAPITAL_U,
					, true		// KEY_CAPITAL_V,
					, true		// KEY_CAPITAL_W,
					, true		// KEY_CAPITAL_X,
					, true		// KEY_CAPITAL_Y,
					, true		// KEY_CAPITAL_Z,
					, true		// KEY_LEFT_BRACKET,
					, true		// KEY_BACK_SLASH,
					, true		// KEY_RIGHT_BRACKET,
					, true		// KEY_UNDERSCORE,
					, true		// KEY_GRAVE,
					, true		// KEY_A,
					, true		// KEY_B,
					, true		// KEY_C,
					, true		// KEY_D,
					, true		// KEY_E,
					, true		// KEY_F,
					, true		// KEY_G,
					, true		// KEY_H,
					, true		// KEY_I,
					, true		// KEY_J,
					, true		// KEY_K,
					, true		// KEY_L,
					, true		// KEY_M,
					, true		// KEY_N,
					, true		// KEY_O,
					, true		// KEY_P,
					, true		// KEY_Q,
					, true		// KEY_R,
					, true		// KEY_S,
					, true		// KEY_T,
					, true		// KEY_U,
					, true		// KEY_V,
					, true		// KEY_W,
					, true		// KEY_X,
					, true		// KEY_Y,
					, true		// KEY_Z,
					, true		// KEY_LEFT_BRACE,
					, true		// KEY_BAR,
					, true		// KEY_RIGHT_BRACE,
					, false		// KEY_TILDE,
					, false		// KEY_EURO,
					, false		// KEY_POUND,
					, false		// KEY_YEN,
					, true		// KEY_MIDDLE_DOT,
					, true		// KEY_SEARCH,
					, true		// KEY_DPAD_LEFT,
					, true		// KEY_DPAD_RIGHT,
					, true		// KEY_DPAD_UP,
					, true		// KEY_DPAD_DOWN,
					, true		// KEY_DPAD_CENTER,
					, true		// KEY_ENTER,
					, false		// KEY_PLAY
				} );

				std::size_t i = 0;
				for( auto a : temp_container )
				{
					_container[i] = a;
					++i;
				}
			}
		}

		AllowedKeys::AllowedKeys() : container()
		{}

		void AllowedKeys::load()
		{
			load_Allowed_keys( container );
		}
	}
}