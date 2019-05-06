#include "rzpch.h"
#include "GridAxis.h"
#include "Razor/Scene/Node.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Materials/Presets/ColorMaterial.h"

namespace Razor
{

	GridAxis::GridAxis(Editor* editor) :
		EditorTool(editor),
		show_grid(true),
		show_axis(true),
		grid_size(8)
	{
		grid_node = new Node();
		axis_node = new Node();

		grid = new Grid((float)grid_size);

		ColorMaterial* grid_mat = new ColorMaterial();
		grid_mat->setColor(glm::vec3(0.4f, 0.4f, 0.4f));

		grid->setMaterial(grid_mat);
		grid_node->meshes.push_back(grid);

		ColorMaterial* axis_mat_x = new ColorMaterial();
		axis_mat_x->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		axis.x = new Ray(glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(-8.0f, 0.0f, 0.0f), 2.0f);

		ColorMaterial* axis_mat_y = new ColorMaterial();
		axis_mat_y->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		axis.y = new Ray(glm::vec3(0.0f, 8.0f, 0.0f), glm::vec3(0.0f, -8.0f, 0.0f), 2.0f);

		ColorMaterial* axis_mat_z = new ColorMaterial();
		axis_mat_z->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
		axis.z = new Ray(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, -8.0f), 2.0f);

		axis.x->setLineWidth(0.5f);
		axis.y->setLineWidth(0.5f);
		axis.z->setLineWidth(0.5f);

		axis.x->setMaterial(axis_mat_x);
		axis.y->setMaterial(axis_mat_y);
		axis.z->setMaterial(axis_mat_z);

		axis_node->meshes.push_back(axis.x);
		axis_node->meshes.push_back(axis.y);
		axis_node->meshes.push_back(axis.z);
	}

	GridAxis::~GridAxis()
	{
		delete grid_node;
		delete axis_node;
	}

}
