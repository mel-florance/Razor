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

	AssetsManager::AssetsManager(Editor* editor) : EditorComponent(editor)
	{
		this->directoryWatcher = std::make_shared<DirectoryWatcher>("./", std::chrono::milliseconds(16));
		this->fileWatcher = std::make_shared<FileWatcher>();
		this->fileBrowser = std::make_shared<FileBrowser>(editor);
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
				if (path == "./Razor.log") {
					std::vector<std::string> lines = fileWatcher->tail("./Razor.log", 1);
					Logger* logger = (Logger*)this->getEditor()->getComponents()["Logger"];

					for (auto line : lines)
						logger->addLog(("\n" + line).c_str());
				}
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

	void AssetsManager::render()
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

			if (type == Type::Model)
			{
				AssimpImporter* importer = new AssimpImporter();
				bool imported = importer->importMesh(opts.toString());

				if (imported) {
					result = new Mesh();
					static_cast<Mesh*>(result)->setName(importer->getNodeData()->name);
					tf(result);
				}
			}
			else if (type == Type::Image) {
				RZ_WARN("No image format implemented for the extension: {0}", ext);
				


			}
			else if (type == Type::Audio) {
				RZ_WARN("No Audio format implemented for the extension: {0}", ext);
			}
			else if (type == Type::Video) {
				RZ_WARN("No Video format implemented for the extension: {0}", ext);
			}
			else
				RZ_ERROR("Unrecognized file extension: {0}", ext);
		}
	}

	void AssetsManager::finished(void* result)
	{
		Mesh* mesh = static_cast<Mesh*>(result);

		if (mesh != nullptr)
			RZ_INFO("Successfully imported mesh: {0}", mesh->getName());
	}

	AssetsManager::~AssetsManager()
	{
	}





}