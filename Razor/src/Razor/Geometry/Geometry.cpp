#include "rzpch.h"
#include "Geometry.h"

namespace Razor { 

	Quad::Quad() : Mesh()
	{
		this->setVertices({
			-1.0f,   1.0f,  0.0f, 1.0f,
			-1.0f,  -1.0f,  0.0f, 0.0f,
			 1.0f,  -1.0f,  1.0f, 0.0f,

			-1.0f,   1.0f,  0.0f, 1.0f,
			 1.0f,  -1.0f,  1.0f, 0.0f,
			 1.0f,   1.0f,  1.0f, 1.0f
		});

		VertexArray* vao = new VertexArray();

		VertexBufferLayout *vbl = new VertexBufferLayout();
		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(2);
		vbl->pushFloat(2);
		vao->addBuffer(*vbo, *vbl);

		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);

		setName("Quad");
	}

	Quad::~Quad()
	{
	}

	Plane::Plane() : Mesh()
	{
		this->setVertices({
			-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f
		});

		VertexArray* vao = new VertexArray();

		VertexBufferLayout *vbl = new VertexBufferLayout();
		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(3);
		vbl->pushFloat(3);
		vbl->pushFloat(2);
		vao->addBuffer(*vbo, *vbl);
		setVertexCount((unsigned int)getVertices().size());

		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);

		setName("Plane");
	}

	Plane::~Plane()
	{
	}

	Cube::Cube() : Mesh()
	{
		this->setVertices({
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,

			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f, 1.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f, 0.0f,

			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, -1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			-1.0f, -1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f,  1.0f, -1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 0.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  1.0f,  0.0f,  0.0f, 1.0f, 0.0f,

			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 1.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			 1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 1.0f, 0.0f,
			-1.0f, -1.0f,  1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 0.0f,
			-1.0f, -1.0f, -1.0f,  0.0f, -1.0f,  0.0f, 0.0f, 1.0f,

			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f,
			 1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 1.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			 1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 1.0f, 0.0f,
			-1.0f,  1.0f,  1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 0.0f,
			-1.0f,  1.0f, -1.0f,  0.0f,  1.0f,  0.0f, 0.0f, 1.0f
		});

		VertexArray* vao = new VertexArray();

		VertexBufferLayout *vbl = new VertexBufferLayout();
		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(3);
		vbl->pushFloat(3);
		vbl->pushFloat(2);
		vao->addBuffer(*vbo, *vbl);

		setVertexCount((unsigned int)getVertices().size());
		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);

		setName("Cube");
	}

	Cube::~Cube()
	{
	}

	Sphere::Sphere()
	{
		std::vector<float> data;
		std::vector<unsigned int> indices;
		const unsigned int segments_x = 72;
		const unsigned int segments_y = 24;
		bool odd = false;

		for (unsigned int y = 0; y <= segments_y; ++y)
		{
			for (unsigned int x = 0; x <= segments_x; ++x)
			{
				float seg_x = (float)x / (float)segments_x;
				float seg_y = (float)y / (float)segments_y;
				float theta = seg_x * 2.0f * PI;
				float phi = seg_y * PI;
				float pos_x = std::cos(theta) * std::sin(phi);
				float pos_y = std::cos(phi);
				float pos_z = std::sin(theta) * std::sin(phi);

				data.push_back(pos_x);
				data.push_back(pos_y);
				data.push_back(pos_z);

				data.push_back(pos_x);
				data.push_back(pos_y);
				data.push_back(pos_z);

				data.push_back(seg_x);
				data.push_back(seg_y);
			}
		}

		for (int y = 0; y < segments_y; ++y)
		{
			if (!odd)
			{
				for (int x = 0; x <= segments_x; ++x)
				{
					indices.push_back(y       * (segments_x + 1) + x);
					indices.push_back((y + 1) * (segments_x + 1) + x);
				}
			}
			else
			{
				for (int x = segments_x; x >= 0; --x)
				{
					indices.push_back((y + 1) * (segments_x + 1) + x);
					indices.push_back(y       * (segments_x + 1) + x);
				}
			}

			odd = !odd;
		}

		this->setVertices(data);
		this->setIndices(indices);
		this->setDrawMode(Mesh::DrawMode::TRIANGLE_STRIP);

		VertexArray* vao = new VertexArray();
		VertexBufferLayout *vbl = new VertexBufferLayout();
		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(3);
		vbl->pushFloat(3);
		vbl->pushFloat(2);

		vao->addBuffer(*vbo, *vbl);

		IndexBuffer* ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVertexCount((unsigned int)getVertices().size());
		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);
		setIbo(ibo);

		setName("Sphere");
	}

	Sphere::~Sphere()
	{

	}

	Bounding::Bounding(const BoundingBox& box)
	{
		this->setVertices({
			 -0.5f, -0.5f, -0.5f, 1.0f,
			  0.5f, -0.5f, -0.5f, 1.0f,
			  0.5f,  0.5f, -0.5f, 1.0f,
			 -0.5f,  0.5f, -0.5f, 1.0f,
			 -0.5f, -0.5f,  0.5f, 1.0f,
			  0.5f, -0.5f,  0.5f, 1.0f,
			  0.5f,  0.5f,  0.5f, 1.0f,
			 -0.5f,  0.5f,  0.5f, 1.0f
		});

		this->setIndices({
			  0, 1, 2, 3,
			  4, 5, 6, 7,
			  0, 4, 1, 5, 2, 6, 3, 7
		});

		VertexArray* vao = new VertexArray();

		VertexBufferLayout *vbl = new VertexBufferLayout();
		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(4);
		vao->addBuffer(*vbo, *vbl);

		IndexBuffer* ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVertexCount((unsigned int)getVertices().size());
		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);
		setIbo(ibo);

		setName("BoundingBox");
	}

	Bounding::~Bounding()
	{

	}
}
