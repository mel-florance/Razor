#pragma once

#include "Editor/EditorTool.h"

namespace Razor
{
	class Node;
	class Ray;
	class Grid;
	class ColorMaterial;

	class GridAxis : public EditorTool
	{
	public:
		struct Axis
		{
			Axis(int size = 8);
			~Axis();

			std::shared_ptr<Ray> x_ray;
			std::shared_ptr<Ray> y_ray;
			std::shared_ptr<Ray> z_ray;
			std::shared_ptr<ColorMaterial> x_material;
			std::shared_ptr<ColorMaterial> y_material;
			std::shared_ptr<ColorMaterial> z_material;
		};

		GridAxis(Editor* editor);
		~GridAxis();

		inline std::shared_ptr<Node> getGridNode() { return grid_node; }
		inline std::shared_ptr<Node> getAxisNode() { return axis_node; }

	private:
		bool show_grid;
		bool show_axis;
		int grid_size;
		int axis_size;

		std::shared_ptr<Node> grid_node;
		std::shared_ptr<Node> axis_node;

		std::shared_ptr<Grid> grid;
		Axis* axis;
		std::shared_ptr<ColorMaterial> grid_mat;
	};

}
