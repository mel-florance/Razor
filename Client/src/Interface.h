#pragma once

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

#include <unordered_map>

namespace Razor {
	class Texture;
}

class Controller;

class Interface {
public:
	Interface();
	~Interface();

	virtual void render() {};

	Controller* controller;
	std::unordered_map<std::string, Razor::Texture*> textures;
};