#include <Razor.h>
#include <GLFW/include/GLFW/glfw3.h>

using namespace Razor;

class Editor;

class TestLayer : public Razor::Layer
{
public:
	TestLayer(Razor::ScenesManager* sceneManager, Razor::Engine* engine);

	void OnUpdate(float delta);
	void OnAttach();

	static void getISSGeodesicCoords(glm::vec2& position);
	static glm::vec2 extractLatLng(const std::string& str);
	void serialize();
	void unserialize();
	void OnEvent(Razor::Event& event);
	static void importFinished(Node* node);

	static glm::vec2 iss_position;
	static std::shared_ptr<Razor::Node> node_satellite;

private:
	Razor::ScenesManager* sm;
	Razor::Engine* engine;
	std::shared_ptr<Razor::Node> earth_node;
	std::shared_ptr<Razor::Node> nodeCube;
};

glm::vec2 TestLayer::iss_position = glm::vec2(0.0f);
std::shared_ptr<Razor::Node> TestLayer::node_satellite = nullptr;

TestLayer::TestLayer(Razor::ScenesManager* sceneManager, Razor::Engine* engine) :
	Layer("Test"),
	sm(sceneManager),
	engine(engine)
{
	//std::shared_ptr<Razor::Node> nodeTerrain = std::make_shared<Razor::Node>();
	//	std::shared_ptr<Razor::LandscapeMaterial> landscapeMaterial = std::make_shared<Razor::LandscapeMaterial>();

	//	Razor::Texture* splatmap = new Razor::Texture("./data/terrain/splatmap.png", true);
	//	landscapeMaterial->setSplatmap(splatmap->getId());

	//	Razor::Texture* sand_diffuse = new Razor::Texture("./data/terrain/sand_diffuse.jpg", true);
	//	Razor::Texture* sand_specular = new Razor::Texture("./data/terrain/sand_specular.jpg", true);
	//	Razor::Texture* sand_normal = new Razor::Texture("./data/terrain/sand_normal.jpg", true);
	//	landscapeMaterial->setTextureMap(Razor::Material::TextureType::Diffuse, sand_diffuse->getId());
	//	landscapeMaterial->setTextureMap(Razor::Material::TextureType::Specular, sand_specular->getId());
	//	landscapeMaterial->setTextureMap(Razor::Material::TextureType::Normal, sand_normal->getId());

	//	Razor::Texture* dirt_diffuse = new Razor::Texture("./data/terrain/dirt_diffuse.jpg", true);
	//	Razor::Texture* dirt_specular = new Razor::Texture("./data/terrain/dirt_specular.jpg", true);
	//	Razor::Texture* dirt_normal = new Razor::Texture("./data/terrain/dirt_normal.jpg", true);
	//	landscapeMaterial->setRedChannelDiffuse(dirt_diffuse->getId());
	//	landscapeMaterial->setRedChannelSpecular(dirt_specular->getId());
	//	landscapeMaterial->setRedChannelNormal(dirt_normal->getId());

	//	Razor::Texture* grass_diffuse = new Razor::Texture("./data/terrain/grass_diffuse.jpg", true);
	//	Razor::Texture* grass_specular = new Razor::Texture("./data/terrain/grass_specular.jpg", true);
	//	Razor::Texture* grass_normal = new Razor::Texture("./data/terrain/grass_normal.jpg", true);
	//	landscapeMaterial->setGreenChannelDiffuse(grass_diffuse->getId());
	//	landscapeMaterial->setGreenChannelSpecular(grass_specular->getId());
	//	landscapeMaterial->setGreenChannelNormal(grass_normal->getId());

	//	Razor::Texture* rock_diffuse = new Razor::Texture("./data/terrain/rock_diffuse.jpg", true);
	//	Razor::Texture* rock_specular = new Razor::Texture("./data/terrain/rock_specular.jpg", true);
	//	Razor::Texture* rock_normal = new Razor::Texture("./data/terrain/rock_normal.jpg", true);
	//	landscapeMaterial->setBlueChannelDiffuse(rock_diffuse->getId());
	//	landscapeMaterial->setBlueChannelSpecular(rock_specular->getId());
	//	landscapeMaterial->setBlueChannelNormal(rock_normal->getId());

	//	float tiling = 200.0f;
	//	landscapeMaterial->setDiffuseTiling(glm::vec2(tiling, tiling));
	//	landscapeMaterial->setSpecularTiling(glm::vec2(tiling, tiling));
	//	landscapeMaterial->setNormalTiling(glm::vec2(tiling, tiling));

	//	std::shared_ptr<Razor::Landscape> landscape = std::make_shared<Razor::Landscape>("./data/terrain/heightmap.png");
	//	landscape->generate();
	//	landscape->getMesh()->setMaterial(landscapeMaterial);
	//	nodeTerrain->landscapes.push_back(landscape);

	//	nodeTerrain->name = "Terrain";
	//	//nodeTerrain->transform.setScale(glm::vec3(0.002f));
	//	nodeTerrain->meshes.push_back(landscape->getMesh());

	//	sm->getActiveScene()->getSceneGraph()->addNode(nodeTerrain);

	//std::shared_ptr<Razor::PhongMaterial> defaultMaterial = std::make_shared<Razor::PhongMaterial>();
	//defaultMaterial->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));

	//std::shared_ptr<Razor::Node> nodePlane = std::make_shared<Razor::Node>();
	//std::shared_ptr<Razor::Plane> plane = std::make_shared<Razor::Plane>();
	//plane->setMaterial(defaultMaterial);
	//nodePlane->name = "Plane";
	//nodePlane->meshes.push_back(plane);
	//plane->setPhysicsEnabled(true);

	//PlanePhysicsBody* plane_body = new PlanePhysicsBody(nodePlane.get());
	//plane->setPhysicsBody(plane_body);

	//nodeCube = std::make_shared<Node>();
	//std::shared_ptr<Cube> cube = std::make_shared<Cube>();
	//cube->setMaterial(defaultMaterial);
	//nodeCube->meshes.push_back(cube);
	//nodeCube->name = "Cube";
	//cube->setPhysicsEnabled(true);

	//CubePhysicsBody* body = new CubePhysicsBody(nodeCube.get(), glm::vec3(1.0f, 1.0f, 1.0f));
	//cube->setPhysicsBody(body);
	//engine->getPhysicsWorld()->addNode(nodeCube);
	//engine->getPhysicsWorld()->addNode(nodePlane);

	//sm->getActiveScene()->getSceneGraph()->addNode(nodeCube);
	//sm->getActiveScene()->getSceneGraph()->addNode(nodePlane);

	//std::shared_ptr<Razor::PhongMaterial> defaultMaterial = std::make_shared<Razor::PhongMaterial>();
	//std::shared_ptr<Razor::PbrMaterial> pbrMaterial = std::make_shared<Razor::PbrMaterial>();

	//nodeCube = std::make_shared<Node>();
	//std::shared_ptr<Plane> cube = std::make_shared<Plane>();
	//cube->setMaterial(pbrMaterial);
	//nodeCube->meshes.push_back(cube);
	//nodeCube->name = "Cube";
	//cube->setPhysicsEnabled(true);
	//sm->getActiveScene()->getSceneGraph()->addNode(nodeCube);

	//std::shared_ptr<Node> n = std::make_shared<Node>();
	//std::shared_ptr<Cone> c = std::make_shared<Cone>(12);
	//c->setMaterial(defaultMaterial);
	//n->meshes.push_back(c);
	//n->name = "Cone";
	//sm->getActiveScene()->getSceneGraph()->addNode(n);

}

