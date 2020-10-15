#include "Client.h"

#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Scene/ScenesManager.h"

#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

namespace Razor {

	Client::Client() :
		Application(false, Application::Type::CLIENT, WindowProps("Razor Client", 400.0f, 720.0f))
	{
		Razor::ScenesManager* sm = this->getScenesManager();
		Razor::Engine* engine = this->getEngine();
		auto camera = new FPSCamera(&Application::Get().GetWindow());

		auto main = new ImGuiLayer(nullptr);

		auto layer = new TestLayer(sm, engine);
		PushLayer(layer);
		PushOverlay(main);

		auto scene = sm->getActiveScene();
		scene->addCamera(camera);
		scene->setActiveCamera(camera);
	}

	Client::~Client()
	{
	}

	TestLayer::TestLayer(Razor::ScenesManager* sceneManager, Razor::Engine* engine) :
		Layer("Test"),
		sm(sceneManager),
		engine(engine),
		connected(false),
		input_host("127.0.0.1"),
		input_port("55555"),
		input_message("")
	{
		client = std::make_unique<TCPClient>();

		logo = new Razor::Texture("./data/logo.png", true);
	}

	void TestLayer::OnUpdate(float delta)
	{

	}

	void TestLayer::OnAttach()
	{

	}

	void TestLayer::OnEvent(Razor::Event& event) {

	}

	void TestLayer::OnImGuiRender()
	{
		ImGuiViewport* viewport = ImGui::GetMainViewport();
		ImGui::SetNextWindowPos(viewport->Pos);
		ImGui::SetNextWindowSize(viewport->Size);
		ImGui::SetNextWindowViewport(viewport->ID);

		bool p_open;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_DockNodeHost;
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("Razor Client", &p_open, window_flags);
		ImGui::PopStyleVar();

		ImGui::Dummy(ImVec2(-1, 20));

		unsigned int logo_size = 64;

		ImGui::SetCursorPosX((ImGui::GetWindowWidth() * 0.5f) - logo_size * 0.5f);
		ImGui::Image((void*)logo->getId(), ImVec2(logo_size, logo_size));
		ImGui::Dummy(ImVec2(-1, 20));

		std::string text = "Welcome to the razor client!";
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (ImGui::GetColumnWidth() * 0.5f) - ImGui::CalcTextSize(text.c_str()).x  * 0.5f);

		ImGui::Text("%s", text.c_str());
		ImGui::Dummy(ImVec2(-1, 20));

		auto indent = (ImGui::GetWindowSize().x - 380) / 2;
		ImGui::Indent(indent);

		if (!connected) {
			ImGui::Columns(2, "columns", false);
			ImGui::SetColumnWidth(0, 80);
			ImGui::SetColumnWidth(1, 300);
		
			ImGui::Text("Host ");
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##ipaddr", input_host, 15);
			ImGui::PopItemWidth();
			ImGui::NextColumn();
			ImGui::Text(" Port ");
			ImGui::NextColumn();
			ImGui::PushItemWidth(-1);
			ImGui::InputText("##port", input_port, 6);
			ImGui::PopItemWidth();

			ImGui::Dummy(ImVec2(-1, 10));

			if (ImGui::Button("Connect", ImVec2(-1, 0))) {
				connected = client->Connect(input_host, std::stoi(input_port));
			}

			ImGui::Columns(1, "columns");
		}
		else {
			ImGui::Text("Connected to %s:%d", input_host, std::stoi(input_port));

			if (ImGui::Button("Test send a lot", ImVec2(ImGui::GetWindowSize().x - indent * 2, 0))) {
				for (unsigned int i = 0; i < 1000; ++i)
					client->Emit("Omg teeeest");
			}

			if (ImGui::Button("Disconnect", ImVec2(ImGui::GetWindowSize().x - indent * 2, 0))) {
				client->Close();
				connected = false;
			}

			ImGui::PushItemWidth(ImGui::GetWindowSize().x - indent * 2);

			if (ImGui::InputText("##message", input_message, 128, ImGuiInputTextFlags_EnterReturnsTrue)) {
				client->Emit(input_message);

				client->Messages.push_back({ input_message });
				memset(input_message, 0, 128);
			}

			for (auto& message : client->Messages) {
				ImGui::Text(message.text.c_str());
			}

			ImGui::PopItemWidth();
		}

		ImGui::Indent(-indent);
		ImGui::End();
	}
}