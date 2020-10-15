#pragma once

#include "Editor/EditorModal.h"

namespace Razor
{

	class ImportAsset : public EditorModal
	{
	public:
		ImportAsset(Editor* editor);
		~ImportAsset();

		void render() override;
	};

}
