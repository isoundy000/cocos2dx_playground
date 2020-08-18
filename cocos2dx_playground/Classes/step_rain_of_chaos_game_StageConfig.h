#pragma once

#include "math/CCGeometry.h"
#include "math/Vec2.h"

namespace step_rain_of_chaos
{
	namespace game
	{
		class StageConfig
		{
		public:
			void Build( const int center_x, const int center_y, const int width )
			{
				mCenter.set( center_x, center_y );

				mStageArea.setRect(
					mCenter.x - ( width * 0.5f ), mCenter.y - ( width * 0.5f )
					, width, width
				);

				float temp_radius = sqrt( ( ( mStageArea.size.width * 0.5f ) * ( mStageArea.size.width * 0.5f ) ) * 2.f ) + 8.f;
				mBulletGenerateAreaMin.setRect(
					mCenter.x - temp_radius, mCenter.y - temp_radius
					, temp_radius * 2.f, temp_radius * 2.f
				);

				temp_radius += 40.f;
				mBulletGenerateAreaMax.setRect(
					mCenter.x - temp_radius, mCenter.y - temp_radius
					, temp_radius * 2.f, temp_radius * 2.f
				);

				temp_radius += 20.f;
				mBulletLifeArea.setRect(
					mCenter.x - temp_radius, mCenter.y - temp_radius
					, temp_radius * 2.f, temp_radius * 2.f
				);
			}

			cocos2d::Vec2 GetCenter() const { return mCenter; }
			cocos2d::Rect GetStageArea() const { return mStageArea; }
			cocos2d::Rect GetBulletLifeArea() const { return  mBulletLifeArea; }
			cocos2d::Rect GetBulletGenerateAreaMin() const { return mBulletGenerateAreaMin; }
			cocos2d::Rect GetBulletGenerateAreaMax() const { return mBulletGenerateAreaMax; }

		private:
			cocos2d::Vec2 mCenter;
			cocos2d::Rect mStageArea;
			cocos2d::Rect mBulletLifeArea;
			cocos2d::Rect mBulletGenerateAreaMin;
			cocos2d::Rect mBulletGenerateAreaMax;
		};
	}
}
