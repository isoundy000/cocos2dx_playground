#pragma once

#include <functional>
#include <vector>

#include "ui/UIWidget.h"

namespace cpgui
{
	class ScrollViewGenerator
	{
	private:
		ScrollViewGenerator() = delete;

	public:
		struct Config
		{
			const std::size_t FontSize = 8u;
			const std::size_t ItemVisibleCount = 20u;
			const cocos2d::Size ItemSize = cocos2d::Size( 180.f, 20.f );
			const cocos2d::ui::Margin ItemMargin = cocos2d::ui::Margin( 1.f, 1.f, 1.f, 1.f );
		};

		struct Item
		{
			Item( int tag, const std::string& name ) :
				Tag( tag )
				, Name( name )
			{}

			int Tag = -1;
			std::string Name;
		};
		using ItemContainerT = std::vector<Item>;

		static cocos2d::Node* Create(
			const Config& config
			, const char* title_string
			, const ItemContainerT& item_container
			, const cocos2d::ui::Widget::ccWidgetTouchCallback& item_select_callback
		);
	};
}
