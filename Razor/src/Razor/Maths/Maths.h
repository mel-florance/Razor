#pragma once

namespace Razor
{
	struct BoundingBox
	{
		float min_x = 0.0f;
		float max_x = 0.0f;
		float min_y = 0.0f;
		float max_y = 0.0f;
		float min_z = 0.0f;
		float max_z = 0.0f;
	};

	class Maths
	{
	public:
		inline static float lerp(float a, float b, float d)
		{
			return a + d * (b - a);
		}
	};

}
