#include "rzpch.h"
#include "imgui.h"
#include "AssetsManager.h"
#include "Logger.h"
#include "Editor/Editor.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Materials/Texture.h"

namespace Razor 
{

	AssetsManager::ExtsMap AssetsManager::exts = {
		{AssetsManager::Type::Model, { "3ds",  "blend", "dae",  "fbx",  "gltf", "obj", "raw",  "stl"}},
		{AssetsManager::Type::Image, { "jpg",  "png",   "dds",  "png",  "psd",  "gif", "exif", "tiff", "hdr", "svg", "tga"}},
		{AssetsManager::Type::Audio, { "wav"/*,"ogg", "aac",   "aiff", "flac", "mp3" */}},
		{AssetsManager::Type::Video, { "webm", "flv",   "ogv",  "mov",  "mp4",  "3gp"}}
	};

	TexturesManager* AssetsManager::texturesManager = nullptr;
	std::unique_ptr<AssimpImporter> AssetsManager::importer = nullptr;

	AssetsManager::AssetsManager(Editor* editor) : EditorComponent(editor)
	{
		texturesManager = new TexturesManager();
		importer = std::make_unique<AssimpImporter>();

		this->directoryWatcher = std::make_shared<DirectoryWatcher>("F:/Razor/Razor/Sandbox", std::chrono::milliseconds(1000));
		this->fileWatcher = std::make_shared<FileWatcher>();
		this->fileBrowser = std::make_shared<FileBrowser>();

		//watch();
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

		ImGui::End();
	}

	void AssetsManager::import(std::shared_ptr<Node> result, TaskFinished tf, Variant opts)
	{
		Log::trace("Task Arguments: %s", opts.toString().c_str());

		if (opts.type() == Variant::Type::String)
		{
			std::string str = opts.toString();
			auto ext = str.substr(str.find_last_of(".") + 1);
			auto type = AssetsManager::getExt(ext);
			
			switch (type) 
			{
			case Type::Model:
				if (importer->importMesh(opts.toString())) 
				{
					result = importer->getNodeData();
					tf(result);
					importer->resetRootNode();
				}
				break;
			case Type::Image:
				Log::warn("No image format implemented for the extension: %s", ext.c_str());
				break;
			case Type::Audio:
				Log::warn("No Audio format implemented for the extension: %s", ext.c_str());
				break;
			case Type::Video:
				Log::warn("No Video format implemented for the extension: %s", ext.c_str());
				break;
			default:
				Log::error("Unrecognized file extension: %s", ext.c_str());
			}
		}
	}



	AssetsManager::~AssetsManager()
	{
	}





}