void TestLayer::OnUpdate(float delta)
{
	if (earth_node != nullptr && node_satellite != nullptr)
	{
		// ISS Positionning	
		glm::vec3 old = node_satellite->transform.getPosition();
		float iss_altitude = 0.408f;
		glm::vec3 pos = Razor::Maths::geodesicToSpherical(iss_position, glm::vec2(90, 180), earth_node->transform.getScale().x + iss_altitude);

		float x = Razor::Maths::lerp(old.x, pos.x, delta / PI);
		float y = Razor::Maths::lerp(old.y, pos.y, delta / PI);
		float z = Razor::Maths::lerp(old.z, pos.z, delta / PI);

		node_satellite->transform.setPosition(glm::vec3(x, y, z));
		//node_satellite->transform.setPosition(pos);
	}

	if (Razor::Input::IsMouseButtonPressed(0))
	{
		/*std::shared_ptr<Scene> scene = sm->getActiveScene();

		Transform* t = new Transform();
		t->setPosition(scene->getActiveCamera()->getPosition());

		glm::vec3 rot = glm::vec3(Utils::randomf(0, TAU), Utils::randomf(0, TAU), Utils::randomf(0, TAU));
		t->setRotation(rot);

		CubePhysicsBody* body = new CubePhysicsBody(nodeCube.get(), glm::vec3(1.0f, 1.0f, 1.0f), scene->getActiveCamera()->getPosition() + scene->getActiveCamera()->g(), rot);
		body->init();

		glm::vec3 dir = scene->getActiveCamera()->getDirection() * 60.0f;
		body->getBody()->applyCentralImpulse(btVector3(dir.x, dir.y, dir.z));

		nodeCube->meshes[0]->addInstance("test", t, body);
		nodeCube->meshes[0]->setupInstances();

		engine->getPhysicsWorld()->getWorld()->addRigidBody(body->getBody());*/
	}
}

