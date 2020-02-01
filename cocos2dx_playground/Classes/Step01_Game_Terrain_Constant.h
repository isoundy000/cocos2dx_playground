#pragma once

#include <vector>

namespace step01
{
	namespace game
	{
		namespace terrain
		{
			enum class eTileType
			{
				damage,
				road,

				SIZE,
				FIRST = damage,
			};
		} // namespace pathfinder
	}
}
