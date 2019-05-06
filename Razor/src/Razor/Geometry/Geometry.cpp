#include "rzpch.h"
#include "Geometry.h"

namespace Razor { 

	Quad::Quad() :
		StaticMesh()
	{
		this->setVertices({
			-1.0f,  1.0f, 0.0f, 1.0f,
			-1.0f, -1.0f, 0.0f, 0.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,

			-1.0f,  1.0f, 0.0f, 1.0f,
			 1.0f, -1.0f, 1.0f, 0.0f,
			 1.0f,  1.0f, 1.0f, 1.0f
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

		setName("QuadStaticMesh");
	}

	Quad::~Quad()
	{
	}

	Plane::Plane(float radius) :
		StaticMesh(),
		radius(radius)
	{
		this->setVertices({
			-radius, 0.0f, -radius,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f,
			 radius, 0.0f, -radius,  0.0f, 1.0f,  0.0f, 1.0f, 1.0f,
			 radius, 0.0f,  radius,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
			 radius, 0.0f,  radius,  0.0f, 1.0f,  0.0f, 1.0f, 0.0f,
			-radius, 0.0f,  radius,  0.0f, 1.0f,  0.0f, 0.0f, 0.0f,
			-radius, 0.0f, -radius,  0.0f, 1.0f,  0.0f, 0.0f, 1.0f
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

		setName("PlaneStaticMesh");
	}

	Plane::~Plane()
	{
	}

	Cube::Cube(float size) 
		: StaticMesh()
	{
		this->setVertices({
			-size, -size, -size,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,
			 size, -size, -size,  0.0f,  0.0f, -1.0f,  1.0f,  0.0f,
			 size,  size, -size,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			 size,  size, -size,  0.0f,  0.0f, -1.0f,  1.0f,  1.0f,
			-size,  size, -size,  0.0f,  0.0f, -1.0f,  0.0f,  1.0f,
			-size, -size, -size,  0.0f,  0.0f, -1.0f,  0.0f,  0.0f,

			-size, -size,  size,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,
			 size, -size,  size,  0.0f,  0.0f,  1.0f,  1.0f,  0.0f,
			 size,  size,  size,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			 size,  size,  size,  0.0f,  0.0f,  1.0f,  1.0f,  1.0f,
			-size,  size,  size,  0.0f,  0.0f,  1.0f,  0.0f,  1.0f,
			-size, -size,  size,  0.0f,  0.0f,  1.0f,  0.0f,  0.0f,

			-size,  size,  size, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			-size,  size, -size, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			-size, -size, -size, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-size, -size, -size, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			-size, -size,  size, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			-size,  size,  size, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			 size,  size,  size,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,
			 size,  size, -size,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f,
			 size, -size, -size,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 size, -size, -size,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,
			 size, -size,  size,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f,
			 size,  size,  size,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,

			-size, -size, -size,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,
			 size, -size, -size,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f,
			 size, -size,  size,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			 size, -size,  size,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,
			-size, -size,  size,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f,
			-size, -size, -size,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f,

			-size,  size, -size,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,
			 size,  size, -size,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f,
			 size,  size,  size,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			 size,  size,  size,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f,
			-size,  size,  size,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f,
			-size,  size, -size,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f
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

		setName("CubeStaticMesh");
		setHasCulling(false);
	}

	Cube::~Cube()
	{
	}

	UVSphere::UVSphere(float radius, const glm::ivec2& segments) :
		StaticMesh(),
		radius(radius),
		segments(segments)
	{
		std::vector<float> data;
		std::vector<unsigned int> indices;
		bool odd = false;

		for (int y = 0; y <= segments.y; ++y)
		{
			for (int x = 0; x <= segments.x; ++x)
			{
				float seg_x = (float)x / (float)segments.x;
				float seg_y = (float)y / (float)segments.y;
				float theta = seg_x * 2.0f * PI;
				float phi = seg_y * PI;
				float pos_x = radius * std::cos(theta) * std::sin(phi);
				float pos_y = radius * std::cos(phi);
				float pos_z = radius * std::sin(theta) * std::sin(phi);

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

		for (int y = 0; y < segments.y; ++y)
		{
			if (!odd)
			{
				for (int x = 0; x <= segments.x; ++x)
				{
					indices.push_back(y       * (segments.x + 1) + x);
					indices.push_back((y + 1) * (segments.x + 1) + x);
				}
			}
			else
			{
				for (int x = segments.x; x >= 0; --x)
				{
					indices.push_back((y + 1) * (segments.x + 1) + x);
					indices.push_back(y       * (segments.x + 1) + x);
				}
			}

			odd = !odd;
		}

		this->setVertices(data);
		this->setIndices(indices);
		this->setDrawMode(StaticMesh::DrawMode::TRIANGLE_STRIP);

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

		setName("SphereStaticMesh");
	}

	UVSphere::~UVSphere()
	{

	}

	Bounding::Bounding(const BoundingBox& box)
		: StaticMesh()
	{
		this->setVertices({
			 -1.0f, -1.0f, -1.0f, 1.0f,
			  1.0f, -1.0f, -1.0f, 1.0f,
			  1.0f,  1.0f, -1.0f, 1.0f,
			 -1.0f,  1.0f, -1.0f, 1.0f,

			 -1.0f, -1.0f,  1.0f, 1.0f,
			  1.0f, -1.0f,  1.0f, 1.0f,
			  1.0f,  1.0f,  1.0f, 1.0f,
			 -1.0f,  1.0f,  1.0f, 1.0f
		});

		this->setIndices({
			  0, 1, 2, 3,
			  4, 5, 6, 7,
			  0, 4, 1, 5,
			  2, 6, 3, 7
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

		setName("BoundingBoxStaticMesh");
	}

	Bounding::~Bounding()
	{

	}

	Grid::Grid(float divisions) :
		StaticMesh(),
		divisions(divisions)
	{
		generate();
	}

	void Grid::generate()
	{
		std::vector<float> vertices;

		for (float i = -divisions; i <= divisions; i++)
		{
			vertices.push_back(i);
			vertices.push_back(0.0f);
			vertices.push_back(-divisions);

			vertices.push_back(i);
			vertices.push_back(0.0f);
			vertices.push_back(divisions);

			vertices.push_back(-divisions);
			vertices.push_back(0.0f);
			vertices.push_back(i);

			vertices.push_back(divisions);
			vertices.push_back(0.0f);
			vertices.push_back(i);
		}

		this->setVertices(vertices);
		this->setDrawMode(StaticMesh::DrawMode::LINES);

		VertexArray* vao = new VertexArray();
		VertexBufferLayout* vbl = new VertexBufferLayout();
		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(3);

		vao->addBuffer(*vbo, *vbl);

		setVertexCount((unsigned int)getVertices().size());
		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);

		setName("GridStaticMesh");
	}

	Grid::~Grid()
	{

	}


	Ray::Ray(
		const glm::vec3& origin,
		const glm::vec3& direction,
		float length
	) :
		StaticMesh(),
		origin(origin),
		direction(direction),
		length(length)
	{
		glm::vec3 end = origin + direction * length;

		this->setVertices({
			origin.x,
			origin.y,
			origin.z,

			end.x,
			end.y,
			end.z
		});

		this->setDrawMode(StaticMesh::DrawMode::LINES);

		VertexArray* vao = new VertexArray();
		VertexBufferLayout* vbl = new VertexBufferLayout();
		VertexBuffer* vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vbl->pushFloat(3);

		vao->addBuffer(*vbo, *vbl);

		setVertexCount((unsigned int)getVertices().size());
		setVao(vao);
		setVbl(vbl);
		setVbo(vbo);

		setName("RayStaticMesh");
	}

	Ray::~Ray()
	{

	}
}
