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
		grid_size(8),
		axis_size(8)
	{
		grid_node = std::make_shared<Node>();
		axis_node = std::make_shared<Node>();

		axis = new Axis(axis_size);
		grid = std::make_shared<Grid>(grid_size);

		axis_node->meshes.push_back(axis->x_ray);
		axis_node->meshes.push_back(axis->y_ray);
		axis_node->meshes.push_back(axis->z_ray);

		grid_mat = std::make_shared<ColorMaterial>();
		grid_mat->setColor(glm::vec3(0.4f, 0.4f, 0.4f));

		grid->setMaterial(grid_mat);
		grid_node->meshes.push_back(grid);
	}

	GridAxis::Axis::Axis(int axis_size)
	{
		x_ray = std::make_shared<Ray>(glm::vec3(axis_size, 0.0f, 0.0f), glm::vec3(-axis_size, 0.0f, 0.0f), 2.0f);
		y_ray = std::make_shared<Ray>(glm::vec3(0.0f, axis_size, 0.0f), glm::vec3(0.0f, -axis_size, 0.0f), 2.0f);
		z_ray = std::make_shared<Ray>(glm::vec3(0.0f, 0.0f, axis_size), glm::vec3(0.0f, 0.0f, -axis_size), 2.0f);

		x_material = std::make_shared<ColorMaterial>();
		y_material = std::make_shared<ColorMaterial>();
		z_material = std::make_shared<ColorMaterial>();

		x_ray->setLineWidth(0.5f);
		x_material->setColor(glm::vec3(1.0f, 0.0f, 0.0f));
		x_ray->setMaterial(x_material);
		y_ray->setLineWidth(0.5f);
		y_material->setColor(glm::vec3(0.0f, 1.0f, 0.0f));
		y_ray->setMaterial(y_material);
		z_ray->setLineWidth(0.5f);
		z_material->setColor(glm::vec3(0.0f, 0.0f, 1.0f));
		z_ray->setMaterial(z_material);
	}

	GridAxis::Axis::~Axis()
	{

	}

	GridAxis::~GridAxis()
	{
		delete axis;
	}
}
