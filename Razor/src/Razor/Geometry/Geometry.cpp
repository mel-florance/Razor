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

		this->setUvs({
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 0.0f,

			0.0f, 1.0f,
			1.0f, 0.0f,
			1.0f, 1.0f
		});

		vao = new VertexArray();

		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));

		vao->addBuffer(*vbo, 0, 3);
		vao->addBuffer(*uvbo, 1, 2);

		setDrawMode(StaticMesh::DrawMode::QUADS);
		setVertexCount(6);
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
			radius, 0, radius,
			-radius, 0, -radius,
			-radius, 0, radius,
			radius, 0, -radius
		});

		this->setNormals({
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f,
			0.0f, 1.0f, 0.0f
		});

		this->setUvs({
			1.0f, 0.0f,
			0.0f, 1.0f,
			0.0f, 0.0f,
			1.0f, 1.0f
		});

		this->setIndices({
			0, 1, 2,
			0, 3, 1
		});

		this->setTangents({
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f,
			1.0f, 0.0f, 0.0f
		});

		vao = new VertexArray();

		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		nbo = new VertexBuffer(getNormals().data(), (unsigned int)getNormals().size() * sizeof(float));
		vao->addBuffer(*nbo, 1, 3);

		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));
		vao->addBuffer(*uvbo, 2, 2);

		tbo = new VertexBuffer(getTangents().data(), (unsigned int)getTangents().size() * sizeof(float));
		vao->addBuffer(*tbo, 3, 3);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size() * sizeof(unsigned int));

		setVertexCount((unsigned int)getVertices().size() / 3);
		setName("PlaneStaticMesh");
	}

	Plane::~Plane()
	{
	}

	Cube::Cube(float size) 
		: StaticMesh()
	{
		this->setVertices({
			-size, -size,  size,
			-size,  size,  size,
			-size,  size, -size,
			-size, -size, -size,

			-size, -size, -size,
			-size,  size, -size,
			 size,  size, -size,
			 size, -size, -size,

			 size, -size, -size,
			 size,  size, -size,
			 size,  size,  size,
			 size, -size,  size,

			 size, -size,  size,
			 size,  size,  size,
			-size,  size,  size,
			-size, -size,  size,

			-size, -size, -size,
			 size, -size, -size,
			 size, -size,  size,
			-size, -size,  size,

			 size,  size, -size,
			-size,  size, -size,
			-size,  size,  size,
			 size,  size,  size
		});

		this->setNormals({
			-1,  0,  0,
			-1,  0,  0,
			-1,  0,  0,
			-1,  0,  0,

			 0,  0, -1,
			 0,  0, -1,
			 0,  0, -1,
			 0,  0, -1,

			 1,  0,  0,
			 1,  0,  0,
			 1,  0,  0,
			 1,  0,  0,

			 0,  0,  1,
			 0,  0,  1,
			 0,  0,  1,
			 0,  0,  1,

			 0, -1,  0,
			 0, -1,  0,
			 0, -1,  0,
			 0, -1,  0,

			 0,  1,  0,
			 0,  1,  0,
			 0,  1,  0,
			 0,  1,  0,
		});

		this->setUvs({
			0.375, 0,
			0.625, 0,
			0.625, 0.25,
			0.375, 0.25,

			0.375, 0.25,
			0.625, 0.25,
			0.625, 0.5,
			0.375, 0.5,

			0.375, 0.5,
			0.625, 0.5,
			0.625, 0.75,
			0.375, 0.75,

			0.375, 0.75,
			0.625, 0.75,
			0.625, 1,
			0.375, 1,

			0.125, 0.5,
			0.375, 0.5,
			0.375, 0.75,
			0.125, 0.75,

			0.625, 0.5,
			0.875, 0.5,
			0.875, 0.75,
			0.625, 0.75,
		});

		this->setIndices({
			0, 1, 2,
			0, 2, 3,

			4, 5, 6,
			4, 6, 7,

			8, 9, 10,
			8, 10, 11,

			12, 13, 14,
			12, 14, 15,

			16, 17, 18,
			16, 18, 19,

			20, 21, 22,
			20, 22, 23
		});

		this->setTangents({
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 0, 1, 0,
			 
			 1, 0, 0,
			 1, 0, 0,
			 1, 0, 0,
			 1, 0, 0,

			-1, 0, 0,
			-1, 0, 0,
			-1, 0, 0,
			-1, 0, 0
		});

		vao = new VertexArray();

		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		nbo = new VertexBuffer(getNormals().data(), (unsigned int)getNormals().size() * sizeof(float));
		vao->addBuffer(*nbo, 1, 3);

		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));
		vao->addBuffer(*uvbo, 2, 2);

		tbo = new VertexBuffer(getTangents().data(), (unsigned int)getTangents().size() * sizeof(float));
		vao->addBuffer(*tbo, 3, 3);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size() * sizeof(unsigned int));

		setVertexCount((unsigned int)getVertices().size() / 3);
		setName("CubeStaticMesh");
	}

	Cube::~Cube()
	{
	}

	UVSphere::UVSphere(float radius, const glm::ivec2& segments) :
		StaticMesh(),
		radius(radius),
		segments(segments)
	{
		std::vector<float> vertices;
		std::vector<float> normals;
		std::vector<float> uvs;
		std::vector<unsigned int> indices;
		bool odd = false;
		float inv_len = 1.0f / radius;

		for (int y = 0; y <= segments.y; ++y)
		{
			for (int x = 0; x <= segments.x; ++x)
			{
				float seg_x = (float)x / (float)segments.x;
				float seg_y = (float)y / (float)segments.y;
				float theta = seg_x * 2.0f * PI;
				float phi = seg_y * PI;
				float pos_x = std::cos(theta) * std::sin(phi) * radius;
				float pos_y = std::sin(-PI / 2 + PI * seg_y) * radius;
				float pos_z = std::sin(theta) * std::sin(phi) * radius;

				vertices.push_back(pos_x);
				vertices.push_back(pos_y);
				vertices.push_back(pos_z);

				normals.push_back(pos_x * inv_len);
				normals.push_back(pos_y * inv_len);
				normals.push_back(pos_z * inv_len);

				uvs.push_back(seg_x);
				uvs.push_back(seg_y);
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

		this->setVertices(vertices);
		this->setNormals(normals);
		this->setUvs(uvs);
		this->setIndices(indices);
		this->setDrawMode(StaticMesh::DrawMode::TRIANGLE_STRIP);
		calculateTangents();

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
		setName("SphereStaticMesh");
	}

	UVSphere::~UVSphere()
	{

	}

	Bounding::Bounding(const BoundingBox& box)
		: StaticMesh()
	{
		this->setVertices({
			 -1.0f, -1.0f, -1.0f,
			  1.0f, -1.0f, -1.0f,
			  1.0f,  1.0f, -1.0f,
			 -1.0f,  1.0f, -1.0f,

			 -1.0f, -1.0f,  1.0f,
			  1.0f, -1.0f,  1.0f,
			  1.0f,  1.0f,  1.0f,
			 -1.0f,  1.0f,  1.0f
		});

		this->setIndices({
			  0, 1, 2, 3,
			  4, 5, 6, 7,
			  0, 4, 1, 5,
			  2, 6, 3, 7
		});

		vao = new VertexArray();

		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVertexCount((unsigned int)getVertices().size() / 3);
		this->setDrawMode(StaticMesh::DrawMode::LINES);
		setName("BoundingBoxStaticMesh");
	}

	Bounding::~Bounding()
	{

	}

	Grid::Grid(int divisions) :
		StaticMesh(),
		divisions(divisions)
	{
		generate();
	}

	void Grid::generate()
	{
		std::vector<float> vertices;

		for (int i = -divisions; i <= divisions; i++)
		{
			if (i != 0)
			{
				vertices.push_back((float)i);
				vertices.push_back(0.0f);
				vertices.push_back((float)-divisions);

				vertices.push_back((float)i);
				vertices.push_back(0.0f);
				vertices.push_back((float)divisions);

				vertices.push_back((float)-divisions);
				vertices.push_back(0.0f);
				vertices.push_back((float)i);

				vertices.push_back((float)divisions);
				vertices.push_back(0.0f);
				vertices.push_back((float)i);
			}
		}

		this->setVertices(vertices);

		vao = new VertexArray();
		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);
		vao->unbind();

		setDrawMode(StaticMesh::DrawMode::LINES);
		setVertexCount((unsigned int)getVertices().size() * (unsigned)divisions * (unsigned)divisions);
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

		vao = new VertexArray();
		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		setVertexCount(2);
		setDrawMode(StaticMesh::DrawMode::LINES);
		setName("RayStaticMesh");
	}

	Ray::~Ray()
	{

	}
}