void TestLayer::OnAttach()
{
	/*	Razor::LuaScript lua_script;

		std::shared_ptr<Razor::Scene> scene = sm->getActiveScene();

		std::shared_ptr<Razor::PhongMaterial> defaultMaterial = std::make_shared<Razor::PhongMaterial>();
		defaultMaterial->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));

		std::shared_ptr<Razor::Node> nodePlane = std::make_shared<Razor::Node>();
		std::shared_ptr<Razor::Plane> plane = std::make_shared<Razor::Plane>();
		plane->setMaterial(defaultMaterial);
		nodePlane->name = "Plane";
		nodePlane->meshes.push_back(plane);

		sm->getActiveScene()->getSceneGraph()->addNode(nodePlane);*/

		/*float radius = 200.0f;
		unsigned int count = 4000000;
		float offset = 75.0f;

		for (unsigned int i = 0; i < count; i++)
		{
			Razor::Transform* t = new Razor::Transform();

			float angle = (float)i / (float)count * 360.0f;
			float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float x = sin(angle) * radius + displacement;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float y = displacement * 0.4f;
			displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
			float z = cos(angle) * radius + displacement;

			t->setPosition(glm::vec3(x, y, z));
			t->setRotation(glm::vec3(x, y, z));
			nodePlane->meshes[0]->addInstance("test", t);
		}

		nodePlane->meshes[0]->setupInstances();*/

	//test geodesic to spherical
	//earth_node = std::make_shared<Razor::Node>();
	//earth_node->transform.setScale(glm::vec3(6.370f));
	//std::shared_ptr<Razor::UVSphere> earth = std::make_shared<Razor::UVSphere>(1.0f, glm::ivec2(64, 48));
	//std::shared_ptr<Razor::PhongMaterial> mat = std::make_shared<Razor::PhongMaterial>();
	//Razor::Texture* earth_d = new Razor::Texture("./data/planets/earth/diffuse.jpg");
	//Razor::Texture* earth_s = new Razor::Texture("./data/planets/earth/specular.png");
	//mat->setTextureMap(Razor::Material::TextureType::Diffuse, earth_d->getId());
	//mat->setTextureMap(Razor::Material::TextureType::Specular, earth_s->getId());
	//earth->setMaterial(mat);
	//earth_node->meshes.push_back(earth);
	//earth_node->name = "Earth";
	//earth->updateBoundings(earth_node->transform);
	//earth->getBoundingMesh()->setMaterial(Razor::ForwardRenderer::getColorMaterial());
	//Razor::ForwardRenderer::addBoundingBox(earth_node);
	//sm->getActiveScene()->getSceneGraph()->addNode(earth_node);

	//// Satellite
	//Node* n = nullptr;
	//AssetsManager::import(n, &TestLayer::importFinished, Variant("./data/iss.fbx"));
	//std::thread threadObj(&TestLayer::getISSGeodesicCoords, std::ref(iss_position));
	//threadObj.detach();
}

void TestLayer::importFinished(Node* node)
{
	Node d(new Node(node));
	std::shared_ptr<Node> n = std::make_shared<Node>(d);
	n->setupMeshBuffers(n);

	if (n->name == "Assembled ISS")
	{
		node_satellite = n;
		node_satellite->transform.setScale(glm::vec3(0.0000005f));
	}

	Application::Get().getScenesManager()->getActiveScene()->getSceneGraph()->addNode(n);
}

void TestLayer::getISSGeodesicCoords(glm::vec2& position)
{
	double lastTime = glfwGetTime();
	double deltaTime = 0, nowTime = 0;
	double time = 0.0;
	bool thread_running = true;

	while (thread_running)
	{
		nowTime = glfwGetTime();
		double passed = (nowTime - lastTime);
		time += passed;
		lastTime = nowTime;

		if (time >= 1.0f)
		{
			std::string str = Razor::Http::get("api.open-notify.org", "/iss-now.json");
			position = extractLatLng(str);
			time = 0.0;
		}
	}
}

