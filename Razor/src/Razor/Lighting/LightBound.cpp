#include "rzpch.h"
#include "LightBound.h"

#include "Razor/Scene/Node.h"
#include "Razor/Rendering/ForwardRenderer.h"
#include "Razor/Materials/Presets/ColorMaterial.h"

#include "Razor/Lighting/Light.h"
#include "Razor/Lighting/Directional.h"
#include "Razor/Lighting/Point.h"
#include "Razor/Lighting/Spot.h"

#include "Razor/Geometry/Geometry.h"
#include "Razor/Application/Application.h"

namespace Razor
{

	LightBound::LightBound(Light* light) :
		node(std::make_shared<Node>()),
		light(light)
	{
	}

	LightBound::~LightBound()
	{
	}

	DirectionalBound::DirectionalBound(Directional* directional) : LightBound(directional)
	{
		glm::vec3 pos = directional->getPosition();
		glm::vec3 dir = directional->getDirection();

		ray = std::make_shared<Ray>(pos, dir, -10.0f, true);
		ray->setMaterial(ForwardRenderer::colorMaterial);
		ray->setLineDashed(true);
		node->meshes.push_back(ray);

		ForwardRenderer::addLineMesh(node, 0);
	}

	DirectionalBound::~DirectionalBound()
	{

	}

	void DirectionalBound::update()
	{
		Directional* directional = static_cast<Directional*>(light);
		glm::vec3 pos = directional->getPosition();
		glm::vec3 dir = directional->getDirection();

		Ray r = Ray(pos, dir, -10.f, true);
		ray->getVbo()->update((unsigned int)r.getVertices().size() * sizeof(float), &r.getVertices()[0]);
	}

	PointBound::PointBound(Point* point, float radius) : 
		LightBound(point),
		radius(radius)
	{
		circle = std::make_shared<Circle>(radius);
		circle->setMaterial(ForwardRenderer::colorMaterial);
		circle->setLineDashed(false);

		circle_node_x = std::make_shared<Node>();
		circle_node_x->transform.setPosition(point->getPosition());
		circle_node_x->meshes.push_back(circle);
		circle_node_x->name = "Circle_X";
		node->nodes.push_back(circle_node_x);

		circle_node_z = std::make_shared<Node>();
		circle_node_z->transform.setRotation(glm::vec3(PI * 0.5f, 0.0f, 0.0f));
		circle_node_z->transform.setPosition(point->getPosition());
		circle_node_z->meshes.push_back(circle);
		circle_node_z->name = "Circle_Z";
		node->nodes.push_back(circle_node_z);

		ForwardRenderer::addLineMesh(node, 0);
	}

	PointBound::~PointBound()
	{

	}

	void PointBound::update()
	{
		Point* point = static_cast<Point*>(light);
		circle_node_x->transform.setPosition(point->getPosition());
		circle_node_z->transform.setPosition(point->getPosition());
	}

	SpotBound::SpotBound(Spot* spot) :
		LightBound(spot)
	{

	}

	SpotBound::~SpotBound()
	{

	}
}
