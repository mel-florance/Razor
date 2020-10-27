#pragma once

#include "../Interface.h"

namespace Razor {
	class Texture;
}

class Loading : public Interface
{
public:
	Loading();
	~Loading();

	virtual void render() override;
};

