#pragma once

#include "RenderPass.h"

namespace Razor
{

	class Pipeline
	{
	public:
		Pipeline();
		~Pipeline();

		virtual void render() {}

	private:
		std::vector<RenderPass*> renderPasses;
	};

}
