#pragma once

#include "RenderPass.h"

namespace Razor {

	class Pipeline
	{
	public:
		Pipeline();
		~Pipeline();

	private:
		std::vector<RenderPass*> renderPasses;
	};

}
