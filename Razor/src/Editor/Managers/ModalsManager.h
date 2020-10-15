#pragma once

#include "Editor/EditorManager.h"
#include "Editor/EditorModal.h"

namespace Razor
{

	class ModalsManager : public EditorManager
	{
	public:
		ModalsManager(Editor* editor);
		~ModalsManager();

		typedef std::unordered_map<const char*, EditorModal*> ModalsMap;

		inline ModalsMap& getModals() { return modals; }

	private:
		ModalsMap modals;
	};

}