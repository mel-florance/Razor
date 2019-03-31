#pragma once

#include "Razor/KeyCodes.h"
#include "Razor/Layer.h"
#include "Razor/Input.h"
#include "Razor/MouseButtons.h"
#include "Razor/ImGui/ImGuiLayer.h"

#include "AssimpImporter.h"

namespace Razor {

	class RZ_API Editor : public Razor::Layer
	{
	public:
		Editor();
		~Editor();

		void OnUpdate() override;
		void OnEvent(Razor::Event& event) override;
		void OnImGuiRender() override;

		inline ImGuiLayer* getLayer() { return m_ImGuiLayer;  }

	private:
		ImGuiLayer* m_ImGuiLayer;
	};

}