glm::vec2 TestLayer::extractLatLng(const std::string& str)
{
	std::string source_lat = str;
	std::string pattern_lat = "\"latitude\": \"([-]?[0-9]*\\.[0-9]{4,})";
	std::regex rgx_lat(pattern_lat, std::regex_constants::extended);
	std::smatch match_lat;
	std::vector<float> results_lat;

	while (std::regex_search(source_lat, match_lat, rgx_lat))
	{
		results_lat.push_back(std::stof(match_lat[1].str()));
		source_lat = match_lat.suffix();
	}

	std::string source_lon = str;
	std::string pattern_lon = "\"longitude\": \"([-]?[0-9]*\\.[0-9]{4,})";
	std::regex rgx_lon(pattern_lon, std::regex_constants::extended);
	std::smatch match_lon;
	std::vector<float> results_lon;

	while (std::regex_search(source_lon, match_lon, rgx_lon))
	{
		results_lon.push_back(std::stof(match_lon[1].str()));
		source_lon = match_lon.suffix();
	}

	return glm::vec2(results_lat[0], results_lon[0]);
}

void TestLayer::serialize()
{

	//auto f = engine->getThreadPool()->addTask([=]
	//{
	//	Node* j = nullptr;
	//	AssetsManager::import(j, &TestLayer::importFinished, Variant("./data/Jeep.fbx"));
	//	return j;
	//});

	//Node* n = f.get();
	//Log::trace("%s", n->name);

	//Node* j = nullptr;
	//	AssetsManager::import(j, &TestLayer::importFinished, Variant("./data/Jeep.fbx"));

	////////////////////

	//HuffmanEncoding huff;

	//std::string test = "COMME_CHARMANT_E";

	//std::string encoded = huff.encode(test);
	//Log::trace("ENCODED: %s", encoded.c_str());

	//std::string decoded = huff.decode(encoded);
	//Log::trace("DECODED: %s", decoded.c_str());

	///////////////////////
	//System* system = engine->getSystem();
	//system->getStats();

	//System::VirtualMemory& vm = system->getVirtualMemory();
	//System::PhysicalMemory& pm = system->getPhysicalMemory();

	//Log::trace("Virtual memory total          : %s", Utils::bytesToSize(vm.total).c_str());
	//Log::trace("Virtual memory system usage   : %s", Utils::bytesToSize(vm.system_usage).c_str());
	//Log::trace("Virtual memory process usage  : %s", Utils::bytesToSize(vm.process_usage).c_str());
	//									
	//Log::trace("Physical memory total         : %s", Utils::bytesToSize(pm.total).c_str());
	//Log::trace("Physical memory system usage  : %s", Utils::bytesToSize(pm.system_usage).c_str());
	//Log::trace("Physical memory process usage : %s", Utils::bytesToSize(pm.process_usage).c_str());
}

void TestLayer::unserialize()
{

}

void TestLayer::OnEvent(Razor::Event& event)
{
	if (event.GetEventType() == Razor::EventType::KeyPressed)
	{
		Razor::KeyPressedEvent& e = (Razor::KeyPressedEvent&)event;

		if (e.GetKeyCode() == RZ_KEY_J)
			serialize();
		else if (e.GetKeyCode() == RZ_KEY_K)
			unserialize();

		//Razor::Viewport* vp = (Razor::Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];
	}

	if (event.GetEventType() == Razor::EventType::MouseButtonPressed)
	{
		Razor::MouseButtonEvent& e = (Razor::MouseButtonEvent&)event;

		if (e.GetMouseButton() == RZ_MOUSE_BUTTON_1)
		{
			/*std::shared_ptr<Scene> scene = sm->getActiveScene();

			Transform* t = new Transform();
			t->setPosition(scene->getActiveCamera()->getPosition());

			glm::vec3 rot = glm::vec3(Utils::randomf(0, TAU), Utils::randomf(0, TAU), Utils::randomf(0, TAU));
			t->setRotation(rot);


			CubePhysicsBody* body = new CubePhysicsBody(nodeCube.get(), glm::vec3(1.0f, 1.0f, 1.0f), scene->getActiveCamera()->getPosition() + scene->getActiveCamera()->getDirection(), rot);
			body->init();

			glm::vec3 dir = scene->getActiveCamera()->getDirection() * 60.0f;
			body->getBody()->applyCentralImpulse(btVector3(dir.x, dir.y, dir.z));

			nodeCube->meshes[0]->addInstance("test", t, body);
			nodeCube->meshes[0]->setupInstances();

			engine->getPhysicsWorld()->getWorld()->addRigidBody(body->getBody());*/
		}
	}
}


class Sandbox : public Razor::Application
{		
public:
	Sandbox() : Application(false, Application::Type::EDITOR)
	{
		Razor::ScenesManager* sm = this->getScenesManager();
		Razor::Engine* engine = this->getEngine();
		PushLayer(new TestLayer(sm, engine));
	}

	~Sandbox()
	{

	}
};

Razor::Application* Razor::createApplication()
{
	return new Sandbox();
}
