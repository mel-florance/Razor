#pragma once

#include "Razor/ImGui/ImRampEdit.h"
#include "Razor/ImGui/ImSequencer.h"

static inline ImVec2 operator-(const ImVec2& lhs, const ImVec2& rhs) { return ImVec2(lhs.x - rhs.x, lhs.y - rhs.y); }
static const char* SequencerItemTypeNames[] = { "Camera","Music", "ScreenEffect", "FadeIn", "Animation" };

namespace Razor
{

	class Sequence : public ImSequencer::SequenceInterface
	{
	public:
		Sequence();
		~Sequence();

		struct SequenceItem
		{
			int type;
			int frame_start, frame_end;
			bool expanded;
		};

		virtual void DoubleClick(int index);
		virtual void CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect);
		virtual void CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect);

		virtual inline void Add(int type) { items.push_back(SequenceItem{ type, 0, 10, false }); };
		virtual inline void Del(int index) { items.erase(items.begin() + index); }
		virtual inline void Duplicate(int index) { items.push_back(items[index]); }

		virtual inline size_t GetCustomHeight(int index) { return items[index].expanded ? 300 : 0; }

		virtual inline int GetFrameMin() const { return frame_min; }
		virtual inline int GetFrameMax() const { return frame_max; }
		virtual inline int GetItemCount() const { return (int)items.size(); }

		virtual inline int GetItemTypeCount() const { return sizeof(SequencerItemTypeNames) / sizeof(char*); }
		virtual inline const char* GetItemTypeName(int typeIndex) const { return SequencerItemTypeNames[typeIndex]; }
		virtual inline const char* GetItemLabel(int index) const
		{
			static char tmps[512];
			sprintf_s(tmps, "[%02d] %s", index, SequencerItemTypeNames[items[index].type]);
			return tmps;
		}

		virtual inline void Get(int index, int** start, int** end, int* type, unsigned int* color)
		{
			SequenceItem& item = items[index];
			if (color)
				*color = 0xFFAA8080; // same color for everyone, return color based on type
			if (start)
				*start = &item.frame_start;
			if (end)
				*end = &item.frame_end;
			if (type)
				*type = item.type;
		}

		std::vector<SequenceItem> items;
		RampEdit rampEdit;
		int frame_min;
		int frame_max;
	};

}	
