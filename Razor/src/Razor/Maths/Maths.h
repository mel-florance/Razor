#pragma once

namespace Razor
{
	struct AABB
	{
		float min_x = 0.0f;
		float max_x = 0.0f;
		float min_y = 0.0f;
		float max_y = 0.0f;
		float min_z = 0.0f;
		float max_z = 0.0f;

		void set(const glm::vec3& t)
		{
			if (t.x < min_x) min_x = t.x;
			if (t.x > max_x) max_x = t.x;
			if (t.y < min_y) min_y = t.y;
			if (t.y > max_y) max_y = t.y;
			if (t.z < min_z) min_z = t.z;
			if (t.z > max_z) max_z = t.z;
		}
	};

	class Maths
	{
	public:
		inline static float lerp(float a, float b, float t)
		{
			return a + t * (b - a);
		}

		inline static glm::vec3 geodesicToSpherical(const glm::vec2& coords, const glm::vec2& offset, float radius)
		{
			float phi = glm::radians(offset.x - coords.x);
			float theta = glm::radians(coords.y + offset.y);

			return glm::vec3(
				 ((radius) * sinf(phi) * cosf(theta)),
				 ((radius) * cosf(phi)),
				 ((radius) * sinf(phi) * sinf(theta))
			);
		}
	};

}
