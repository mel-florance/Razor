#include "rzpch.h"
#include "StaticMesh.h"
#include <glad/glad.h>
#include "Razor/Physics/PhysicsBody.h"
#include "Razor/Core/Transform.h"
#include <glm/gtc/type_ptr.hpp>

namespace Razor {

	StaticMesh::StaticMesh() :
		windingOrder(WindingOrder::COUNTER_CLOCKWISE),
		drawMode(DrawMode::TRIANGLES),
		cullType(CullType::BACK),
		culling(true),
		line_width(1.0f),
		line_factor(5),
		line_pattern(0xAAAA),
		is_line_dashed(false),
		material(nullptr),
		name("StaticMesh"),
		vao(nullptr),
		vbo(nullptr),
		ibo(nullptr),
		nbo(nullptr),
		uvbo(nullptr),
		body(nullptr),
		physics_enabled(true),
		show_bounding_box(false)
	{
	}

	StaticMesh::~StaticMesh()
	{
		delete vao;
		delete vbo;
		delete nbo;
		delete uvbo;
		delete tbo;
		delete ibo;
		delete mbo;

		delete body;
		material.reset();

		std::vector<std::shared_ptr<StaticMeshInstance>>::iterator it = instances.begin();
		for (; it != instances.end(); it++)
		{
			it->reset();
			instances.erase(it);
		}
	}

	void StaticMesh::setupBuffers() 
	{
		vao = new VertexArray();

		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		nbo = new VertexBuffer(getNormals().data(), (unsigned int)getNormals().size() * sizeof(float));
		vao->addBuffer(*nbo, 1, 3);

		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));
		vao->addBuffer(*uvbo, 2, 2);

		tbo = new VertexBuffer(getTangents().data(), (unsigned int)getTangents().size() * sizeof(float));
		vao->addBuffer(*tbo, 3, 3);
		 
		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVertexCount((unsigned int)getVertices().size() / 3);
	}

	void StaticMesh::setupInstances()
	{
		if (vao != nullptr && instances.size() > 0)
		{
			std::vector<glm::mat4> matrices;

			for (auto i : instances)
				matrices.push_back(i->transform->getMatrix());

			vao->bind();
			mbo = new VertexBuffer(reinterpret_cast<GLfloat*>(&matrices[0]), matrices.size() * sizeof(glm::mat4));
			mbo->setUsage(VertexBuffer::BufferUsage::DYNAMIC_DRAW);

			glEnableVertexAttribArray(4);
			glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
			glEnableVertexAttribArray(5);
			glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
			glEnableVertexAttribArray(6);
			glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
			glEnableVertexAttribArray(7);
			glVertexAttribPointer(7, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

			glVertexAttribDivisor(4, 1);
			glVertexAttribDivisor(5, 1);
			glVertexAttribDivisor(6, 1);
			glVertexAttribDivisor(7, 1);

			mbo->unbind();
			vao->unbind();
		}
		else
			Log::error("Setup instances: The mesh %s doesn't have a vao.", name.c_str());
	}

	std::shared_ptr<StaticMesh::StaticMeshInstance> StaticMesh::addInstance(const std::string& name, Transform* transform, PhysicsBody* body)
	{
		unsigned int index = instances.size();
		std::shared_ptr<StaticMeshInstance> instance = std::make_shared<StaticMeshInstance>(index, name, transform, body);
		instances.push_back(instance);

		return instance;
	}

	void StaticMesh::updateInstance(const glm::mat4& matrix, unsigned int index)
	{
		mbo->bind();
		glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(matrix));
		mbo->unbind();
	}

	void StaticMesh::calculateTangents()
	{
		for (unsigned int i = 0; i < indices.size(); i ++)
		{
			glm::vec3 tangent;
			glm::vec3 n = glm::vec3(normals[i + 0], normals[i + 1], normals[i + 2]);
			
			glm::vec3 c1 = glm::cross(n, glm::vec3(uvs[i + 0], uvs[i + 1], uvs[i + 2]));
			glm::vec3 c2 = glm::cross(n, glm::vec3(uvs[i + 3], uvs[i + 4], uvs[i + 5]));

			tangent = c1.length() > c2.length() ? c1 : c2;
			tangent = glm::normalize(tangent);
		
			tangents.push_back(tangent.x);
			tangents.push_back(tangent.y);
			tangents.push_back(tangent.z);
		}
	}


	void StaticMesh::draw()
	{
		if (hasCulling())
		{
			glEnable(GL_CULL_FACE);
			glFrontFace((GLenum)windingOrder);
			glCullFace((GLenum)cullType);
		}
		else
			glDisable(GL_CULL_FACE);

		if (drawMode == DrawMode::LINES)
		{
			glLineWidth(line_width);

			if (is_line_dashed)
			{
				glLineStipple(line_factor, line_pattern);
				glEnable(GL_LINE_STIPPLE);
			}
			else
				glDisable(GL_LINE_STIPPLE);
		}
		else
			glLineWidth(1.0f);

		if (getIndices().size() > 0)
			glDrawElements((GLenum)drawMode, (GLsizei)getIndices().size(), GL_UNSIGNED_INT, 0);
		else
			glDrawArrays((GLenum)drawMode, 0, getVertexCount());
	}

	void StaticMesh::drawInstances()
	{
		if (hasCulling())
		{
			glEnable(GL_CULL_FACE);
			glFrontFace((GLenum)windingOrder);
			glCullFace((GLenum)cullType);
		}
		else
			glDisable(GL_CULL_FACE);

		if (getIndices().size() > 0)
			glDrawElementsInstanced((GLenum)drawMode, (GLsizei)getIndices().size(), GL_UNSIGNED_INT, 0, (GLsizei)getInstances().size());
		else
			glDrawArraysInstanced((GLenum)drawMode, 0, (GLsizei)getVertexCount(), (GLsizei)getInstances().size());
	}

}
