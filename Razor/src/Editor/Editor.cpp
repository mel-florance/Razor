#include "rzpch.h"
#include "Editor.h"

#include "imgui.h"

namespace Razor {

	Editor::Editor()
	{
		m_ImGuiLayer = new ImGuiLayer();
		AssimpImporter importer;
		importer.load("test");
	}

	void Editor::OnUpdate()
	{
		if (Razor::Input::IsKeyPressed(RZ_KEY_TAB))
		{
			RZ_INFO("Tab key is pressed!(poll)");
		}
	}

	void Editor::OnEvent(Razor::Event& event)
	{
		if (event.GetEventType() == Razor::EventType::KeyPressed)
		{
			Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;

			if (e.GetKeyCode() == RZ_KEY_TAB)
			{

			}

			RZ_INFO("{0}", (char)e.GetKeyCode());
		}
	}

	void Editor::OnImGuiRender()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);


		bool p_open;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_DockNodeHost;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &p_open, window_flags);
		ImGui::PopStyleVar();


		ImGuiIO& io = ImGui::GetIO();
		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
		}

		//ImGui::SetWindowFontScale(1.1f);

		if (ImGui::BeginMenuBar())
		{
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("New", "Ctrl + N", false, true);
				ImGui::MenuItem("Open", "Ctrl + O", false, true);
				ImGui::MenuItem("Open Recent...", "Ctrl + Shift + O", false, true);
				ImGui::Separator();
				ImGui::MenuItem("Save", "Ctrl + S", false, true);
				ImGui::MenuItem("Save As...", "Ctrl + Shift + S", false, true);
				ImGui::Separator();
				ImGui::MenuItem("Preferences", NULL, false, true);
				ImGui::Separator();
				ImGui::MenuItem("Import...", NULL, false, true);
				ImGui::MenuItem("Export...", NULL, false, true);
				ImGui::Separator();
				ImGui::MenuItem("Quit", "Ctrl + Q", false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Edit"))
			{
				ImGui::MenuItem("Cut", "Ctrl + X", false, true);
				ImGui::MenuItem("Copy", "Ctrl + C", false, true);
				ImGui::MenuItem("Paste", "Ctrl + V", false, true);
				ImGui::MenuItem("Delete", "Del", false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Window"))
			{
				ImGui::MenuItem("Toggle Console", NULL, false, true);

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Help"))
			{
				ImGui::MenuItem("Manual", NULL, false, true);
				ImGui::MenuItem("Release notes", NULL, false, true);
				ImGui::MenuItem("Report a bug", NULL, false, true);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		ImGui::End();

		ImGui::Begin("Tools");
		ImGui::End();

		ImGui::Begin("Viewport");
		ImGui::End();

		ImGui::Begin("Assets Manager");
		ImGui::End();

		ImGui::Begin("Outliner");
		ImGui::End();
	}

	Editor::~Editor()
	{
		delete m_ImGuiLayer;
	}

}