#include "sphere.h"
const int MIN_SECTOR_COUNT = 3;
const int MIN_STACK_COUNT = 2;

Sphere::Sphere(float _radius, int _sectors, int _stacks, bool _smooth) : ShapeGeometry() {
	setShapeName("sphere");
	init(_radius, _sectors, _stacks, _smooth);
}

Sphere::~Sphere() {

}

void Sphere::buildVerticesSmooth() {
	const float PI = 3.1415926f;
	clear();

	float x, y, z, xy;                              // vertex position
	float nx, ny, nz, lengthInv = 1.0f / radius;    // normal
	float s, t;                                     // texCoord

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	for (int i = 0; i <= stackCount; i++) {
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		xy = radius * cosf(stackAngle);             // r * cos(u)
		z = radius * sinf(stackAngle);              // r * sin(u)

													// add (sectorCount+1) vertices per stack
													// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

													// vertex position
			x = xy * cosf(sectorAngle);             // r * cos(u) * cos(v)
			y = xy * sinf(sectorAngle);             // r * cos(u) * sin(v)
			pushVertex(x, y, z);

			// normalized vertex normal
			nx = x * lengthInv;
			ny = y * lengthInv;
			nz = z * lengthInv;
			pushNormal(nx, ny, nz);

			// vertex tex coord between [0, 1]
			s = (float)j / sectorCount;
			t = (float)i / stackCount;
			pushUv(s, t);
		}
	}

	unsigned int k1, k2;
	for (int i = 0; i < stackCount; ++i) {
		k1 = i * (sectorCount + 1);     // beginning of current stack
		k2 = k1 + sectorCount + 1;      // beginning of next stack

		for (int j = 0; j < sectorCount; ++j, ++k1, ++k2) {
			// 2 triangles per sector excluding 1st and last stacks
			if (i != 0) {
				addIndex(k1, k2, k1 + 1);   // k1---k2---k1+1
			}

			if (i != (stackCount - 1)) {
				addIndex(k1 + 1, k2, k2 + 1); // k1+1---k2---k2+1
			}

			// vertical lines for all stacks
			lineIndices.push_back(k1);
			lineIndices.push_back(k2);
			if (i != 0) {  // horizontal lines except 1st stack
				lineIndices.push_back(k1);
				lineIndices.push_back(k1 + 1);
			}
		}
	}

	// generate interleaved vertex array as well
	buildInterleavedVertices();
}

void Sphere::buildInterleavedVertices() {
	std::vector<float>().swap(interleavedVertices);

	std::size_t i, j;
	std::vector<glm::vec3> vertices = getVertices();
	std::vector<glm::vec3> normals = getNormals();
	std::vector<glm::vec2> uvs = getUvs();
	std::size_t count = vertices.size();
	for (i = 0; i < count; i++) {
		interleavedVertices.push_back(vertices[i].x);
		interleavedVertices.push_back(vertices[i].y);
		interleavedVertices.push_back(vertices[i].z);

		interleavedVertices.push_back(normals[i].x);
		interleavedVertices.push_back(normals[i].y);
		interleavedVertices.push_back(normals[i].z);

		interleavedVertices.push_back(uvs[i].x);
		interleavedVertices.push_back(uvs[i].y);
	}
}

