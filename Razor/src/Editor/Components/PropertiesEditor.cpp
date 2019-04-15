#include "rzpch.h"
#include "imgui.h"
#include "PropertiesEditor.h"

namespace Razor {

	PropertiesEditor::PropertiesEditor(Editor* editor) : EditorComponent(editor)
	{
		Documents.push_back(MyDocument("Properties", true, ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
		Documents.push_back(MyDocument("World", true, ImVec4(0.4f, 0.8f, 0.4f, 1.0f)));
		Documents.push_back(MyDocument("Levels", true, ImVec4(0.8f, 0.5f, 1.0f, 1.0f)));
	}

	PropertiesEditor::~PropertiesEditor()
	{
	}

	void PropertiesEditor::render(float delta)
	{
		static PropertiesEditor app(this->editor);

		ImGui::Begin("Properties Editor");

		if (ImGui::TreeNode("Groups"))
		{
			ImGui::BeginGroup();
			{
				ImGui::BeginGroup();
				ImGui::Button("AAA");
				ImGui::SameLine();
				ImGui::Button("BBB");
				ImGui::SameLine();
				ImGui::BeginGroup();
				ImGui::Button("CCC");
				ImGui::Button("DDD");
				ImGui::EndGroup();
				ImGui::SameLine();
				ImGui::Button("EEE");
				ImGui::EndGroup();
				if (ImGui::IsItemHovered())
					ImGui::SetTooltip("First group hovered");
			}
			// Capture the group size and create widgets using the same size
			ImVec2 size = ImGui::GetItemRectSize();
			const float values[5] = { 0.5f, 0.20f, 0.80f, 0.60f, 0.25f };
			ImGui::PlotHistogram("##values", values, IM_ARRAYSIZE(values), 0, NULL, 0.0f, 1.0f, size);

			ImGui::Button("ACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
			ImGui::SameLine();
			ImGui::Button("REACTION", ImVec2((size.x - ImGui::GetStyle().ItemSpacing.x)*0.5f, size.y));
			ImGui::EndGroup();
			ImGui::SameLine();

			ImGui::Button("LEVERAGE\nBUZZWORD", size);
			ImGui::SameLine();

			if (ImGui::ListBoxHeader("List", size))
			{
				ImGui::Selectable("Selected", true);
				ImGui::Selectable("Not Selected", false);
				ImGui::ListBoxFooter();
			}

			ImGui::TreePop();
		}

		static bool opt_reorderable = true;
		static ImGuiTabBarFlags opt_fitting_flags = ImGuiTabBarFlags_FittingPolicyDefault_;

		// Menu
		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				int open_count = 0;
				for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
					open_count += app.Documents[doc_n].Open ? 1 : 0;

				if (ImGui::BeginMenu("Open", open_count < app.Documents.Size))
				{
					for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
					{
						MyDocument* doc = &app.Documents[doc_n];
						if (!doc->Open)
							if (ImGui::MenuItem(doc->Name))
								doc->DoOpen();
					}
					ImGui::EndMenu();
				}
				if (ImGui::MenuItem("Close All Documents", NULL, false, open_count > 0))
					for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
						app.Documents[doc_n].DoQueueClose();
				if (ImGui::MenuItem("Exit", "Alt+F4")) {}
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		// [Debug] List documents with one checkbox for each
		//for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
		//{
		//	MyDocument* doc = &app.Documents[doc_n];
		//	if (doc_n > 0)
		//		ImGui::SameLine();
		//	ImGui::PushID(doc);
		//	if (ImGui::Checkbox(doc->Name, &doc->Open))
		//		if (!doc->Open)
		//			doc->DoForceClose();
		//	ImGui::PopID();
		//}

		//ImGui::Separator();

		// Submit Tab Bar and Tabs
		{
			ImGuiTabBarFlags tab_bar_flags = (opt_fitting_flags) | (opt_reorderable ? ImGuiTabBarFlags_None : 0);
			if (ImGui::BeginTabBar("##tabs", tab_bar_flags))
			{
				if (opt_reorderable)
					NotifyOfDocumentsClosedElsewhere(app);

				// [DEBUG] Stress tests
				//if ((ImGui::GetFrameCount() % 30) == 0) docs[1].Open ^= 1;            // [DEBUG] Automatically show/hide a tab. Test various interactions e.g. dragging with this on.
				//if (ImGui::GetIO().KeyCtrl) ImGui::SetTabItemSelected(docs[1].Name);  // [DEBUG] Test SetTabItemSelected(), probably not very useful as-is anyway..

				// Submit Tabs
				for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
				{
					MyDocument* doc = &app.Documents[doc_n];
					if (!doc->Open)
						continue;

					ImGuiTabItemFlags tab_flags = (doc->Dirty ? ImGuiTabItemFlags_UnsavedDocument | ImGuiTabItemFlags_NoCloseButton : ImGuiTabItemFlags_NoCloseButton);
					bool visible = ImGui::BeginTabItem(doc->Name, &doc->Open, tab_flags);

					// Cancel attempt to close when unsaved add to save queue so we can display a popup.
					if (!doc->Open && doc->Dirty)
					{
						doc->Open = true;
						doc->DoQueueClose();
					}

					MyDocument::DisplayContextMenu(doc);
					if (visible)
					{
						MyDocument::DisplayContents(doc);
						ImGui::EndTabItem();
					}
				}

				ImGui::EndTabBar();
			}
		}

		// Update closing queue
		static ImVector<MyDocument*> close_queue;
		if (close_queue.empty())
		{
			// Close queue is locked once we started a popup
			for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
			{
				MyDocument* doc = &app.Documents[doc_n];
				if (doc->WantClose)
				{
					doc->WantClose = false;
					close_queue.push_back(doc);
				}
			}
		}

		// Display closing confirmation UI
		if (!close_queue.empty())
		{
			int close_queue_unsaved_documents = 0;
			for (int n = 0; n < close_queue.Size; n++)
				if (close_queue[n]->Dirty)
					close_queue_unsaved_documents++;

			if (close_queue_unsaved_documents == 0)
			{
				// Close documents when all are unsaved
				for (int n = 0; n < close_queue.Size; n++)
					close_queue[n]->DoForceClose();
				close_queue.clear();
			}
			else
			{
				if (!ImGui::IsPopupOpen("Save?"))
					ImGui::OpenPopup("Save?");
				if (ImGui::BeginPopupModal("Save?"))
				{
					ImGui::Text("Save change to the following items?");
					ImGui::PushItemWidth(-1.0f);
					ImGui::ListBoxHeader("##", close_queue_unsaved_documents, 6);
					for (int n = 0; n < close_queue.Size; n++)
						if (close_queue[n]->Dirty)
							ImGui::Text("%s", close_queue[n]->Name);
					ImGui::ListBoxFooter();

					if (ImGui::Button("Yes", ImVec2(80, 0)))
					{
						for (int n = 0; n < close_queue.Size; n++)
						{
							if (close_queue[n]->Dirty)
								close_queue[n]->DoSave();
							close_queue[n]->DoForceClose();
						}
						close_queue.clear();
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("No", ImVec2(80, 0)))
					{
						for (int n = 0; n < close_queue.Size; n++)
							close_queue[n]->DoForceClose();
						close_queue.clear();
						ImGui::CloseCurrentPopup();
					}
					ImGui::SameLine();
					if (ImGui::Button("Cancel", ImVec2(80, 0)))
					{
						close_queue.clear();
						ImGui::CloseCurrentPopup();
					}
					ImGui::EndPopup();
				}
			}
		}

		
		ImGui::Separator();

		ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(2, 2));
		ImGui::Columns(2);

		struct funcs
		{
			static void ShowDummyObject(const char* prefix, int uid)
			{
				ImGui::PushID(uid);
				ImGui::AlignTextToFramePadding();
				bool node_open = ImGui::TreeNode("Object", "%s_%u", prefix, uid);
				ImGui::NextColumn();
				ImGui::AlignTextToFramePadding();
				ImGui::Text("my sailor is rich");
				ImGui::NextColumn();
				if (node_open)
				{
					static float dummy_members[8] = { 0.0f,0.0f,1.0f,3.1416f,100.0f,999.0f };

					for (int i = 0; i < 8; i++)
					{
						ImGui::PushID(i);
						if (i < 2)
						{
							//ShowDummyObject("Child", 424242);
						}
						else
						{
							ImGui::AlignTextToFramePadding();
							ImGui::TreeNodeEx("Field", ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen | ImGuiTreeNodeFlags_Bullet, "Field_%d", i);
							ImGui::NextColumn();
							ImGui::PushItemWidth(-1);
							if (i >= 5)
								ImGui::InputFloat("##value", &dummy_members[i], 1.0f);
							else
								ImGui::DragFloat("##value", &dummy_members[i], 0.01f);
							ImGui::PopItemWidth();
							ImGui::NextColumn();
						}
						ImGui::PopID();
					}
					ImGui::TreePop();
				}
				ImGui::PopID();
			}
		};

		for (int obj_i = 0; obj_i < 3; obj_i++)
			funcs::ShowDummyObject("Object", obj_i);

		ImGui::Columns(1);
		ImGui::Separator();
		ImGui::PopStyleVar();

		ImGui::End();
	
	}

}