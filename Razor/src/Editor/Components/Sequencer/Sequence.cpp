#include "rzpch.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "Sequence.h"
#include "Razor/ImGui/ImCurveEdit.h"

namespace Razor
{

	Sequence::Sequence() :
		frame_min(0),
		frame_max(0)
	{
	}

	Sequence::~Sequence()
	{
	}

	void Sequence::DoubleClick(int index)
	{
		if (items[index].expanded)
		{
			items[index].expanded = false;
			return;
		}

		for (auto& item : items)
			item.expanded = false;

		items[index].expanded = !items[index].expanded;
	}

	void Sequence::CustomDraw(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& legendRect, const ImRect& clippingRect, const ImRect& legendClippingRect)

	{
		static const char* labels[] = { "Translation", "Rotation" , "Scale" };

		rampEdit.mMax = ImVec2(float(frame_max), 1.f);
		rampEdit.mMin = ImVec2(float(frame_min), 0.f);
		draw_list->PushClipRect(legendClippingRect.Min, legendClippingRect.Max, true);

		for (int i = 0; i < 3; i++)
		{
			ImVec2 pta(legendRect.Min.x + 30, legendRect.Min.y + i * 14.f);
			ImVec2 ptb(legendRect.Max.x, legendRect.Min.y + (i + 1) * 14.f);
			draw_list->AddText(pta, rampEdit.mbVisible[i] ? 0xFFFFFFFF : 0x80FFFFFF, labels[i]);

			if (ImRect(pta, ptb).Contains(ImGui::GetMousePos()) && ImGui::IsMouseClicked(0))
				rampEdit.mbVisible[i] = !rampEdit.mbVisible[i];
		}

		draw_list->PopClipRect();

		ImGui::SetCursorScreenPos(rc.Min);
		ImCurveEdit::Edit(rampEdit, rc.Max - rc.Min, 137 + index, &clippingRect);
	}

	void Sequence::CustomDrawCompact(int index, ImDrawList* draw_list, const ImRect& rc, const ImRect& clippingRect)
	{
		rampEdit.mMax = ImVec2(float(frame_max), 1.f);
		rampEdit.mMin = ImVec2(float(frame_min), 0.f);
		draw_list->PushClipRect(clippingRect.Min, clippingRect.Max, true);

		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < rampEdit.mPointCount[i]; j++)
			{
				float p = rampEdit.mPts[i][j].x;

				if (p < items[index].frame_start || p > items[index].frame_end)
					continue;

				float r = (p - frame_min) / float(frame_max - frame_min);
				float x = ImLerp(rc.Min.x, rc.Max.x, r);
				draw_list->AddLine(ImVec2(x, rc.Min.y + 6), ImVec2(x, rc.Max.y - 4), 0xAA000000, 4.f);
			}
		}

		draw_list->PopClipRect();
	}

}
