#pragma once

#include "../Interface.h"

namespace Razor {
	class Texture;
}

class Game : public Interface
{
public:
	Game();
	~Game();

	virtual void render() override;
	unsigned int current_pass;
};

