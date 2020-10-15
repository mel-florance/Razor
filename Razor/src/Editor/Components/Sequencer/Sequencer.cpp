#include "rzpch.h"
#include "Sequencer.h"
#include "Sequence.h"

namespace Razor
{

	Sequencer::Sequencer(Editor* editor) :
		EditorComponent(editor),
		selected_entry(-1),
		first_frame(0),
		expanded(true),
		current_frame(100)
	{
		sequence = new Sequence();
		sequence->frame_min = -100;
		sequence->frame_max = 1000;
		sequence->items.push_back(Sequence::SequenceItem{ 0, 10, 30, false });
		sequence->items.push_back(Sequence::SequenceItem{ 1, 20, 30, true });
		sequence->items.push_back(Sequence::SequenceItem{ 3, 12, 60, false });
		sequence->items.push_back(Sequence::SequenceItem{ 2, 61, 90, false });
		sequence->items.push_back(Sequence::SequenceItem{ 4, 90, 99, false });
	}

	Sequencer::~Sequencer()
	{
		delete sequence;
	}

	void Sequencer::render(float delta)
	{
		ImGui::Begin("Sequencer");

		ImGui::PushItemWidth(130);
		ImGui::InputInt("Frame Min", &sequence->frame_min);
		ImGui::SameLine();
		ImGui::InputInt("Frame ", &current_frame);
		ImGui::SameLine();
		ImGui::InputInt("Frame Max", &sequence->frame_max);
		ImGui::PopItemWidth();

		ImSequencer::SequencerInstance(sequence, &current_frame, &expanded, &selected_entry, &first_frame,
			ImSequencer::SEQUENCER_EDIT_STARTEND |
			ImSequencer::SEQUENCER_ADD           | 
			ImSequencer::SEQUENCER_DEL           | 
			ImSequencer::SEQUENCER_COPYPASTE     | 
			ImSequencer::SEQUENCER_CHANGE_FRAME);
		
		// add a UI to edit that particular item
		if (selected_entry != -1)
		{
			//const Sequence::SequenceItem& item = sequence->items[selected_entry];
			//ImGui::Text("I am a %s, please edit me", SequencerItemTypeNames[item.type]);
			// switch (type) ....
		}

		ImGui::End();
	}

}
