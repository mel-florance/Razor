#pragma once

#include "../Interface.h"

namespace Razor {
	class Texture;
}

class Lobby : public Interface
{
public:
	Lobby();
	~Lobby();

	virtual void render() override;

	bool ready_button_hovered;
};

