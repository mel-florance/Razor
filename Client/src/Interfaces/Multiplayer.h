#pragma once

#include "../Interface.h"

class Multiplayer : public Interface
{
public:
	Multiplayer();
	~Multiplayer();

	virtual void render() override;

	bool show_game_name_error;
	char game_name_error[20];
};

