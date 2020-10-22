#pragma once

#include "rzpch.h"

#include "Core.h"
#include "Razor/Events/Event.h"

namespace Razor 
{

	struct WindowProps
	{
		std::string Title;
		float Width;
		float Height;

		WindowProps(const std::string& title = "Razor Engine",
			float width = 1280.0f,
			float height = 720.0f)
			: Title(title), Width(width), Height(height)
		{
		}
	};

	class Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;

		virtual ~Window() {}

		virtual void OnUpdate() = 0;

		virtual float GetWidth() const = 0;
		virtual float GetHeight() const = 0;

		virtual void SetEventCallback(const EventCallbackFn& callback) = 0;
		virtual void SetVSync(bool enabled) = 0;
		virtual bool IsVSync() const = 0;

		virtual void* GetNativeWindow() const = 0;
		virtual void Maximize() const = 0;

		static Window* Create(const WindowProps& props = WindowProps());
	};

}