#pragma once

namespace Razor
{

	class Node;
	class Ray;
	class Circle;
	class Light;
	class Directional;
	class Point;
	class Spot;

	class LightBound
	{
	public:
		LightBound(Light* light);
		~LightBound();

		virtual void update() {}
		inline std::shared_ptr<Node> getNode() { return node; }

	protected:
		std::shared_ptr<Node> node;
		Light* light;
	};

	class DirectionalBound : public LightBound
	{
	public:
		DirectionalBound(Directional* directional);
		~DirectionalBound();

		void update() override;

	private:
		std::shared_ptr<Ray> ray;
	};

	class PointBound : public LightBound
	{
	public:
		PointBound(Point* point, float radius = 2.0f);
		~PointBound();

		void update() override;

	private:
		float radius;
		std::shared_ptr<Node> circle_node_x;
		std::shared_ptr<Node> circle_node_z;
		std::shared_ptr<Circle> circle;
	};

	class SpotBound : public LightBound
	{
	public:
		SpotBound(Spot* spot);
		~SpotBound();

	private:
		float distance;
		float angle;
	};

}
