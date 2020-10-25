#pragma once

#include "../Interface.h"

namespace Razor {
	class Texture;
}

class Home : public Interface
{
public:
	Home();
	~Home();

	virtual void render() override;
};

