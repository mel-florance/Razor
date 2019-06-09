#include "rzpch.h"
#include "StaticMesh.h"
#include <glad/glad.h>
#include "Razor/Physics/PhysicsBody.h"

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
		physics_enabled(false),
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

		delete body;
		material.reset();
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

	void StaticMesh::setupInstances(std::vector<glm::mat4>& matrices)
	{
		unsigned int buffer;
		glGenBuffers(1, &buffer);
		glBindBuffer(GL_ARRAY_BUFFER, buffer);
		glBufferData(GL_ARRAY_BUFFER, matrices.size() * sizeof(glm::mat4), reinterpret_cast<GLfloat*>(&matrices[0]), GL_DYNAMIC_DRAW);

		vao->bind();

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

		glBindBuffer(GL_ARRAY_BUFFER, 0);

		vao->unbind();
	}

	void StaticMesh::updateInstance(const glm::mat4& matrix, unsigned int index)
	{
		glBindBuffer(GL_ARRAY_BUFFER, mbo);
		glBufferSubData(GL_ARRAY_BUFFER, index * sizeof(glm::mat4), sizeof(glm::mat4), &matrix);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
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

}
