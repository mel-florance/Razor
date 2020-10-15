#include "rzpch.h"
#include "HeightfieldPhysicsBody.h"

namespace Razor
{

	HeightfieldPhysicsBody::HeightfieldPhysicsBody(
		Node* node,
		char* heightmap,
		glm::vec2 height_stick_length,
		float scale,
		float min_height,
		float max_height,
		int up_axis,
		PHY_ScalarType height_data_type,
		bool flip_quad_edges
	) : 
		PhysicsBody(node),
		node(node),
		heightmap(heightmap),
		height_stick_length(height_stick_length),
		scale(scale),
		min_height(min_height),
		max_height(max_height),
		up_axis(up_axis),
		height_data_type(height_data_type),
		flip_quad_edges(flip_quad_edges)
	{
	}

	void HeightfieldPhysicsBody::init()
	{

	}

}
