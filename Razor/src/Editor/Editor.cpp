#include "rzpch.h"
#include "imgui.h"
#include "Editor.h"

#include "Razor/Mesh.h"
#include "Razor/Application.h"

namespace Razor {

	Editor::Editor()
	{
		m_ImGuiLayer = new ImGuiLayer();
		tasksManager = new TasksManager();
		assetsManager = new AssetsManager();
		assetsManager->watch();

		Mesh mesh, mesh1, mesh2, mesh3;
		tasksManager->add({ &mesh,  &Editor::import, &Editor::finished, Variant("data/house.fbx"), "Import task 1", 50 });
		tasksManager->add({ &mesh1, &Editor::import, &Editor::finished, Variant("data/Lia.fbx"), "Import task 2", 200 });
		tasksManager->add({ &mesh2, &Editor::import, &Editor::finished, Variant("house_wood_3.fbx"), "Import task 3", 23 });
		tasksManager->add({ &mesh3, &Editor::import, &Editor::finished, Variant("house_wood_4.fbx"), "Import task 4", 74 });
	}

	void Editor::import(void* result, TaskFinished tf, Variant opts)
	{
		RZ_CORE_TRACE("Task Arguments: {0}", opts.toString());

		AssimpImporter* importer = new AssimpImporter();
		bool imported = importer->importMesh(opts.toString());

		if (imported) {
			result = new Mesh();
			static_cast<Mesh*>(result)->setName(importer->getNodeData()->name);
			tf(result);
		}
	}

	void Editor::finished(void* result)
	{
		Mesh* mesh = static_cast<Mesh*>(result);

		if(mesh != nullptr)
			RZ_CORE_INFO("Successfully imported mesh: {0}", mesh->getName());
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

		MainMenu::setup();

		ImGui::End();

		ImGui::Begin("Tools");
		ImGui::End();

		ImGui::Begin("Assets Manager");
		ImGui::End();

		ImGui::Begin("Outliner");
		ImGui::End();

		ImGui::Begin("Console");
		ImGui::End();

		ImGui::Begin("Properties Editor");
		ImGui::End();
	}

	Editor::~Editor()
	{
		delete m_ImGuiLayer;
	}

}