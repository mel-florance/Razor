#include "rzpch.h"
#include "imgui.h"
#include "IconsManager.h"

#include "Razor/Materials/TextureAtlas.h"

namespace Razor
{

	IconsManager::IconsManager()
	{
		atlas = new TextureAtlas("./data/icons.png", false, false);

		atlas->addItem("sequencer", 0, 0);
		atlas->addItem("texture", 0, 1);
		atlas->addItem("image_curve", 0, 2);
		atlas->addItem("image_colors", 0, 3);
		atlas->addItem("image_histo", 0, 4);
		atlas->addItem("texture_mini", 0, 5);
		atlas->addItem("plane", 1, 0);
		atlas->addItem("sphere", 1, 1);
		atlas->addItem("cube", 1, 2);
		atlas->addItem("calendar", 2, 4);
		atlas->addItem("node", 5, 19);
		atlas->addItem("node_hover", 5, 20);
		atlas->addItem("point_light", 18, 12);
		atlas->addItem("directional_light", 18, 13);
		atlas->addItem("spot_light", 18, 14);
		atlas->addItem("camera", 19, 6);
		atlas->addItem("noisy_curve", 8, 18);
		atlas->addItem("clear", 29, 19);
		atlas->addItem("copy", 6, 0);
		atlas->addItem("export", 28, 18);
		atlas->addItem("record", 11, 0);
		atlas->addItem("info", 29, 1);
		atlas->addItem("joystick", 26, 7);
		atlas->addItem("gear", 15, 2);
		atlas->addItem("brush", 22, 0);
		atlas->addItem("magnify", 27, 6);
		atlas->addItem("terminal", 25, 17);
		atlas->addItem("folder", 3, 17);
		atlas->addItem("file", 3, 18);
		atlas->addItem("arrow_right", 29, 4);
		atlas->addItem("arrow_down", 29, 5);
		atlas->addItem("arrow_left", 29, 6);
		atlas->addItem("arrow_top ", 29, 7);
		atlas->addItem("arrow_previous", 2, 15);
		atlas->addItem("arrow_next", 2, 16);
		atlas->addItem("editor ", 25, 18);
		atlas->addItem("minus", 29, 10);
		atlas->addItem("plus", 29, 11);
		atlas->addItem("eye_open", 20, 19);
		atlas->addItem("eye_closed", 20, 20);
		atlas->addItem("eye_closed", 20, 20);
	}

	void IconsManager::drawIcon(const std::string& name, const glm::vec2& size, IconType type)
	{
		auto extents = atlas->getItemPosition(name);

		switch (type) {
		default:
		case IconType::IMAGE:
			ImGui::Image(
				(void*)(intptr_t)atlas->getId(),
				ImVec2(size.x, size.y),
				ImVec2(extents.x, extents.y),
				ImVec2(extents.z, extents.w)
			);
			break;
		case IconType::BUTTON:
			ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.0f);
			ImGui::ImageButton(
				(void*)(intptr_t)atlas->getId(),
				ImVec2(size.x, size.y),
				ImVec2(extents.x, extents.y),
				ImVec2(extents.z, extents.w),
				0
			);
			ImGui::PopStyleVar();
			break;
		}
	}

	IconsManager::~IconsManager()
	{
		delete atlas;
	}

}
