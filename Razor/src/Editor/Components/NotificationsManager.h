#pragma once

#include "Editor/EditorComponent.h"

namespace Razor
{

	class NotificationsManager : public EditorComponent
	{
	public:
		NotificationsManager(Editor* editor);
		~NotificationsManager();

		void render(float delta) override;
	};

}
