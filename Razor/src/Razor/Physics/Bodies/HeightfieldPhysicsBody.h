#pragma once

#include "Razor/Physics/PhysicsBody.h"

namespace Razor
{

	class HeightfieldPhysicsBody : public PhysicsBody
	{
	public:
		HeightfieldPhysicsBody(
			Node* node,
			char* heightmap, 
			glm::vec2 height_stick_length,
			float scale, 
			float min_height,
			float max_height,
			int up_axis,
			PHY_ScalarType height_data_type,
			bool flip_quad_edges
		);

		void init() override;

	private:
		Node* node;
		char* heightmap;
		glm::vec2 height_stick_length;
		float scale;
		float min_height;
		float max_height;
		int up_axis;
		PHY_ScalarType height_data_type;
		bool flip_quad_edges;
	};

}