void Sphere::buildVerticesFlat() {
	const float PI = 3.1415926f;

	// tmp vertex definition (x,y,z,s,t)
	struct Vertex {
		float x, y, z, s, t;
	};
	std::vector<Vertex> tmpVertices;

	float sectorStep = 2 * PI / sectorCount;
	float stackStep = PI / stackCount;
	float sectorAngle, stackAngle;

	// compute all vertices first, each vertex contains (x,y,z,s,t) except normal
	for (int i = 0; i <= stackCount; ++i) {
		stackAngle = PI / 2 - i * stackStep;        // starting from pi/2 to -pi/2
		float xy = radius * cosf(stackAngle);       // r * cos(u)
		float z = radius * sinf(stackAngle);        // r * sin(u)

													// add (sectorCount+1) vertices per stack
													// the first and last vertices have same position and normal, but different tex coords
		for (int j = 0; j <= sectorCount; ++j) {
			sectorAngle = j * sectorStep;           // starting from 0 to 2pi

			Vertex vertex;
			vertex.x = xy * cosf(sectorAngle);      // x = r * cos(u) * cos(v)
			vertex.y = xy * sinf(sectorAngle);      // y = r * cos(u) * sin(v)
			vertex.z = z;                           // z = r * sin(u)
			vertex.s = (float)j / sectorCount;        // s
			vertex.t = (float)i / stackCount;         // t
			tmpVertices.push_back(vertex);
		}
	}

	// clear memory of prev arrays
	clear();

	Vertex v1, v2, v3, v4;                          // 4 vertex positions and tex coords
	std::vector<float> n;                           // 1 face normal

	int i, j, k, vi1, vi2;
	int index = 0;                                  // index for vertex
	for (i = 0; i < stackCount; ++i)
	{
		vi1 = i * (sectorCount + 1);                // index of tmpVertices
		vi2 = (i + 1) * (sectorCount + 1);

		for (j = 0; j < sectorCount; ++j, ++vi1, ++vi2)
		{
			// get 4 vertices per sector
			//  v1--v3
			//  |    |
			//  v2--v4
			v1 = tmpVertices[vi1];
			v2 = tmpVertices[vi2];
			v3 = tmpVertices[vi1 + 1];
			v4 = tmpVertices[vi2 + 1];

			// if 1st stack and last stack, store only 1 triangle per sector
			// otherwise, store 2 triangles (quad) per sector
			if (i == 0) // a triangle for first stack ==========================
			{
				// put a triangle
				pushVertex(v1.x, v1.y, v1.z);
				pushVertex(v2.x, v2.y, v2.z);
				pushVertex(v4.x, v4.y, v4.z);

				// put tex coords of triangle
				pushUv(v1.s, v1.t);
				pushUv(v2.s, v2.t);
				pushUv(v4.s, v4.t);

				// put normal
				n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v4.x, v4.y, v4.z);
				for (k = 0; k < 3; ++k)  // same normals for 3 vertices
				{
					pushNormal(n[0], n[1], n[2]);
				}

				// put indices of 1 triangle
				addIndex(index, index + 1, index + 2);

				// indices for line (first stack requires only vertical line)
				lineIndices.push_back(index);
				lineIndices.push_back(index + 1);

				index += 3;     // for next
			}
			else if (i == (stackCount - 1)) // a triangle for last stack =========
			{
				// put a triangle
				pushVertex(v1.x, v1.y, v1.z);
				pushVertex(v2.x, v2.y, v2.z);
				pushVertex(v3.x, v3.y, v3.z);

				// put tex coords of triangle
				pushUv(v1.s, v1.t);
				pushUv(v2.s, v2.t);
				pushUv(v3.s, v3.t);

				// put normal
				n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
				for (k = 0; k < 3; ++k) {  // same normals for 3 vertices
					pushNormal(n[0], n[1], n[2]);
				}

				// put indices of 1 triangle
				addIndex(index, index + 1, index + 2);

				// indices for lines (last stack requires both vert/hori lines)
				lineIndices.push_back(index);
				lineIndices.push_back(index + 1);
				lineIndices.push_back(index);
				lineIndices.push_back(index + 2);

				index += 3;     // for next
			}
			else // 2 triangles for others ====================================
			{
				// put quad vertices: v1-v2-v3-v4
				pushVertex(v1.x, v1.y, v1.z);
				pushVertex(v2.x, v2.y, v2.z);
				pushVertex(v3.x, v3.y, v3.z);
				pushVertex(v4.x, v4.y, v4.z);

				// put tex coords of quad
				pushUv(v1.s, v1.t);
				pushUv(v2.s, v2.t);
				pushUv(v3.s, v3.t);
				pushUv(v4.s, v4.t);

				// put normal
				n = computeFaceNormal(v1.x, v1.y, v1.z, v2.x, v2.y, v2.z, v3.x, v3.y, v3.z);
				for (k = 0; k < 4; ++k)  // same normals for 4 vertices
				{
					pushNormal(n[0], n[1], n[2]);
				}

				// put indices of quad (2 triangles)
				addIndex(index, index + 1, index + 2);
				addIndex(index + 2, index + 1, index + 3);

				// indices for lines
				lineIndices.push_back(index);
				lineIndices.push_back(index + 1);
				lineIndices.push_back(index);
				lineIndices.push_back(index + 2);

				index += 4;     // for next
			}
		}
	}

	// generate interleaved vertex array as well
	buildInterleavedVertices();
}

