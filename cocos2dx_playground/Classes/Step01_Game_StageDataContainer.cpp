#include "Step01_Game_StageDataContainer.h"

#include "cocos/platform/CCFileUtils.h"
#include "json/document.h"

namespace step_pathfinder
{
	namespace game
	{
		StageDataContainer::StageDataContainer() : mContainer() {}

		bool StageDataContainer::load()
		{
			const std::string regionStr( std::move( cocos2d::FileUtils::getInstance()->getStringFromFile( "datas/step01/game.json" ) ) );
			rapidjson::Document doc;
			doc.Parse<0>( regionStr.c_str() );

			if( doc.HasParseError() )
			{
				CCLOG( "json parse error" );
				return false;
			}

			if( doc.IsNull() )
			{
				CCLOG( "json is empty" );
				return false;
			}

			if( !doc.IsArray() )
			{
				CCLOG( "invalid data struct" );
				return false;
			}

			mContainer.reserve( doc.Size() );
			for( rapidjson::SizeType cur = 0u, end = doc.Size(); cur < end; ++cur )
			{
				mContainer.emplace_back( doc[cur].GetString() );
			}

			return true;
		}

		const std::string& StageDataContainer::get( const std::size_t  stage_index ) const
		{
			if( mContainer.size() <= stage_index )
			{
				static const std::string dummy;
				return dummy;
			}

			return mContainer[stage_index];
		}
	} // namespace game
}
