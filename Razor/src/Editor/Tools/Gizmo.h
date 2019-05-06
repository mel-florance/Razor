#pragma once

#include "Editor/EditorTool.h"

namespace Razor
{

	class Gizmo : public EditorTool
	{
	public:
		Gizmo(Editor* editor);
		~Gizmo();
	};

}
