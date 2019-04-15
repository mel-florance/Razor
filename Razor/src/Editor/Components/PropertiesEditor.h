#pragma once

#include "Razor/Core/Core.h"
#include "Editor/EditorComponent.h"

namespace Razor {

	struct MyDocument
	{
		const char* Name;           // Document title
		bool        Open;           // Set when the document is open (in this demo, we keep an array of all available documents to simplify the demo)
		bool        OpenPrev;       // Copy of Open from last update.
		bool        Dirty;          // Set when the document has been modified
		bool        WantClose;      // Set when the document
		ImVec4      Color;          // An arbitrary variable associated to the document

		MyDocument(const char* name, bool open = true, const ImVec4& color = ImVec4(1.0f, 1.0f, 1.0f, 1.0f))
		{
			Name = name;
			Open = OpenPrev = open;
			Dirty = false;
			WantClose = false;
			Color = color;
		}
		void DoOpen() { Open = true; }
		void DoQueueClose() { WantClose = true; }
		void DoForceClose() { Open = false; Dirty = false; }
		void DoSave() { Dirty = false; }

		// Display dummy contents for the Document
		static void DisplayContents(MyDocument* doc)
		{
			ImGui::PushID(doc);
			ImGui::Text("Document \"%s\"", doc->Name);
			ImGui::PushStyleColor(ImGuiCol_Text, doc->Color);
			ImGui::TextWrapped("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua.");
			ImGui::PopStyleColor();
			if (ImGui::Button("Modify", ImVec2(100, 0)))
				doc->Dirty = true;
			ImGui::SameLine();
			if (ImGui::Button("Save", ImVec2(100, 0)))
				doc->DoSave();
			ImGui::ColorEdit3("color", &doc->Color.x);  // Useful to test drag and drop and hold-dragged-to-open-tab behavior.
			ImGui::PopID();
		}

		// Display context menu for the Document
		static void DisplayContextMenu(MyDocument* doc)
		{
			if (!ImGui::BeginPopupContextItem())
				return;

			char buf[256];
			sprintf(buf, "Save %s", doc->Name);
			if (ImGui::MenuItem(buf, "CTRL+S", false, doc->Open))
				doc->DoSave();
			if (ImGui::MenuItem("Close", "CTRL+W", false, doc->Open))
				doc->DoQueueClose();
			ImGui::EndPopup();
		}
	};

	class RAZOR_API PropertiesEditor : public EditorComponent
	{
	public:
		PropertiesEditor(Editor* editor);
		~PropertiesEditor();

		ImVector<MyDocument> Documents;

		static void NotifyOfDocumentsClosedElsewhere(PropertiesEditor& app)
		{
			for (int doc_n = 0; doc_n < app.Documents.Size; doc_n++)
			{
				MyDocument* doc = &app.Documents[doc_n];
				if (!doc->Open && doc->OpenPrev)
					ImGui::SetTabItemClosed(doc->Name);
				doc->OpenPrev = doc->Open;
			}
		}

		void render(float delta) override;
	};

	
}
