#include "CPG_Input_KeyMapConfigHelper.h"

#include <utility>
#include <fstream>

#include "platform\CCFileUtils.h"
#include "json/document.h"
#include "json/stringbuffer.h"
#include "json/writer.h"

USING_NS_CC;

namespace CPG
{
	namespace Input
	{
		namespace
		{
			const char* string_key_code = "key_code";
			const char* string_name = "name";
			const char* string_empty = "o_o";
		}

		KeyMapConfigHelper::KeyMapConfigHelper() : container() {}

		const bool KeyMapConfigHelper::load( const char* _key_map_file_name )
		{
			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( _key_map_file_name );

			if( load_Json( path.c_str() ) )
				return true;

			// load default
			if( load_Json( "datas/keyconfig/keymap_default.json" ) )
			{
				save_Json( path.c_str() );
				return true;
			}

			return false;
		}

		void KeyMapConfigHelper::set( const int _key_index, const cocos2d::EventKeyboard::KeyCode _new_keycode )
		{
			if( 0 > _key_index || _key_index >= container.size() )
				return;

			container[static_cast<std::size_t>( _key_index )].keycode = _new_keycode;
		}

		const bool KeyMapConfigHelper::load_Json( const char* _key_map_path )
		{
			// load json
			const std::string regionStr = cocos2d::FileUtils::getInstance()->getStringFromFile( _key_map_path );
			rapidjson::Document doc;
			doc.Parse<0>( regionStr.c_str() );

			if( doc.HasParseError() )
			{
				cocos2d::log( "json parse error" );
				return false;
			}

			if( doc.IsNull() )
			{
				cocos2d::log( "json is empty" );
				return false;
			}

			if( !doc.IsArray() )
			{
				cocos2d::log( "invalid data struct" );
				return false;
			}

			container.reserve( doc.Size() );

			rapidjson::Value::MemberIterator name_itr;
			int key_idx = 0;
			rapidjson::Value::MemberIterator key_code_itr;
			for( auto cur = doc.Begin(); cur != doc.End(); ++cur )
			{
				name_itr = cur->FindMember( string_name );
				if( name_itr == cur->MemberEnd() )
					continue;

				key_code_itr = cur->FindMember( string_key_code );
				if( key_code_itr == cur->MemberEnd() )
					continue;

				container.emplace_back( name_itr->value.GetString(), key_idx, static_cast<EventKeyboard::KeyCode>( key_code_itr->value.GetInt() ) );

				++key_idx;
			}

			return true;
		}

		const bool KeyMapConfigHelper::save_Json( const char* _key_map_path ) const
		{
			std::string path( std::move( cocos2d::FileUtils::getInstance()->getWritablePath() ) );
			path.append( _key_map_path );

			rapidjson::Document document;
			document.SetArray();

			for( const auto h : container )
			{
				rapidjson::Value val;
				val.SetObject();

				val.AddMember( rapidjson::Value::StringRefType( string_key_code ), static_cast<int>( h.keycode ), document.GetAllocator() );
				val.AddMember( rapidjson::Value::StringRefType( string_name ), h.idx, document.GetAllocator() );

				document.PushBack( val, document.GetAllocator() );
			}

			rapidjson::StringBuffer buffer;
			rapidjson::Writer<rapidjson::StringBuffer> writer( buffer );
			document.Accept( writer );

			std::ofstream fs( path.c_str(), std::ios::out );
			fs << buffer.GetString() << std::endl;
			fs.close();

			return true;
		}
	}
}