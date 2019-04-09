#include "rzpch.h"
#include "Mesh.h"
#include <glad/glad.h>

namespace Razor {

	Mesh::Mesh()
	{
	}

	Mesh::~Mesh()
	{
	}

	void Mesh::draw()
	{
		size_t size = getIndices().size();

		if (size > 0)
			glDrawElements(GL_TRIANGLES, (GLsizei)getIndices().size(), GL_UNSIGNED_INT, nullptr);
		else
			glDrawArrays(GL_TRIANGLES, 0, getVertexCount());
	}

}
