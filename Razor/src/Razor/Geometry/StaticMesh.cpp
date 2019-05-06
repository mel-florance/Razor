#include "rzpch.h"
#include "StaticMesh.h"
#include <glad/glad.h>

namespace Razor {

	StaticMesh::StaticMesh() :
		drawMode(DrawMode::TRIANGLES),
		cullType(CullType::FRONT),
		culling(false),
		line_width(1.0f),
		line_factor(5),
		line_pattern(0xAAAA),
		is_line_dashed(false),
		material(nullptr)
	{
	}

	StaticMesh::~StaticMesh()
	{
		
	}

	void StaticMesh::setupBuffers() 
	{
		vao = new VertexArray();
		vbl = new VertexBufferLayout();

		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(3);
		vbl->pushFloat(3);
		vbl->pushFloat(2);
		vbl->pushFloat(3);

		vao->addBuffer(*vbo, *vbl);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);
		setIbo(ibo);
	}

	void StaticMesh::draw()
	{
		if (drawMode == DrawMode::LINES && is_line_dashed)
		{
			glLineStipple(line_factor, line_pattern);
			glEnable(GL_LINE_STIPPLE);
		}
		else 
			glDisable(GL_LINE_STIPPLE);

		if (getIndices().size() > 0)
			glDrawElements((GLenum)drawMode, (GLsizei)getIndices().size(), GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays((GLenum)drawMode, 0, getVertexCount());
	}

}
