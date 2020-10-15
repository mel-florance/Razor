#pragma once

namespace Razor
{
	class Editor;

	class EditorModal
	{
	public:
		EditorModal(Editor* editor);
		virtual ~EditorModal();

		virtual void render() {}
		virtual void init() {}

		inline void setActive(bool value) { active = value; }
		inline bool isActive() { return active; }

	protected:
		Editor* editor;
		bool active;
	};

}