std::vector<float> Sphere::computeFaceNormal(float x1, float y1, float z1,  // v1
	float x2, float y2, float z2,  // v2
	float x3, float y3, float z3)  // v3
{
	const float EPSILON = 0.000001f;

	std::vector<float> normal(3, 0.0f);     // default return value (0,0,0)
	float nx, ny, nz;

	// find 2 edge vectors: v1-v2, v1-v3
	float ex1 = x2 - x1;
	float ey1 = y2 - y1;
	float ez1 = z2 - z1;
	float ex2 = x3 - x1;
	float ey2 = y3 - y1;
	float ez2 = z3 - z1;

	// cross product: e1 x e2
	nx = ey1 * ez2 - ez1 * ey2;
	ny = ez1 * ex2 - ex1 * ez2;
	nz = ex1 * ey2 - ey1 * ex2;

	// normalize only if the length is > 0
	float length = sqrtf(nx * nx + ny * ny + nz * nz);
	if (length > EPSILON)
	{
		// normalize
		float lengthInv = 1.0f / length;
		normal[0] = nx * lengthInv;
		normal[1] = ny * lengthInv;
		normal[2] = nz * lengthInv;
	}

	return normal;
}

void Sphere::updateRadius() {
	std::vector<glm::vec3> vertices = getVertices();
	float scale = sqrtf(radius * radius / (vertices[0].x * vertices[0].x + vertices[0].y * vertices[0].y + vertices[0].z * vertices[0].z));
	std::size_t i, j;
	std::size_t count = vertices.size();
	for (i = 0, j = 0; i < count; i++, j += 8) {
		vertices[i].x *= scale;
		vertices[i].y *= scale;
		vertices[i].z *= scale;

		interleavedVertices[j] *= scale;
		interleavedVertices[j + 1] *= scale;
		interleavedVertices[j + 2] *= scale;
	}
	setVertices(vertices);
}

void Sphere::init(float _radius, int _sectors, int _stacks, bool _smooth) {
	radius = _radius;
	sectorCount = _sectors;
	stackCount = _stacks;
	smooth = _smooth;
	if (this->sectorCount < MIN_SECTOR_COUNT)
		this->sectorCount = MIN_SECTOR_COUNT;
	if (this->sectorCount < MIN_STACK_COUNT)
		this->sectorCount = MIN_STACK_COUNT;
	if (smooth)
		buildVerticesSmooth();
	else
		buildVerticesFlat();
}

void Sphere::setRadius(const float& _radius) {
	radius = _radius;
	updateRadius();
}

void Sphere::setSectorCount(const int& _sectors) {
	init(radius, _sectors, stackCount, smooth);
}

void Sphere::setStackCount(const int& _stacks) {
	init(radius, sectorCount, _stacks, smooth);
}

void Sphere::setSmooth(const bool& _smooth) {
	if (smooth == _smooth)
		return;
	smooth = _smooth;
	if (smooth) {
		buildVerticesSmooth();
	}
	else {
		buildVerticesFlat();
	}
}

float Sphere::getRadius() const {
	return radius;
}

int Sphere::getSectorCount() const {
	return sectorCount;
}

int Sphere::getStackCount() const {
	return stackCount;
}

bool  Sphere::isSmooth() const {
	return smooth;
}