#include "rzpch.h"
#include "Mesh.h"
#include <glad/glad.h>

namespace Razor {

	Mesh::Mesh() : drawMode(DrawMode::TRIANGLES)
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::setupBuffers() {
		VertexArray* vao = new VertexArray();
		VertexBufferLayout *vbl = new VertexBufferLayout();

		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(3);
		vbl->pushFloat(3);
		vbl->pushFloat(2);
		vbl->pushFloat(3);

		vao->addBuffer(*vbo, *vbl);

		IndexBuffer* ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);
		setIbo(ibo);
	}

	void Mesh::draw()
	{
		size_t size = getIndices().size();

		if (size > 0) {
			glDrawElements((GLenum)drawMode, (GLsizei)getIndices().size(), GL_UNSIGNED_INT, nullptr);
		}
		else
			glDrawArrays((GLenum)drawMode, 0, getVertexCount());
	}

}
