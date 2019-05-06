#include "rzpch.h"

#include "DeferredRenderer.h"
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
#include "Razor/Scene/SceneGraph.h"
#include "Razor/Cameras/FPSCamera.h"
#include "Razor/Cameras/TPSCamera.h"
#include "Razor/Buffers/Buffers.h"
#include "Razor/Geometry/Geometry.h"
#include "Razor/Core/Window.h"
#include "Razor/Application/Application.h"
#include "Razor/Scene/ScenesManager.h"
#include "Editor/Editor.h"
#include "Razor/Terrains/Terrain.h"
#include "Razor/Maths/Raycast.h"
#include "Razor/Materials/Presets/PhongMaterial.h"
#include "Razor/Materials/Presets/ColorMaterial.h"

namespace Razor {

	DeferredRenderer::DeferredRenderer(Window* window, Engine* engine, ScenesManager* scenesManager) :
		engine(engine),
		scenesManager(scenesManager)
	{
		shadersManager = new ShadersManager();

		diffuseMap = new Texture("./data/textures/basketball.jpg", true);
		specularMap = new Texture("./data/textures/basketball.jpg", true);
		checkerMap = new Texture("./data/checker.png", true);

		pointLight = new Point();
		pointLight->setPosition(glm::vec3(5.31f, 0.06f, 0.33f));
		pointLight->setDiffuse(glm::vec3(1.0f, 0.6f, 0.0f));
		Point* pointLight2 = new Point();
		pointLight2->setPosition(glm::vec3(-6.13f, 0.20f, -0.15f));
		pointLight2->setDiffuse(glm::vec3(1.0f, 0.6f, 0.0f));

		directionalLight = new Directional();
		directionalLight->setDirection(glm::vec3(8.0f, 20.5f, 3.0f));

		Spot* spotLight = new Spot();
		spotLight->setPosition(glm::vec3(1.0f, 8.0f, 2.0f));
		spotLight->setDirection(glm::vec3(0.0f, 1.0f, 0.0f));

		angle = 0.0f;
		deltaTime = 0.0f;

		color = { 0.3f, 0.3f, 0.3f, 0.3f };
		framebuffer_size = { 1280, 720 };
	
		particleShader = shadersManager->createShader("particle");
		particleShader->compile();
		particleShader->link();

		gridShader = shadersManager->createShader("grid");
		gridShader->compile();
		gridShader->link();

		depthShader = shadersManager->createShader("depth");
		depthShader->compile();
		depthShader->link();

		//debugDepthShader = shadersManager->createShader("debug_depth");
		//debugDepthShader->compile();
		//debugDepthShader->link();

		defaultShader = shadersManager->createShader("default");
		defaultShader->defineConstant(Shader::Type::FRAGMENT, "MAX_DIRECTIONAL_LIGHTS", 2);
		defaultShader->defineConstant(Shader::Type::FRAGMENT, "MAX_POINT_LIGHTS", 3);
		defaultShader->defineConstant(Shader::Type::FRAGMENT, "MAX_SPOT_LIGHTS", 1);
		defaultShader->replaceConstants();
		defaultShader->compile();
		defaultShader->link();

		defaultMaterial = new PhongMaterial();
		defaultMaterial->setTextureMap(Material::TextureType::Diffuse, diffuseMap->getId());
		defaultMaterial->setTextureMap(Material::TextureType::Specular, specularMap->getId());
		defaultMaterial->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));

		PhongMaterial* checker_material = new PhongMaterial();
		checker_material->setTextureMap(Material::TextureType::Diffuse, checkerMap->getId());
		checker_material->setSpecularColor(glm::vec3(0.0f, 0.0f, 0.0f));

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
		depth_attachment = depthbuffer->addTextureAttachment(glm::vec2(2048.0f, 2048.0f), true);

		node_axis = new Node();
		axis_x = new Ray(glm::vec3(8.0f, 0.0f, 0.0f), glm::vec3(-8.0f, 0.0f, 0.0f), 2.0f);
		axis_y = new Ray(glm::vec3(0.0f, 8.0f, 0.0f), glm::vec3(0.0f, -8.0f, 0.0f), 2.0f);
		axis_z = new Ray(glm::vec3(0.0f, 0.0f, 8.0f), glm::vec3(0.0f, 0.0f, -8.0f), 2.0f);

		axis_x->setLineWidth(0.5f);
		axis_y->setLineWidth(0.5f);
		axis_z->setLineWidth(0.5f);

		node_axis->meshes.push_back(axis_x);
		node_axis->meshes.push_back(axis_y);
		node_axis->meshes.push_back(axis_z);

		Node* nodeCube = new Node();
		nodePlane = new Node();
		nodeSphere = new Node();
		Node* nodeTerrain = new Node();

		Cube* cube = new Cube();
		cube->setMaterial(defaultMaterial);

		Plane* plane = new Plane();
		plane->setMaterial(checker_material);

		UVSphere* sphere = new UVSphere();
		sphere->setMaterial(defaultMaterial);

		Terrain* terrain = new Terrain(glm::vec3());
		terrain->generate();
		terrain->getMesh()->setMaterial(defaultMaterial);

		nodeTerrain->name = "Terrain";
		nodeTerrain->meshes.push_back(terrain->getMesh());

		nodeCube->name = "Cube";
		nodePlane->name = "Plane";

		nodeCube->meshes.push_back(cube);
		nodePlane->meshes.push_back(plane);
		nodeSphere->meshes.push_back(sphere);
		nodeSphere->name = "Sphere";
		nodeSphere->transform.setScale(glm::vec3(0.2f, 0.2f, 0.2f));
		nodeSphere->transform.setPosition(glm::vec3(0.0f, 8.0f, 0.0f));

		nodePlane->transform.setScale(glm::vec3(100.0f, 1.0f, 100.0f));
	

		//raycast = new Raycast(window, tps_camera, tps_camera->getProjectionMatrix());

		std::shared_ptr<Scene> scene = std::make_shared<Scene>("default_scene");
		scenesManager->setActiveScene(scene);

		grid_node = new Node();
		grid = new Grid(8.0f);
		ColorMaterial* grid_mat = new ColorMaterial();
		grid_mat->setColor(glm::vec3(0.4f, 0.4f, 0.4f));
		grid->setMaterial(grid_mat);
		grid_node->meshes.push_back(grid);

		scenesManager->getActiveScene()->addLight(directionalLight);
		scenesManager->getActiveScene()->addLight(pointLight);
		scenesManager->getActiveScene()->addLight(pointLight2);
		//scenesManager->getActiveScene()->addLight(spotLight);

		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeCube);
		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodePlane);
		scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeSphere);
		//scenesManager->getActiveScene()->getSceneGraph()->addNode(nodeTerrain);

		// Shadows
		float near_plane = 1.0f, far_plane = 100.0f;
		lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane);

		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LESS);
		/*glEnable(GL_STENCIL_TEST);
		glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);*/

		plane_body = new PlanePhysicsBody(glm::vec3(0.0f, 1.0f, 0.0f), 0.0f, nodePlane->transform.getPosition());


		sphere_body = new SpherePhysicsBody(1.0f, nodeSphere->transform.getPosition());
		sphere_body->mass = 2.0f;

		engine->getPhysicsWorld()->addBody(plane_body);
		engine->getPhysicsWorld()->addBody(sphere_body);
	}

	DeferredRenderer::~DeferredRenderer()
	{
		
	}
	
	void DeferredRenderer::update(float delta)
	{
		deltaTime = (float)delta;
		angle += deltaTime;
		engine->getPhysicsWorld()->tick(delta);

		/*PhysicsBody* sphere = engine->getPhysicsWorld()->getBodies()[1];


		if (Input::IsKeyPressed(RZ_KEY_V))
		{

			sphere->getBody()->setLinearVelocity(btVector3(Utils::randomf(-4.0f, 4.0f), 10.0f, Utils::randomf(-4.0f, 4.0f)));
			sphere->getBody()->setAngularVelocity(btVector3(Utils::randomf(-4.0f, 4.0f), 0.0f, Utils::randomf(-4.0f, 4.0f)));
			sphere->getBody()->clearForces();
		}

		btTransform t;
		sphere->getBody()->getMotionState()->getWorldTransform(t);

		btVector3 origin = t.getOrigin();
		nodeSphere->transform.setPosition(glm::vec3(origin.getX(), origin.getY(), origin.getZ()));

		float mat[16];
		 t.getOpenGLMatrix(mat);

		glm::mat4 m = glm::make_mat4(mat);
		glm::vec3 scale;
		glm::quat rotation;
		glm::vec3 translation;
		glm::vec3 skew;
		glm::vec4 perspective;
		glm::decompose(m, scale, rotation, translation, skew, perspective);
		glm::vec3 euler = glm::eulerAngles(rotation);

		nodeSphere->transform.setRotation(euler);
		nodeSphere->transform.setScale(scale);*/
	}

	void DeferredRenderer::render()
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

		//Todo: Add all others flags at the begining of this functio and also at the end.

		// Make backface culling available per mesh.

		lightView = glm::lookAt(directionalLight->getDirection(), glm::vec3(0.0f), glm::vec3(0.0f, -1.0f, 0.0f));
		lightSpaceMatrix = lightProjection * lightView;
		//directionalLight->setDirection(glm::vec3(0.0f, std::cos(angle) * -4.0f,std::sin(angle) * 4.0f));

		// Depth pass

		enableDepthTest();
		glCullFace(GL_FRONT);

		depthShader->bind();
		depthShader->setUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);
		depthShader->setUniformMat4f("model", glm::mat4(1.0f));

		setViewport(0, 0, 2048.0f, 2048.0f);
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

		//std::sort(line_meshes.begin(), line_meshes.end());

		for (auto& line_mesh : line_meshes)
			renderLineMesh(line_mesh);

		//renderLineMesh();
		//renderLineMesh(grid_node);

		defaultShader->bind();
		defaultMaterial->bindLights(defaultShader, scene->getLights());
		defaultMaterial->bind(defaultShader);

		glActiveTexture(GL_TEXTURE3);
		depth_attachment->bind();

		defaultShader->setUniform3f("viewPosition", scene->getActiveCamera()->getPosition());
		defaultShader->setUniform3f("lightPos", directionalLight->getDirection());
		defaultShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
		defaultShader->setUniformMat4f("projection", scene->getActiveCamera()->getProjectionMatrix());
		defaultShader->setUniformMat4f("lightSpaceMatrix", lightSpaceMatrix);

		for (auto node : scene->getSceneGraph()->getNodes())
			renderNode(defaultShader, node, glm::mat4(1.0f));

		//colorbuffer = depth_attachment;

		defaultShader->unbind();
		glBindTexture(GL_TEXTURE_2D, 0);
		glActiveTexture(GL_TEXTURE0);

		framebuffer->unbind();
		disableDepthTest();
	}

	void DeferredRenderer::renderNode(Shader* shader, Node* node, glm::mat4 parent, bool depth)
	{
		//if (node->name == "Cube")
		//	node->transform.setRotation(glm::vec3(0.0f, std::cos(angle) * PI, 0.0f));
		//else if (node->name == "Sphere")
		//{
		//	glm::vec3 pos = glm::vec3(std::cos(angle), 2.5f, std::sin(angle));
		//	node->transform.setPosition(pos);
		//	node->transform.setRotation(glm::vec3(std::cos(angle) * PI, std::cos(angle), std::sin(angle) * PI));
		//	//pointLight->setPosition(glm::vec3(pos.x, pos.y - 4.0f, pos.z));
		//}

		glm::mat4 local = parent * node->transform.getMatrix();
	
		for (auto& mesh : node->meshes)
		{
			shader->setUniformMat4f("model", local);

			if (mesh->getMaterial() != nullptr && !depth)
			{
				mesh->getMaterial()->bindLights(defaultShader, scenesManager->getActiveScene()->getLights());
				mesh->getMaterial()->bind(defaultShader);
			}

			if (mesh->hasCulling())
			{
				glEnable(GL_CULL_FACE);
				glCullFace((GLenum)mesh->getCullType());
			}
			else
				glDisable(GL_CULL_FACE);


			if(mesh->getDrawMode() == StaticMesh::DrawMode::LINES)
				glLineWidth(mesh->getLineWidth());

			mesh->getVao()->bind();
			mesh->draw();
			mesh->getVao()->unbind();
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, 0);
		}

		for (auto& n : node->nodes)
			renderNode(shader, n, local);
	}

	void DeferredRenderer::renderParticleSystems()
	{
	}

	void DeferredRenderer::renderLineMesh(Node* node)
	{
		std::shared_ptr<Scene> scene = scenesManager->getActiveScene();

		gridShader->bind();

		gridShader->setUniformMat4f("view", scene->getActiveCamera()->getViewMatrix());
		gridShader->setUniformMat4f("projection", scene->getActiveCamera()->getProjectionMatrix());
		gridShader->setUniformMat4f("model", node->transform.getMatrix());
		gridShader->setUniform3f("color", glm::vec3(0.4f, 0.4f, 0.4f));

		for (auto& mesh : node->meshes)
		{
			mesh->getMaterial()->bind(gridShader);

			mesh->getVao()->bind();
			mesh->draw();
			mesh->getVao()->unbind();
		}

		gridShader->unbind();
	}

	void DeferredRenderer::clear(ClearType type)
	{
		int flags;
		glClearColor(color.x, color.y, color.z, color.w);
		
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

	void DeferredRenderer::enableDepthTest()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void DeferredRenderer::disableDepthTest()
	{
		glDisable(GL_DEPTH_TEST);
	}

	void DeferredRenderer::setViewport(unsigned int x, unsigned int y, float w, float h)
	{
		glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
	}

	TextureAttachment* DeferredRenderer::getColorBuffer()
	{
		return colorbuffer;
	}

}


