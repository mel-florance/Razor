#pragma once

#include "../Interface.h"

namespace Razor {
	class Texture;
}

class Options : public Interface
{
public:
	Options();
	~Options();

	virtual void render() override;

	void display_general();
	void display_video();
	void display_audio();
	void display_gameplay();
	void display_controls();

	std::vector<char*> sections;
	char* selected_section;
};

