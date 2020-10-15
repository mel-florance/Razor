#pragma once

#include "Editor/EditorComponent.h"

namespace Razor
{

	static const char* SequencerItemTypeNames[] =
	{ 
		"Camera",
		"Music", 
		"ScreenEffect", 
		"FadeIn", 
		"Animation" 
	};

	class Sequence;

	class Sequencer : public EditorComponent
	{
	public:
		Sequencer(Editor* editor);
		~Sequencer();

		enum class ItemType
		{
			CAMERA,
			MUSIC,
			ANIMATION,
			FADE_IN,
			FADE_OUT
		};

		void render(float delta) override;

	private:
		Sequence* sequence;

		int selected_entry;
		int current_frame;
		int first_frame;
		bool expanded;
	};

}
