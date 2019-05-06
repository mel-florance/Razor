#pragma once

#include "Editor/EditorTool.h"

namespace Razor
{
	class Node;
	class Ray;
	class Grid;

	class GridAxis : public EditorTool
	{
	public:
		GridAxis(Editor* editor);
		~GridAxis();

		struct Axis
		{
			Ray* x;
			Ray* y;
			Ray* z;
		};

		inline Node* getGridNode() { return grid_node; }
		inline Node* getAxisNode() { return axis_node; }

	private:
		bool show_grid;
		bool show_axis;
		int grid_size;

		Node* grid_node;
		Node* axis_node;

		Grid* grid;
		Axis axis;
	};

}
