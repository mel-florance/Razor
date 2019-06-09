#include "rzpch.h"

#include "ForwardRenderer.h"
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/matrix_decompose.hpp>
#include <glm/gtc/quaternion.hpp>

#include "Razor/Core/Engine.h"
#include "Razor/Physics/World.h"
#include "Razor/Physics/PlanePhysicsBody.h"
#include "Razor/Physics/SpherePhysicsBody.h"
#include "Razor/Physics/CubePhysicsBody.h"
#include "Razor/Scene/SceneGraph.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Core/Window.h"
#include "Razor/Application/Application.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Razor/Landscape/Landscape.h"
#include "Razor/Maths/Raycast.h"
#include "Razor/Materials/Presets/PhongMaterial.h"
#include "Razor/Materials/Presets/ColorMaterial.h"
#include "Razor/Materials/Presets/LandscapeMaterial.h"

#include "Editor/Components/AssetsManager.h"
#include "Razor/Materials/TexturesManager.h"
#include "Razor/Materials/ShadersManager.h"
#include "Razor/Input/MouseButtons.h"

namespace Razor 
{

	int ForwardRenderer::num_directional_lights = 1;
	int ForwardRenderer::num_point_lights = 1;
	int ForwardRenderer::num_spot_lights = 1;
	Shader* ForwardRenderer::defaultShader = nullptr;
	Shader* ForwardRenderer::landscapeShader = nullptr;

