#pragma once

#include "Editor/EditorManager.h"
#include "Editor/EditorTool.h"

namespace Razor
{

	class ToolsManager : public EditorManager
	{
	public:
		ToolsManager(Editor* editor);
		~ToolsManager();

		typedef std::unordered_map<const char*, EditorTool*> ToolsMap;

		inline ToolsMap& getTools() { return tools; }

		template<typename T>
		T* getTool(const char* name) {
			
			auto it = tools.find(name);

			if (it != tools.end())
				return (T*)it->second;

			return nullptr;
		}

	private:
		ToolsMap tools;
	};

}