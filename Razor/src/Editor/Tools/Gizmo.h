#pragma once

#include "Editor/EditorTool.h"
#include "Razor/ImGui/ImGuizmo.h"

namespace Razor
{
	class Node;
	class Camera;
	 
	class Gizmo : public EditorTool
	{
	public:
		Gizmo(Editor* editor);
		~Gizmo();

		void update();

		inline std::shared_ptr<Node> getNodePtr() { return node_ptr; }
		inline glm::vec2& getViewportPosition() { return viewport_position; }
		inline glm::vec2& getViewportSize() { return viewport_size; }
		inline ImGuizmo::OPERATION getCurrentOperation() { return current_operation; }
		inline ImGuizmo::MODE getCurrentMode() { return current_mode; }
		inline bool isUsingSnap() { return use_snap; }
		inline bool isBoundSizing() { return bound_sizing; }
		inline bool isBoundSizingSnap() { return bound_sizing_snap; }
		inline glm::vec3& getSnap() { return snap; }
		inline glm::mat2x3 getBounds() { return bounds; }
		inline glm::vec3 getBoundsSnap() { return bounds_snap; }
		inline Camera* getCamera() { return camera; }
		inline bool isAxisYflipped() { return axis_y_flipped; }

		inline void setNodePtr(std::shared_ptr<Node> node) { node_ptr = node; }
		inline void setViewportPosition(const glm::vec2& position) {viewport_position = position; }
		inline void setViewportSize(const glm::vec2& size) { viewport_size = size; }
		inline void setCurrentOperation(ImGuizmo::OPERATION operation) { current_operation = operation; }
		inline void setCurrentMode(ImGuizmo::MODE mode) { current_mode = mode; }
		inline void setIsUsingSnap(bool value) { use_snap = value; }
		inline void setIsBoundSizing(bool value) { bound_sizing = value; }
		inline void setIsBoundSizingSnap(bool value) { bound_sizing_snap = value; }
		inline void setSnap(const glm::vec3& value) { snap = value; }
		inline void setBounds(const glm::mat2x3& mat) { bounds = mat; }
		inline void setBoundsSnap(const glm::vec3& bounds) { bounds_snap = bounds; }
		inline void setCamera(Camera* cam) { camera = cam; }
		inline void setAxisYFlipped(bool value) { axis_y_flipped = value; }

	private:
		Camera* camera;
		std::shared_ptr<Node> node_ptr;
		glm::vec2 viewport_position;
		glm::vec2 viewport_size;
		ImGuizmo::OPERATION current_operation;
		ImGuizmo::MODE current_mode;
		bool use_snap;
		bool bound_sizing = false;
		bool bound_sizing_snap = false;
		bool axis_y_flipped;
		glm::mat4 delta_matrix;
		glm::vec3 snap;
		glm::mat2x3 bounds;
		glm::vec3 bounds_snap;
	};

}