	ForwardRenderer::ForwardRenderer(Window* window, Engine* engine, ScenesManager* scenesManager, ShadersManager* shadersManager) :
		window(window),
		engine(engine),
		scenesManager(scenesManager),
		shadersManager(shadersManager)
	{
		std::shared_ptr<Node> nodePointLight = std::make_shared<Node>();
		nodePointLight->name = "Point Light";
		pointLight = std::make_shared<Point>();
		pointLight->setPosition(glm::vec3(-4.885f, 0.783f, -1.615f));
		pointLight->setDiffuse(glm::vec3(1.0f, 0.6f, 0.0f));
		nodePointLight->lights.push_back(pointLight);

		std::shared_ptr<Node> nodePointLight2 = std::make_shared<Node>();
		nodePointLight2->name = "Point Light 2";
		std::shared_ptr<Point> pointLight2 = std::make_shared<Point>();
		pointLight2->setPosition(glm::vec3(3.973f, 0.546f, -0.130f));
		pointLight2->setDiffuse(glm::vec3(1.0f, 0.6f, 0.0f));
		nodePointLight2->lights.push_back(pointLight2);

		std::shared_ptr<Node> nodeDirectional = std::make_shared<Node>();
		nodeDirectional->name = "Directional Light";
		directionalLight = std::make_shared<Directional>();
		directionalLight->setDirection(glm::vec3(8.0f, 20.0F, 12.0f));
		directionalLight->setDiffuse(glm::vec3(1.05f, 1.05f, 1.05f));
		nodeDirectional->lights.push_back(directionalLight);
		
		std::shared_ptr<Node> nodeSpot = std::make_shared<Node>();
		nodeSpot->name = "Spot Light";
		std::shared_ptr<Spot> spotLight = std::make_shared<Spot>();
		spotLight->setPosition(glm::vec3(0.0f, 5.0f, 0.0f));
		spotLight->setDirection(glm::vec3(0.0f, -1.0f, 0.0f));
		nodeSpot->lights.push_back(spotLight);

		angle = 0.0f;
		deltaTime = 0.0f;

		clear_color = { 0.18f, 0.27f, 0.47f, 1.0f };
		framebuffer_size = { 1405, 814 };
	
		setupShaders();

		//Texture* checkerMap = new Texture("./data/FreshTilledDirt_albedo.png", true);
		//Texture* checkerSpec = new Texture("./data/FreshTilledDirt_specular.png", true);
		//Texture* checkerNormal = new Texture("./data/FreshTilledDirt_normal.png", true);

		defaultMaterial = std::make_shared<PhongMaterial>();
		//defaultMaterial->setTextureMap(Material::TextureType::Diffuse, checkerMap->getId());
		//defaultMaterial->setTextureMap(Material::TextureType::Specular, checkerMap->getId());
		defaultMaterial->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));

		std::shared_ptr<PhongMaterial> checker_material = std::make_shared<PhongMaterial>();
		//float tiling = 200.0f;
		//checker_material->setDiffuseTiling(glm::vec2(tiling, tiling));
		//checker_material->setSpecularTiling(glm::vec2(tiling, tiling));
		//checker_material->setNormalTiling(glm::vec2(tiling, tiling));
		// 
		//checker_material->setTextureMap(Material::TextureType::Diffuse, checkerMap->getId());
		//checker_material->setTextureMap(Material::TextureType::Specular, checkerSpec->getId());
		//checker_material->setTextureMap(Material::TextureType::Normal, checkerNormal->getId());
		//checker_material->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));
		//checker_material->setAmbientColor(glm::vec3(0.0f, 0.0f, 0.0f));

		viewportShader = shadersManager->createShader("viewport");
		viewportShader->compile();
		viewportShader->link();
		viewportShader->bind();
		viewportShader->setUniform1i("target", 0);

		quad = new Quad();

		framebuffer = new FrameBuffer();
		colorbuffer = framebuffer->addTextureAttachment(framebuffer_size);
		framebuffer->addRenderBufferAttachment(framebuffer_size);

		depthbuffer = new FrameBuffer();
		depth_attachment = depthbuffer->addTextureAttachment(glm::vec2(2048, 2048), true);

		nodeCube = std::make_shared<Node>();
		nodePlane = std::make_shared<Node>();
		nodeSphere = std::make_shared<Node>();
		std::shared_ptr<Node> nodeTerrain = std::make_shared<Node>();

		std::shared_ptr<Cube> cube = std::make_shared<Cube>();
		cube->setMaterial(defaultMaterial);
		cube->setPhysicsBody(new CubePhysicsBody(nodeCube.get()));

		std::shared_ptr<Plane> plane = std::make_shared<Plane>();
		plane->setMaterial(defaultMaterial);
		plane->setPhysicsBody(new PlanePhysicsBody(nodePlane.get(), glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, nodePlane->transform.getPosition()));

		std::shared_ptr<UVSphere> sphere = std::make_shared<UVSphere>();
		sphere->setMaterial(defaultMaterial);
		sphere->setPhysicsBody(new SpherePhysicsBody(nodeSphere.get(), 1.0f, nodeSphere->transform.getPosition()));

		//landscapeMaterial = std::make_shared<LandscapeMaterial>();

		//Texture* splatmap = new Texture("./data/terrain/splatmap.png", true);
		//landscapeMaterial->setSplatmap(splatmap->getId());

		//Texture* sand_diffuse = new Texture("./data/terrain/sand_diffuse.jpg", true);
		//Texture* sand_specular = new Texture("./data/terrain/sand_specular.jpg", true);
		//Texture* sand_normal = new Texture("./data/terrain/sand_normal.jpg", true);
		//landscapeMaterial->setTextureMap(Material::TextureType::Diffuse, sand_diffuse->getId());
		//landscapeMaterial->setTextureMap(Material::TextureType::Specular, sand_specular->getId());
		//landscapeMaterial->setTextureMap(Material::TextureType::Normal, sand_normal->getId());

		//Texture* dirt_diffuse = new Texture("./data/terrain/dirt_diffuse.jpg", true);
		//Texture* dirt_specular = new Texture("./data/terrain/dirt_specular.jpg", true);
		//Texture* dirt_normal = new Texture("./data/terrain/dirt_normal.jpg", true);
		//landscapeMaterial->setRedChannelDiffuse(dirt_diffuse->getId());
		//landscapeMaterial->setRedChannelSpecular(dirt_specular->getId());
		//landscapeMaterial->setRedChannelNormal(dirt_normal->getId());

		//Texture* grass_diffuse = new Texture("./data/terrain/grass_diffuse.jpg", true);
		//Texture* grass_specular = new Texture("./data/terrain/grass_specular.jpg", true);
		//Texture* grass_normal = new Texture("./data/terrain/grass_normal.jpg", true);
		//landscapeMaterial->setGreenChannelDiffuse(grass_diffuse->getId());
		//landscapeMaterial->setGreenChannelSpecular(grass_specular->getId());
		//landscapeMaterial->setGreenChannelNormal(grass_normal->getId());

		//Texture* rock_diffuse = new Texture("./data/terrain/rock_diffuse.jpg", true);
		//Texture* rock_specular = new Texture("./data/terrain/rock_specular.jpg", true);
		//Texture* rock_normal = new Texture("./data/terrain/rock_normal.jpg", true);
		//landscapeMaterial->setBlueChannelDiffuse(rock_diffuse->getId());
		//landscapeMaterial->setBlueChannelSpecular(rock_specular->getId());
		//landscapeMaterial->setBlueChannelNormal(rock_normal->getId());

		//float tiling = 200.0f;
		//landscapeMaterial->setDiffuseTiling(glm::vec2(tiling, tiling));
		//landscapeMaterial->setSpecularTiling(glm::vec2(tiling, tiling));
		//landscapeMaterial->setNormalTiling(glm::vec2(tiling, tiling));

		//Landscape* landscape = new Landscape("./data/terrain/heightmap.png");
		//landscape->generate();
		//landscape->getMesh()->setMaterial(landscapeMaterial);

		//nodeTerrain->name = "Terrain";
		////nodeTerrain->transform.setScale(glm::vec3(0.002f));
		//nodeTerrain->meshes.push_back(landscape->getMesh());

		nodeCube->name = "Cube";
		nodePlane->name = "Plane";
		nodeCube->transform.setPosition(glm::vec3(0.0f, 150.0f, 0.0f));
		nodeCube->meshes.push_back(cube);

		nodePlane->meshes.push_back(plane);
		nodeSphere->meshes.push_back(sphere);
		nodeSphere->name = "Sphere";
		//nodeSphere->transform.setScale(glm::vec3(0.2f, 0.2f, 0.2f));
		nodeSphere->transform.setPosition(glm::vec3(0.0f, 1.0f, 0.0f));
		nodePlane->transform.setScale(glm::vec3(100.0f, 1.0f, 100.0f));

		//raycast = new Raycast(window, tps_camera, tps_camera->getProjectionMatrix());

		std::shared_ptr<Scene> scene = std::make_shared<Scene>("default_scene");
		scenesManager->setActiveScene(scene);

		scenesManager->getActiveScene()->addLight(directionalLight, Light::Type::DIRECTIONAL);
		//scenesManager->getActiveScene()->addLight(pointLight, Light::Type::POINT);
		//scenesManager->getActiveScene()->addLight(pointLight2, Light::Type::POINT);
		//scenesManager->getActiveScene()->addLight(spotLight, Light::Type::SPOT);

		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeCube);
		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodePlane);
		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeSphere);

		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeTerrain);
		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeDirectional);
		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodePointLight);
		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodePointLight2);
		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeSpot);
		
		// Shadows
		float near_plane = 0.0f, far_plane = 20.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		glShadeModel(GL_FLAT);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		glEnable(GL_MULTISAMPLE);
		/*glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/

		engine->getPhysicsWorld()->addNode(nodePlane);
		engine->getPhysicsWorld()->addNode(nodeCube);
		engine->getPhysicsWorld()->addNode(nodeSphere);

		//// For instancing later

		//int count = 1000;
		//float radius = 150.0;
		//float offset = 25.0f;

		//AssimpImporter* importer = new AssimpImporter();
		//importer->importMesh("./data/SM_Pine01_lod2.obj");

		//std::shared_ptr<Node> node = std::make_shared<Node>(importer->getNodeData());

		//Editor::setupMeshBuffers(node);

		//pine_tree = node->meshes[0];
		//pine_tree->setMaterial(defaultMaterial);
		//scenesManager->getActiveScene()->getSceneGraph()->addNode(node);


		//for (int i = 0; i < count; i++)
		//{
		//	float angle = (float)i / (float)count * 360.0f;
		//	float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		//	float x = sin(angle) * radius + displacement;
		//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		//	float y = displacement * 0.4f;
		//	displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		//	float z = cos(angle) * radius + displacement;

		//	Transform* cube_transform = new Transform();
		//	cube_transform->setScale(glm::vec3(0.02f));
		//	cube_transform->setPosition(glm::vec3(x, y, z));

		//	modelMatrices.push_back(cube_transform->getMatrix());
		//}

		//unsigned int buffer;
		//glGenBuffers(1, &buffer);
		//glBindBuffer(GL_ARRAY_BUFFER, buffer);
		//glBufferData(GL_ARRAY_BUFFER, count * sizeof(glm::mat4), reinterpret_cast<GLfloat*>(&modelMatrices[0]), GL_STATIC_DRAW);

		//pine_tree->getVao()->bind();
	
		//glEnableVertexAttribArray(4);
		//glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		//glEnableVertexAttribArray(5);
		//glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		//glEnableVertexAttribArray(6);
		//glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		//glEnableVertexAttribArray(7);
		//glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

		//glVertexAttribDivisor(4, 1);
		//glVertexAttribDivisor(5, 1);
		//glVertexAttribDivisor(6, 1);
		//glVertexAttribDivisor(7, 1);

		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//pine_tree->getVao()->unbind();
	}

	ForwardRenderer::~ForwardRenderer()
	{
		
	}
	
	void ForwardRenderer::update(float delta)
	{
		deltaTime = (float)delta;
		angle += deltaTime;
		engine->getPhysicsWorld()->tick(delta);
		engine->getPhysicsWorld()->updateNodes();
		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];

		if (Input::IsMouseButtonPressed(RZ_MOUSE_BUTTON_3) && vp->isHovered())
		{
			std::shared_ptr<Scene> scene = scenesManager->getActiveScene();

			std::shared_ptr<Node> node = std::make_shared<Node>();
			glm::vec3 rot = glm::vec3(Utils::randomf(0, TAU), Utils::randomf(0, TAU), Utils::randomf(0, TAU));
			node->transform.setRotation(rot);

			std::shared_ptr<StaticMesh> object;
			if (rand() % 2)
				object = std::make_shared<Cube>();
			else 
				object = std::make_shared<UVSphere>();

			object->setPhysicsEnabled(true);

			CubePhysicsBody* body = new CubePhysicsBody(node.get(), glm::vec3(1.0f, 1.0f, 1.0f), scene->getActiveCamera()->getPosition(), rot);
			body->init();
			glm::vec3 dir = scene->getActiveCamera()->getDirection() * 2000.0f;
			btVector3 direction = btVector3(btScalar(dir.x), btScalar(dir.y), btScalar(dir.z));
			body->getBody()->setLinearVelocity(direction);

			object->setPhysicsBody(body);
			node->meshes.push_back(object);
			engine->getPhysicsWorld()->addNode(node);
			scene->getSceneGraph()->addNode(node);
		}
	}

	void ForwardRenderer::render()
	{
		Viewport* vp = (Viewport*)Application::Get().getEditor()->getComponents()["Viewport"];
		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();

		//Tools* tools = (Tools*)Application::Get().getEditor()->getComponents()["Tools"];

		//if (vp != nullptr && tools != nullptr) {

		//	auto tools_width = tools->getSize().x;
		//	raycast->setViewport(glm::vec2(vp->getSize().x, vp->getSize().y));
		//}

		//raycast->computeRay();
		//
		//std::cout << glm::to_string(raycast->computeRay()) << std::endl;


		enableDepthTest();
		glCullFace(GL_FRONT);

		depthShader->bind();
		depthShader->setUniformMat4f("model", glm::mat4(1.0f));

		//for (auto light : scene->getLights())
		//{
		//	ShadowGenerator& generator = light->getShadowGenerator();
		//	glm::mat4 light_matrix = generator.getProjectionMatrix() * generator.getViewMatrix();

		//	depthShader->setUniformMat4f("lightSpaceMatrix", light_matrix);

		//	setViewport(0, 0, generator.getSize().x, generator.getSize().y);

		//	generator.getDepthBuffer()->bind();

		//	glClear(GL_DEPTH_BUFFER_BIT);

		//	for (auto node : scene->getSceneGraph()->getNodes())
		//		renderNode(depthShader, node, glm::mat4(1.0f), true);

		//	generator.getDepthBuffer()->unbind();
		//}

		lightView = glm::lookAt(directionalLight->getDirection(), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
		//directionalLight->setDirection(glm::vec3(0.0f, std::cos(angle) * -4.0f,std::sin(angle) * 4.0f));

		// Depth pass

		depthShader->setUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

		setViewport(0, 0, 2048, 2048);

		depthbuffer->bind();

		glClear(GL_DEPTH_BUFFER_BIT);

		for (auto node : scene->getSceneGraph()->getNodes())
			renderNode(depthShader, node, glm::mat4(1.0f), true);

		depthbuffer->unbind();

		glCullFace(GL_BACK);

		// Render pass
		setViewport(0, 0, framebuffer_size.x, framebuffer_size.y);
		framebuffer->bind();

		clear();

		for (auto& line_mesh : line_meshes)
			renderLineMesh(line_mesh);

		if (scene->getSceneGraph()->getNodes().size() > 0)
		{
			//glm::vec3 pos = glm::vec3(8*std::cos(angle), 8, 8*std::sin(angle));
			//directionalLight->setDirection(pos);

		
			for (auto node : scene->getSceneGraph()->getNodes())
			{
				if (node->name == "Terrain")
				{
					landscapeShader->bind();
					landscapeMaterial->bindLights(landscapeShader, scene->getLights());
					landscapeMaterial->bind(landscapeShader);
				
					glActiveTexture(GL_TEXTURE3);
					depth_attachment->bind();

					landscapeShader->setUniform3f("lightPos", directionalLight->getDirection());
					landscapeShader->setUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
					landscapeShader->setUniform3f("viewPosition", scene->getActiveCamera()->getPosition());
					landscapeShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
					landscapeShader->setUniformMat4f("projection", scene->getActiveCamera()->getProjectionMatrix());
			
					renderNode(landscapeShader, node, glm::mat4(1.0f));

					depth_attachment->unbind();
					landscapeShader->unbind();
				}
				else {
					defaultShader->bind();
					defaultMaterial->bindLights(defaultShader, scene->getLights());
					defaultMaterial->bind(defaultShader);

					/*defaultShader->setUniform1i("instanced", 1);
					pine_tree->getVao()->bind();
					glDrawElementsInstanced(GL_TRIANGLES, (unsigned int)pine_tree->getIndices().size(), GL_UNSIGNED_INT, 0, 1000);
					pine_tree->getVao()->unbind();
					defaultShader->setUniform1i("instanced", 0);*/

					glActiveTexture(GL_TEXTURE3);
					depth_attachment->bind();

					defaultShader->setUniform3f("lightPos", directionalLight->getDirection());
					defaultShader->setUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
					defaultShader->setUniform3f("viewPosition", scene->getActiveCamera()->getPosition());
					defaultShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
					defaultShader->setUniformMat4f("projection", scene->getActiveCamera()->getProjectionMatrix());

					renderNode(defaultShader, node, glm::mat4(1.0f));

					depth_attachment->unbind();
					defaultShader->unbind();
				}
			}
			
			//colorbuffer = depth_attachment;
		}

		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);

		framebuffer->unbind();
		disableDepthTest();
	}

	void ForwardRenderer::renderNode(Shader* shader, std::shared_ptr<Node> node, glm::mat4 parent, bool depth)
	{
		glm::mat4 local = parent * node->transform.getMatrix();
	
		for (auto mesh : node->meshes)
		{
			shader->setUniformMat4f("model", local);

			if (mesh->getMaterial() != nullptr && !depth)
			{
				mesh->getMaterial()->bindLights(shader, scenesManager->getActiveScene()->getLights());
				mesh->getMaterial()->bind(shader);
			}

			mesh->getVao()->bind();
			mesh->draw();

			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		for (auto child : node->nodes)
			renderNode(shader, child, local);
	}

	void ForwardRenderer::renderParticleSystems()
	{
	}

	void ForwardRenderer::renderLineMesh(std::shared_ptr<Node> node)
	{
		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();

		gridShader->bind();

		gridShader->setUniformMat4f("model", node->transform.getMatrix());
		gridShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
		gridShader->setUniformMat4f("proj", scene->getActiveCamera()->getProjectionMatrix());

		for (auto& mesh : node->meshes)
		{
			mesh->getMaterial()->bind(gridShader);

			mesh->getVao()->bind();
			mesh->draw();
		}

		gridShader->unbind();
	}

	void ForwardRenderer::clear(ClearType type)
	{
		int flags;
		glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
		
		switch (type) 
		{
			default:
			case ClearType::ALL:
				flags = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT;
				break;
			case ClearType::COLOR:
				flags = GL_COLOR_BUFFER_BIT;
				break;
			case ClearType::DEPTH:
				flags = GL_DEPTH_BUFFER_BIT;
				break;
			case ClearType::STENCIL:
				flags = GL_STENCIL_BUFFER_BIT;
				break;
		}

		glClear(flags);
	}

	void ForwardRenderer::enableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void ForwardRenderer::disableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void ForwardRenderer::setViewport(unsigned int x, unsigned int y, float w, float h)
	{
		glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
	}

	void ForwardRenderer::setupShaders()
	{
		particleShader = shadersManager->createShader("particle");
		particleShader->compile();
		particleShader->link();

		gridShader = shadersManager->createShader("grid");
		gridShader->compile();
		gridShader->link();

		depthShader = shadersManager->createShader("depth");
		depthShader->compile();
		depthShader->link();

		landscapeShader = shadersManager->createShader("landscape");
		landscapeShader->defineConstant(Shader::Type::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", 2);
		landscapeShader->defineConstant(Shader::Type::FRAGMENT, "MAX_POINT_LIGHTS", 1);
		landscapeShader->defineConstant(Shader::Type::FRAGMENT, "MAX_SPOT_LIGHTS", 1);
		landscapeShader->replaceConstants();
		landscapeShader->compile();
		landscapeShader->link();

		defaultShader = shadersManager->createShader("default");
		defaultShader->defineConstant(Shader::Type::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", 2);
		defaultShader->defineConstant(Shader::Type::FRAGMENT, "MAX_POINT_LIGHTS", 1);
		defaultShader->defineConstant(Shader::Type::FRAGMENT, "MAX_SPOT_LIGHTS", 1);
		defaultShader->replaceConstants();
		defaultShader->compile();
		defaultShader->link();
	}

	TextureAttachment* ForwardRenderer::getColorBuffer()
	{
		return colorbuffer;
	}

	TextureAttachment* ForwardRenderer::getDepthBuffer()
	{
		return depth_attachment;
	}

}
