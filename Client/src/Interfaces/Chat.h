#pragma once

#include "../Interface.h"

namespace Razor {
	class Texture;
}

class Chat : public Interface
{
public:
	Chat();
	~Chat();

	virtual void render() override;
};

