#pragma once

#include "Razor/Application/Layer.h"

#include "Razor/Events/ApplicationEvent.h"
#include "Razor/Events/KeyEvent.h"
#include "Razor/Events/MouseEvent.h"

namespace Razor
{

	class Editor;

	class RAZOR_API ImGuiLayer : public Layer
	{
	public:
		ImGuiLayer(Editor* editor);
		~ImGuiLayer();

		void setRotation(const glm::quat &q);

		virtual void OnAttach() override;
		virtual void OnDetach() override;
		virtual void OnImGuiRender() override;
		void Begin();
		void End();
		
	private:
		float m_Time = 0.0f;
		Editor* editor;
	};

}
