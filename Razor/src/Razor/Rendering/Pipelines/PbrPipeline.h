#pragma once

#include "Razor/Rendering/Pipeline.h"

namespace Razor
{

	class PbrPipeline : public Pipeline
	{
	public:
		PbrPipeline();
		~PbrPipeline();

		void render() override;
	};

}
