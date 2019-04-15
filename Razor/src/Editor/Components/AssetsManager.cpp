#include "rzpch.h"
#include "imgui.h"
#include "AssetsManager.h"
#include "Logger.h"
#include "Editor/Editor.h"

namespace Razor {

	AssetsManager::ExtsMap AssetsManager::exts = {
		{AssetsManager::Type::Model, {"3ds",  "blend", "dae",  "fbx", "gltf", "obj", "raw",  "stl"}},
		{AssetsManager::Type::Image, {"jpg",  "png",   "dds",  "png", "psd",  "gif", "exif", "tiff", "hdr", "svg", "tga"}},
		{AssetsManager::Type::Audio, {"aac",  "aiff",  "flac", "mp3", "ogg",  "wav"}},
		{AssetsManager::Type::Video, {"webm", "flv",   "ogv",  "mov", "mp4",  "3gp"}}
	};

	AssetsManager* FileBrowser::assetsManager = nullptr;
	TasksManager* FileBrowser::tasksManager = nullptr;

	AssetsManager::AssetsManager(Editor* editor) : EditorComponent(editor)
	{
		this->directoryWatcher = std::make_shared<DirectoryWatcher>("F:/Razor/Razor/Sandbox", std::chrono::milliseconds(1000));
		this->fileWatcher = std::make_shared<FileWatcher>();
		this->fileBrowser = std::make_shared<FileBrowser>();
		FileBrowser::assetsManager = this;
		FileBrowser::tasksManager = editor->getTasksManager();

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
				RZ_INFO("File created: {0}", path);
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

		this->fileBrowser->render();

		ImGui::End();
	}

	void AssetsManager::import(void* result, TaskFinished tf, Variant opts)
	{
		RZ_TRACE("Task Arguments: {0}", opts.toString());

		if (opts.type() == Variant::Type::String)
		{
			std::string str = opts.toString();
			auto ext = str.substr(str.find_last_of(".") + 1);
			auto type = AssetsManager::getExt(ext);
			AssimpImporter* importer = new AssimpImporter();

			switch (type) {
			case Type::Model:
				if (importer->importMesh(opts.toString())) {
					result = importer->getNodeData();
					tf(result);
				}
				break;
			case Type::Image:
				RZ_WARN("No image format implemented for the extension: {0}", ext);
				break;
			case Type::Audio:
				RZ_WARN("No Audio format implemented for the extension: {0}", ext);
				break;
			case Type::Video:
				RZ_WARN("No Video format implemented for the extension: {0}", ext);
				break;
			default:
				RZ_ERROR("Unrecognized file extension: {0}", ext);
			}
		}
	}



	AssetsManager::~AssetsManager()
	{
	}





}