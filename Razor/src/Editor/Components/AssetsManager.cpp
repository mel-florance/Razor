#include "rzpch.h"
#include "imgui.h"
#include "AssetsManager.h"
#include "FileBrowser.h"
#include "Logger.h"
#include "Editor/Editor.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Materials/Texture.h"

namespace fs = std::filesystem;

namespace Razor 
{

	AssetsManager::ExtsMap AssetsManager::exts = {
		{AssetsManager::Type::Model, { "3ds",  "blend", "dae",  "fbx",  "gltf", "obj", "raw",  "stl", "lwo"}},
		{AssetsManager::Type::Image, { "jpg",  "png", "dds", "psd", "hdr", "tga"}},
		{AssetsManager::Type::Audio, { "wav"/*,"ogg", "aac",   "aiff", "flac", "mp3" */}},
		{AssetsManager::Type::Video, { "webm", "flv",   "ogv",  "mov",  "mp4",  "3gp"}}
	};

	AssetsManager::ExtsMap AssetsManager::internal_exts = {
		{AssetsManager::Type::Model,  { "rzmesh" }},
		{AssetsManager::Type::Image,  { "rztexture"}},
		{AssetsManager::Type::Audio,  { "rzaudio" }},
		{AssetsManager::Type::Video,  { "rzvideo" }},
		{AssetsManager::Type::Script, { "rzscript" }}
	};

	TexturesManager* AssetsManager::texturesManager = nullptr;
	std::unique_ptr<AssimpImporter> AssetsManager::importer = nullptr;
	bool AssetsManager::show_import_modal = false;
	std::string AssetsManager::current_import_path = std::string();
	TaskFinished AssetsManager::import_callback = TaskFinished();

	AssetsManager::AssetsManager(Editor* editor) : 
		EditorComponent(editor)
	{
		texturesManager = new TexturesManager();

		this->directoryWatcher = std::make_shared<DirectoryWatcher>("F:/Razor/Razor/Sandbox", std::chrono::milliseconds(1000));
		this->fileWatcher = std::make_shared<FileWatcher>();
		this->fileBrowser = std::make_shared<FileBrowser>(editor);
		importer = std::make_unique<AssimpImporter>();

		watch();
	}

	void AssetsManager::watch()
	{
		std::thread directoryWatcher_thread(&DirectoryWatcher::start, this->directoryWatcher,
		[=](std::string path, DirectoryWatcher::Status status)
		{
			if (!std::filesystem::is_regular_file(std::filesystem::path(path))
			&& status != DirectoryWatcher::Status::erased)
				return;

			switch (status) {
			case DirectoryWatcher::Status::created:
				Log::info("File created: %s", path.c_str());
				break;
			case DirectoryWatcher::Status::modified:
				/*if (path == "./Razor.log") {
					std::vector<std::string> lines = fileWatcher->tail("./Razor.log", 1);
					Logger* logger = (Logger*)this->getEditor()->getComponents()["Logger"];

					for (auto line : lines)
						logger->addLog(("\n" + line).c_str());
				}*/
				//RZ_INFO("File modified: {0}", path);
				break;
			case DirectoryWatcher::Status::erased:
				//RZ_WARN("File erased: {0}", path);
				break;
				//RZ_ERROR("Error! Unknown file status.");
			}
		});

		directoryWatcher_thread.detach();
	}

	void AssetsManager::render(float delta)
	{
		ImGui::Begin("Assets Manager");

		if(this->fileBrowser != nullptr)
			this->fileBrowser->render();

		if (show_import_modal)
		{
			ImGui::SetNextWindowPosCenter(ImGuiCond_Once);
			ImGui::SetNextWindowFocus();

			if (ImGui::Begin("Import", &show_import_modal, ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse))
			{
				ImGui::Dummy(ImVec2(0, 5.0f));
				ImGui::Columns(2, "twoColumns", true);

				static unsigned short initial_column_spacing = 0;
				if (initial_column_spacing < 2)
				{
					ImGui::SetColumnWidth(0, 100.0f);
					initial_column_spacing++;
				}

				ImGui::Columns(1);
				ImGui::Dummy(ImVec2(0, 50.0f));

				float width = ImGui::GetWindowWidth();
				float pos_y = ImGui::GetWindowHeight();

				ImGui::SetCursorPos(ImVec2(12.0f, pos_y - 45.0f));
				ImGui::PushItemWidth(-1);
				ImGui::Button("IMPORT", ImVec2(width - 25.0f, 30.0f));
				ImGui::PopItemWidth();

				if (ImGui::IsItemClicked())
				{
					std::string path = (fs::path(FileBrowser::getCurrentPath()) / fs::path(current_import_path).filename()).string();
				
					fs::copy(current_import_path, path);

					if (importer->importMesh(path))
					{
						Node* result = importer->getNodeData();
						import_callback(result);
						importer->resetRootNode();
					}
				}

				ImGui::End();
			}
		}

		ImGui::End();
	}

	void AssetsManager::import(Node* result, TaskFinished tf, Variant opts)
	{
		Log::trace("Task Arguments: %s", opts.toString().c_str());

		if (opts.type() == Variant::Type::String)
		{
			std::string str = opts.toString();
			auto ext = str.substr(str.find_last_of(".") + 1);
			auto type = AssetsManager::getExt(ext);
			
			if (type == Type::Model) {
				current_import_path = opts.toString();
				import_callback = tf;
				show_import_modal = true;
				if (importer->importMesh(opts.toString()))
				{
					result = importer->getNodeData();
					tf(result);
					importer->resetRootNode();
				}
			}
			else if (type == Type::Image) {
				current_import_path = opts.toString();
				std::string path = (fs::path(FileBrowser::getCurrentPath()) / fs::path(current_import_path).filename()).string();
				fs::copy(current_import_path, path);

				Texture* tex = new Texture(path);
				texturesManager->addTexture(path, tex);

				// TODO
				// file_browser->importFile();
			}
			else if (type == Type::Audio)
				Log::warn("No Audio format implemented for the extension: %s", ext.c_str());
			else if (type == Type::Video)
				Log::warn("No Video format implemented for the extension: %s", ext.c_str());
			else
				Log::error("Unrecognized file extension: %s", ext.c_str());
		}
	}



	AssetsManager::~AssetsManager()
	{
	}





}
