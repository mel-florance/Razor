#pragma once
#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor
{
	class Renderer;

	class RAZOR_API EditorView : public EditorComponent
	{
	public:
		EditorView(Editor* editor);
		~EditorView();

		void render(float delta) override;

	private:
		Renderer* renderer;
	};

}

