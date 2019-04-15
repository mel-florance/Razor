#pragma once

#include "rzpch.h"
#include "Razor/Core/Core.h"
#include "Razor/Events/Event.h"

namespace Razor {

	class RAZOR_API Layer
	{
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		virtual void OnAttach() {};
		virtual void OnDetach() {};
		virtual void OnUpdate(float delta) {};
		virtual void OnEvent(Event& event) {}
		virtual void OnImGuiRender() {}

		inline const std::string& GetName() const { return m_DebugName; }

	protected:
		std::string m_DebugName;
	};

}