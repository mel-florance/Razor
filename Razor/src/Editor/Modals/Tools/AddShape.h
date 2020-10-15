#pragma once

#include "Editor/EditorModal.h"

namespace Razor
{
	class Editor;

	class AddShape : public EditorModal
	{
	public:
		enum ShapeType
		{
			CIRCLE,
			QUAD,
			PLANE,
			CUBE,
			UVSPHERE,
			CONE,
			BOUNDING,
			GRID,
			RAY
		};

		AddShape(Editor* editor);
		~AddShape();

		void init() override;
		void render() override;

		inline void setCurrentShape(ShapeType shape) { current_shape = shape; }
		inline ShapeType getCurrentShape() { return current_shape; }

		inline std::string currentShapeToStr()
		{
			switch (current_shape) 
			{
			default:
				case ShapeType::CIRCLE   : return "circle";
				case ShapeType::QUAD     : return "quad";
				case ShapeType::PLANE    : return "plane";
				case ShapeType::CUBE     : return "cube";
				case ShapeType::UVSPHERE : return "UVSphere";
				case ShapeType::CONE     : return "cone";
				case ShapeType::BOUNDING : return "bounding box";
				case ShapeType::GRID     : return "grid";
				case ShapeType::RAY      : return "ray";
			}
		}

	private:
		ShapeType current_shape;
	};

}
