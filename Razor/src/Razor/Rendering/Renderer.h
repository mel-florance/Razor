#pragma once

#include "Pipeline.h"

namespace Razor
{

	class Renderer
	{
	public:
		Renderer();
		~Renderer();

		virtual void update() {}
		virtual void render() {}
		virtual void clear()  {}

	protected:
		std::map<std::string, std::unique_ptr<Pipeline>> pipelines;
	};

}
