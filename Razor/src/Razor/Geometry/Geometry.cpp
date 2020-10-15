#include "rzpch.h"
#include "Geometry.h"

namespace Razor
{ 

	Circle::Circle(float radius, unsigned int divisions) : 
		StaticMesh(),
		radius(radius),
		divisions(divisions)
	{
		std::vector<float> vertices;

		for (unsigned int i = 0; i < divisions; i++)
		{
			if (i + 1 <= divisions)
			{
				float start = TAU * i / divisions;
				float end = TAU * (i + 1) / divisions;

				vertices.push_back(radius * cosf(start));
				vertices.push_back(0.0f);
				vertices.push_back(radius * sinf(start));

				vertices.push_back(radius * cosf(end));
				vertices.push_back(0.0f);
				vertices.push_back(radius * sinf(end));
			}
		}

		this->setVertices(vertices);

		vao = new VertexArray();
		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		setCulling(false);
		setDrawMode(StaticMesh::DrawMode::LINES);
		setVertexCount((unsigned int)getVertices().size() / 3);
		setName("CircleMesh");
	}

	Circle::~Circle()
	{
	}

	Quad::Quad() : StaticMesh()
	{
		this->setVertices({
			 -1.0f,  1.0f, 0.0f, 1.0f,
			 -1.0f, -1.0f, 0.0f, 1.0f,
			  1.0f, -1.0f, 0.0f, 1.0f,
			  1.0f,  1.0f, 0.0f, 1.0f
		});

		this->setUvs({
			0.0f, 0.0f,
			0.0f, 1.0f,
			1.0f, 1.0f,
			1.0f, 0.0f
		});

		this->setIndices({
			3, 2, 1, 
			3, 1, 0
		});

		vao = new VertexArray();

		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));

		vao->addBuffer(*vbo, 0, 4);
		vao->addBuffer(*uvbo, 1, 2);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size() * sizeof(unsigned int));

		setCulling(false);
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

		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));
		vao->addBuffer(*uvbo, 1, 2);

		nbo = new VertexBuffer(getNormals().data(), (unsigned int)getNormals().size() * sizeof(float));
		vao->addBuffer(*nbo, 2, 3);

		tbo = new VertexBuffer(getTangents().data(), (unsigned int)getTangents().size() * sizeof(float));
		vao->addBuffer(*tbo, 3, 3);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size() * sizeof(unsigned int));

		setVertexCount((unsigned int)getVertices().size() / 3);
		setName("PlaneStaticMesh");
		setCullType(CullType::FRONT);
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

		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));
		vao->addBuffer(*uvbo, 1, 2);

		nbo = new VertexBuffer(getNormals().data(), (unsigned int)getNormals().size() * sizeof(float));
		vao->addBuffer(*nbo, 2, 3);

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
				float pos_x = std::cos(theta) * std::sin(phi) * -radius;
				float pos_y = std::cos(phi) * -radius;
				float pos_z = std::sin(theta) * std::sin(phi) * -radius;

				vertices.push_back(pos_x);
				vertices.push_back(pos_y);
				vertices.push_back(pos_z);

				normals.push_back(pos_x);
				normals.push_back(pos_y);
				normals.push_back(pos_z);

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

		uvbo = new VertexBuffer(getUvs().data(), (unsigned int)getUvs().size() * sizeof(float));
		vao->addBuffer(*uvbo, 1, 2);

		nbo = new VertexBuffer(getNormals().data(), (unsigned int)getNormals().size() * sizeof(float));
		vao->addBuffer(*nbo, 2, 3);

		tbo = new VertexBuffer(getTangents().data(), (unsigned int)getTangents().size() * sizeof(float));
		vao->addBuffer(*tbo, 3, 3);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVertexCount((unsigned int)getVertices().size() / 3);
		setName("SphereStaticMesh");
	}

	UVSphere::~UVSphere()
	{

	}

	Cone::Cone(unsigned int slices, float radius, float height) :
		slices(slices),
		radius(radius),
		height(height)
	{
		std::vector<unsigned int>* indx = new std::vector<unsigned int>(slices * 6);
		std::vector<glm::vec3>* vertx = new std::vector<glm::vec3>(slices + 2);
		std::vector<glm::vec3>* norms = new std::vector<glm::vec3>(slices + 2);

		float dt = TAU / float(slices);

		for (unsigned int i = 0; i < slices; i++)
		{
			glm::vec3 position = glm::vec3(
				cosf(dt * i),
				0.0f, 
				sinf(dt * i)
			);

			(*vertx)[i] = position;
			(*norms)[i] = position;
		}

		(*vertx)[slices]     = glm::vec3(0,  1,  0);
		(*norms)[slices]     = glm::vec3(0,  1,  0);
		(*vertx)[slices + 1] = glm::vec3(0,  0,  0);
		(*norms)[slices + 1] = glm::vec3(0,  0,  0);

		for (unsigned int i = 0; i < slices - 1; i++)
		{
			(*indx)[i * 6 + 0] = i;
			(*indx)[i * 6 + 1] = slices;
			(*indx)[i * 6 + 2] = i + 1;
			(*indx)[i * 6 + 3] = i;
			(*indx)[i * 6 + 4] = i + 1;
			(*indx)[i * 6 + 5] = slices + 1;
		}

		(*indx)[slices * 6 - 1 - 5] = slices - 1;
		(*indx)[slices * 6 - 1 - 4] = slices;
		(*indx)[slices * 6 - 1 - 3] = 0;
		(*indx)[slices * 6 - 1 - 2] = slices - 1;
		(*indx)[slices * 6 - 1 - 1] = 0;
		(*indx)[slices * 6 - 1 - 0] = slices + 1;


		std::vector<float> v;
		for (unsigned int i = 0; i < vertx->size(); i++)
		{
			v.push_back((*vertx)[i].x);
			v.push_back((*vertx)[i].y);
			v.push_back((*vertx)[i].z);
		}
		
		std::vector<float> n;
		for (unsigned int i = 0; i < norms->size(); i++)
		{
			n.push_back((*norms)[i].x);
			n.push_back((*norms)[i].y);
			n.push_back((*norms)[i].z);
		}

		this->setVertices(v);
		this->setNormals(n);
		this->setIndices(*indx);

		vao = new VertexArray();
		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		nbo = new VertexBuffer(getNormals().data(), (unsigned int)getNormals().size() * sizeof(float));
		vao->addBuffer(*nbo, 0, 3);

		ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		delete vertx;
		delete indx;
		delete norms;

		setVertexCount(slices + 2);
		setName("ConeStaticMesh");
	}

	Cone::~Cone()
	{

	}

	Cylinder::Cylinder()
	{

	}

	Cylinder::~Cylinder()
	{

	}

	Bounding::Bounding(const AABB& box) : StaticMesh()
	{
		this->setVertices({
			// Front
			box.min_x,  box.min_y,  box.max_z,
			box.max_x,  box.min_y,  box.max_z,

			box.max_x,  box.min_y,  box.max_z,
			box.max_x,  box.max_y,  box.max_z,

			box.max_x,  box.max_y,  box.max_z,
			box.min_x,  box.max_y,  box.max_z,

			box.min_x,  box.max_y,  box.max_z,
			box.min_x,  box.min_y,  box.max_z,

			// Back
			box.min_x,  box.min_y,  box.min_z,
			box.max_x,  box.min_y,  box.min_z,

			box.max_x,  box.min_y,  box.min_z,
			box.max_x,  box.max_y,  box.min_z,

			box.max_x,  box.max_y,  box.min_z,
			box.min_x,  box.max_y,  box.min_z,

			box.min_x,  box.max_y,  box.min_z,
			box.min_x,  box.min_y,  box.min_z,

			// Sides
			box.max_x,  box.min_y,  box.max_z,
			box.max_x,  box.min_y,  box.min_z,

			box.max_x,  box.max_y,  box.max_z,
			box.max_x,  box.max_y,  box.min_z,

			box.min_x,  box.min_y,  box.max_z,
			box.min_x,  box.min_y,  box.min_z,

			box.min_x,  box.max_y,  box.max_z,
			box.min_x,  box.max_y,  box.min_z,
		});

		//this->setIndices({
		//	// front
		//	0, 1, 2,
		//	2, 3, 0,
		//	// right
		//	1, 5, 2,
		//	5, 6, 2,
		//	// back
		//	4, 5, 6,
		//	6, 7, 4,
		//	// left
		//	0, 4, 7,
		//	7, 3, 0,
		//	// top
		//	3, 2, 6,
		//	6, 7, 3,
		//	// bottom
		//	0, 1, 5,
		//	5, 4, 0
		//});

		vao = new VertexArray();
		vbo = new VertexBuffer(getVertices().data(), (unsigned int)getVertices().size() * sizeof(float));
		vao->addBuffer(*vbo, 0, 3);

		//ibo = new IndexBuffer(getIndices().data(), (unsigned int)getIndices().size());

		setVertexCount((unsigned int)getVertices().size() / 3);
		this->setLineWidth(1.1f);
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
		float length,
		bool normalized
	) :
		StaticMesh(),
		origin(origin),
		direction(direction),
		length(length),
		normalized(normalized)
	{
		glm::vec3 end = normalized
			? origin + glm::normalize(origin + direction) * length
			: origin + direction * length;

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
