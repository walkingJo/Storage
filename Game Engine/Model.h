#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <vector>
#include "cassert"
#include "Vector3.h"

class Model {
public:
	Model(Vector3 centor, Vector3* vertices, int verCount, int* indices, int idxCount) {
		this->centor = centor;
		//this->direction = Vector3::UnitY;
		
		this->vrtSize = verCount;
		this->vertices = new Vector3[vrtSize];
		for (int i = 0; i < vrtSize; ++i)
			this->vertices[i] = vertices[i];

		this->idxSize = idxCount;
		this->indices = new short[idxSize];
		for (int i = 0; i < idxSize; ++i)
			this->indices[i] = indices[i];
	}
	Model(Vector3 centor, std::vector<Vector3> vertices, std::vector<int> indices) {
		this->centor = centor;
		//this->direction = Vector3::UnitY;

		this->vrtSize = (int)vertices.size();
		this->vertices = new Vector3[vrtSize];
		for (int i = 0; i < vrtSize; ++i)
			this->vertices[i] = vertices[i];

		this->idxSize = (int)indices.size();
		this->indices = new short[idxSize];
		for (int i = 0; i < idxSize; ++i)
			this->indices[i] = indices[i];
	}
	~Model() {
		delete vertices;
		delete indices;
	}

	void move(const Vector3& movement) {
		centor += movement;
		//하위모델->move(movement);
	}
	void moveTo(const Vector3& location) {
		move(location - centor);
	}
	/*vertices는 centor의 상대좌표.
	* 따라서 centor와 vertices의 회전은 별개로 생각해야 한다.
	*/
	void rotate(const Vector3& axis, float radian) {
		for (int i = 0; i < vrtSize; ++i) {
			vertices[i].rotate(axis, radian);
		}
		//하위모델->rotate(axis, radian, centor);
	}
	void rotate(const Vector3& axis, float radian, const Vector3& rotateCentor) {
		move(-1 * rotateCentor);
		centor.rotate(axis, radian);
		rotate(axis, radian);
		move(rotateCentor);
	}
	void scale(float xScale, float yScale, float zScale) {
		for (int i = 0; i < vrtSize; ++i) {
			vertices[i] *= Vector3(xScale, yScale, zScale);
		}
	}
	
	static Model buildPlane(Vector3 src, Vector3 dst) {
		std::vector<Vector3> vertices = {
			Vector3(src.x, src.y, src.z),
			Vector3(dst.x, src.y, src.z),
			Vector3(src.x, dst.y, src.z),
			Vector3(src.x, src.y, dst.z),
			Vector3(src.x, dst.y, dst.z),
			Vector3(dst.x, src.y, dst.z),
			Vector3(dst.x, dst.y, src.z),
			Vector3(dst.x, dst.y, dst.z),
		};
		const std::vector<int> indices = {
			0, 1, 2,
			0, 2, 3,
			0, 3, 1,
			1, 2, 6,
			2, 3, 4,
			3, 1, 5,
			4, 5, 3,
			5, 6, 1,
			6, 4, 2,
			4, 5, 7,
			5, 6, 7,
			6, 4, 7,
		};
		return Model((src + dst) * 0.5f, vertices, indices);
	}
	static Model buildSphere(float radius) {
		Model temp = Sphere;
		temp.scale(radius, radius, radius);
		return temp;
	}

#pragma region preprocessed model
	static const Model Tetrahedron;		//4
	static const Model Cube;			//6
	static const Model Octahedron;		//8
	//static const Model Dodecahedron;	//12
	static const Model Icosahedron;		//20

	static const Model Cylinder;
	static const Model Sphere;
#pragma endregion

private:
	Vector3 centor; //centor는 상위 모델의 중심에 대한 상대좌표이다.
public:
	short vrtSize;
	short idxSize;
	Vector3* vertices; //좌표점들은 모두 centor에 대한 상대좌표이다.
	short* indices;
};

#pragma region preprocessed model data
constexpr float pi = 3.1415926535f;

static std::vector<Vector3> tetrahedronVertices = {
	Vector3(0, 0, 1.0f),
	Vector3(+0.0000000000f, +0.9428090415f, -0.3333333333f),
	Vector3(+0.8164965809f, -0.4714045207f, -0.3333333333f),
	Vector3(-0.8164965809f, -0.4714045207f, -0.3333333333f),
};
static std::vector<int> tetrahedronIndices = {
	0, 1, 2,
	0, 2, 3,
	0, 3, 1,
	1, 2, 3
};
const Model Model::Tetrahedron(Vector3::Zero, tetrahedronVertices, tetrahedronIndices);

static std::vector<Vector3> cubeVertices = {
	Vector3(+0.5773502691f, +0.5773502691f, +0.5773502691f),
	Vector3(+0.5773502691f, -0.5773502691f, +0.5773502691f),
	Vector3(+0.5773502691f, +0.5773502691f, -0.5773502691f),
	Vector3(+0.5773502691f, -0.5773502691f, -0.5773502691f),
	Vector3(-0.5773502691f, +0.5773502691f, +0.5773502691f),
	Vector3(-0.5773502691f, -0.5773502691f, +0.5773502691f),
	Vector3(-0.5773502691f, +0.5773502691f, -0.5773502691f),
	Vector3(-0.5773502691f, -0.5773502691f, -0.5773502691f),
};
static std::vector<int> cubeIndices = {
	0, 1, 2,
	0, 2, 4,
	0, 4, 1,

	1, 2, 3,
	2, 4, 6,
	4, 1, 5,

	1, 5, 3,
	2, 3, 6,
	4, 6, 5,

	7, 3, 5,
	7, 5, 6,
	7, 6, 3
};
const Model Model::Cube(Vector3::Zero, cubeVertices, cubeIndices);

static std::vector<Vector3> octahedronVertices = {
	Vector3(+0, +1, +0),
	Vector3(+0, +0, +1),
	Vector3(+1, +0, +0),
	Vector3(+0, -1, +0),
	Vector3(+0, +0, -1),
	Vector3(-1, +0, +0)
};
static std::vector<int> octahedronIndices = {
	0, 1, 2,
	1, 2, 3,
	2, 3, 4,
	3, 4, 5,
	0, 1, 5,
	0, 2, 4,
	0, 4, 5,
	1, 3, 5
};
const Model Model::Octahedron(Vector3::Zero, octahedronVertices, octahedronIndices);

//static std::vector<Vector3> dodecahedronVertices = {};
//static std::vector<int> dodecahedronIndices = {};
//const Model Model::Dodecahedron;

static std::vector<Vector3> icosahedronVertices = {
	Vector3(0, 0, +1),
	Vector3(cos(0.2f * pi), sin(0.2f * pi), +0.4316157797f),
	Vector3(cos(0.4f * pi), sin(0.4f * pi), -0.4316157797f),
	Vector3(cos(0.6f * pi), sin(0.6f * pi), +0.4316157797f),
	Vector3(cos(0.8f * pi), sin(0.8f * pi), -0.4316157797f),
	Vector3(cos(1.0f * pi), sin(1.0f * pi), +0.4316157797f),
	Vector3(cos(1.2f * pi), sin(1.2f * pi), -0.4316157797f),
	Vector3(cos(1.4f * pi), sin(1.4f * pi), +0.4316157797f),
	Vector3(cos(1.6f * pi), sin(1.6f * pi), -0.4316157797f),
	Vector3(cos(1.8f * pi), sin(1.8f * pi), +0.4316157797f),
	Vector3(cos(2.0f * pi), sin(2.0f * pi), -0.4316157797f),
	Vector3(0, 0, -1),
};
static std::vector<int> icosahedronIndices = {
	 0,  1,  3,
	 0,  3,  5,
	 0,  5,  7,
	 0,  7,  9,
	 0,  9,  1,
	 1,  2,  3,
	 2,  3,  4,
	 3,  4,  5,
	 4,  5,  6,
	 5,  6,  7,
	 6,  7,  8,
	 7,  8,  9,
	 8,  9, 10,
	 9, 10,  1,
	10,  1,  2,
	11,  2,  4,
	11,  4,  6,
	11,  6,  8,
	11,  8, 10,
	11, 10,  2,
};
const Model Model::Icosahedron(Vector3::Zero, icosahedronVertices, icosahedronIndices);

static std::vector<Vector3> cylinderVertices = {
	Vector3(0, 0, +1.0f),

	Vector3(cos( 0 * pi / 8.0f), sin( 0 * pi / 8.0f), +1.0f),
	Vector3(cos( 1 * pi / 8.0f), sin( 1 * pi / 8.0f), +1.0f),
	Vector3(cos( 2 * pi / 8.0f), sin( 2 * pi / 8.0f), +1.0f),
	Vector3(cos( 3 * pi / 8.0f), sin( 3 * pi / 8.0f), +1.0f),
	Vector3(cos( 4 * pi / 8.0f), sin( 4 * pi / 8.0f), +1.0f),
	Vector3(cos( 5 * pi / 8.0f), sin( 5 * pi / 8.0f), +1.0f),
	Vector3(cos( 6 * pi / 8.0f), sin( 6 * pi / 8.0f), +1.0f),
	Vector3(cos( 7 * pi / 8.0f), sin( 7 * pi / 8.0f), +1.0f),
	Vector3(cos( 8 * pi / 8.0f), sin( 8 * pi / 8.0f), +1.0f),
	Vector3(cos( 9 * pi / 8.0f), sin( 9 * pi / 8.0f), +1.0f),
	Vector3(cos(10 * pi / 8.0f), sin(10 * pi / 8.0f), +1.0f),
	Vector3(cos(11 * pi / 8.0f), sin(11 * pi / 8.0f), +1.0f),
	Vector3(cos(12 * pi / 8.0f), sin(12 * pi / 8.0f), +1.0f),
	Vector3(cos(13 * pi / 8.0f), sin(13 * pi / 8.0f), +1.0f),
	Vector3(cos(14 * pi / 8.0f), sin(14 * pi / 8.0f), +1.0f),
	Vector3(cos(15 * pi / 8.0f), sin(15 * pi / 8.0f), +1.0f),

	Vector3(cos( 0 * pi / 8.0f), sin( 0 * pi / 8.0f), -1.0f),
	Vector3(cos( 1 * pi / 8.0f), sin( 1 * pi / 8.0f), -1.0f),
	Vector3(cos( 2 * pi / 8.0f), sin( 2 * pi / 8.0f), -1.0f),
	Vector3(cos( 3 * pi / 8.0f), sin( 3 * pi / 8.0f), -1.0f),
	Vector3(cos( 4 * pi / 8.0f), sin( 4 * pi / 8.0f), -1.0f),
	Vector3(cos( 5 * pi / 8.0f), sin( 5 * pi / 8.0f), -1.0f),
	Vector3(cos( 6 * pi / 8.0f), sin( 6 * pi / 8.0f), -1.0f),
	Vector3(cos( 7 * pi / 8.0f), sin( 7 * pi / 8.0f), -1.0f),
	Vector3(cos( 8 * pi / 8.0f), sin( 8 * pi / 8.0f), -1.0f),
	Vector3(cos( 9 * pi / 8.0f), sin( 9 * pi / 8.0f), -1.0f),
	Vector3(cos(10 * pi / 8.0f), sin(10 * pi / 8.0f), -1.0f),
	Vector3(cos(11 * pi / 8.0f), sin(11 * pi / 8.0f), -1.0f),
	Vector3(cos(12 * pi / 8.0f), sin(12 * pi / 8.0f), -1.0f),
	Vector3(cos(13 * pi / 8.0f), sin(13 * pi / 8.0f), -1.0f),
	Vector3(cos(14 * pi / 8.0f), sin(14 * pi / 8.0f), -1.0f),
	Vector3(cos(15 * pi / 8.0f), sin(15 * pi / 8.0f), -1.0f),

	Vector3(0, 0, -1.0f),
};
static std::vector<int> cylinderIndices = {
	 1,  2,  0,
	 2,  3,  0,
	 3,  4,  0,
	 4,  5,  0,
	 5,  6,  0,
	 6,  7,  0,
	 7,  8,  0,
	 8,  9,  0,
	 9, 10,  0,
	10, 11,  0,
	11, 12,  0,
	12, 13,  0,
	13, 14,  0,
	14, 15,  0,
	15, 16,  0,
	16,  1,  0,

	 1,  2, 17,
	 2,  3, 18,
	 3,  4, 19,
	 4,  5, 20,
	 5,  6, 21,
	 6,  7, 22,
	 7,  8, 23,
	 8,  9, 24,
	 9, 10, 25,
	10, 11, 26,
	11, 12, 27,
	12, 13, 28,
	13, 14, 29,
	14, 15, 30,
	15, 16, 31,
	16,  1, 32,
//	 9, 10, 26,
//	10, 11, 27,
//	11, 12, 28,
//	12, 13, 29,
//	13, 14, 30,
//	14, 15, 31,
//	15, 16, 32,
//	16,  1, 17,

	17, 18,  2,
	18, 19,  3,
	19, 20,  4,
	20, 21,  5,
	21, 22,  6,
	22, 23,  7,
	23, 24,  8,
	24, 25,  9,
	25, 26, 10,
	26, 27, 11,
	27, 28, 12,
	28, 29, 13,
	29, 30, 14,
	30, 31, 15,
	31, 32, 16,
	32, 17,  1,
//	25, 26,  9,
//	26, 27, 10,
//	27, 28, 11,
//	28, 29, 12,
//	29, 30, 13,
//	30, 31, 14,
//	31, 32, 15,
//	32, 17, 16,

	17, 18, 33,
	18, 19, 33,
	19, 20, 33,
	20, 21, 33,
	21, 22, 33,
	22, 23, 33,
	23, 24, 33,
	24, 25, 33,
	25, 26, 33,
	26, 27, 33,
	27, 28, 33,
	28, 29, 33,
	29, 30, 33,
	30, 31, 33,
	31, 32, 33,
	32, 17, 33,
};
const Model Model::Cylinder(Vector3::Zero, cylinderVertices, cylinderIndices);

static std::vector<Vector3> sphereVertices = {
	// index : 0
	Vector3(0, 0, sin(+6 * pi / 12.0f)),

	// index : 1~16
	Vector3(cos(+5 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(+5 * pi / 12.0f)),
	Vector3(cos(+5 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(+5 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(+5 * pi / 12.0f)),

	// index : 17~32
	Vector3(cos(+4 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(+4 * pi / 12.0f)),
	Vector3(cos(+4 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(+4 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(+4 * pi / 12.0f)),

	// index : 33~48
	Vector3(cos(+3 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(+3 * pi / 12.0f)),
	Vector3(cos(+3 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(+3 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(+3 * pi / 12.0f)),

	// index : 49~64
	Vector3(cos(+2 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(+2 * pi / 12.0f)),
	Vector3(cos(+2 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(+2 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(+2 * pi / 12.0f)),

	// index : 65~80
	Vector3(cos(+1 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(+1 * pi / 12.0f)),
	Vector3(cos(+1 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(+1 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(+1 * pi / 12.0f)),

	// index : 81~96
	Vector3(cos(+0 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(+0 * pi / 12.0f)),
	Vector3(cos(+0 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(+0 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(+0 * pi / 12.0f)),

	// index : 97~112
	Vector3(cos(-1 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(-1 * pi / 12.0f)),
	Vector3(cos(-1 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(-1 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(-1 * pi / 12.0f)),

	// index : 113~128
	Vector3(cos(-2 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(-2 * pi / 12.0f)),
	Vector3(cos(-2 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(-2 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(-2 * pi / 12.0f)),

	// index : 129~144
	Vector3(cos(-3 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(-3 * pi / 12.0f)),
	Vector3(cos(-3 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(-3 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(-3 * pi / 12.0f)),

	// index : 145~160
	Vector3(cos(-4 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(-4 * pi / 12.0f)),
	Vector3(cos(-4 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(-4 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(-4 * pi / 12.0f)),

	// index : 161~176
	Vector3(cos(-5 * pi / 12.0f) * cos( 0 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 0 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 1 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 1 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 2 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 2 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 3 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 3 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 4 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 4 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 5 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 5 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 6 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 6 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 7 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 7 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 8 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 8 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos( 9 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin( 9 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos(10 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin(10 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos(11 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin(11 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos(12 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin(12 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos(13 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin(13 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos(14 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin(14 * pi / 8.0f), sin(-5 * pi / 12.0f)),
	Vector3(cos(-5 * pi / 12.0f) * cos(15 * pi / 8.0f), cos(-5 * pi / 12.0f) * sin(15 * pi / 8.0f), sin(-5 * pi / 12.0f)),

	// index : 177
	Vector3(0, 0, sin(-6 * pi / 12.0f)),
};
static std::vector<int> sphereIndices = {
	// +6
	 0,  1,  2,
	 0,  2,  3,
	 0,  3,  4,
	 0,  4,  5,
	 0,  5,  6,
	 0,  6,  7,
	 0,  7,  8,
	 0,  8,  9,
	 0,  9, 10,
	 0, 10, 11,
	 0, 11, 12,
	 0, 12, 13,
	 0, 13, 14,
	 0, 14, 15,
	 0, 15, 16,
	 0, 16,  1,

	 // +5
	 00 +  1, 00 +  2, 16 +  1,
	 00 +  2, 00 +  3, 16 +  2,
	 00 +  3, 00 +  4, 16 +  3,
	 00 +  4, 00 +  5, 16 +  4,
	 00 +  5, 00 +  6, 16 +  5,
	 00 +  6, 00 +  7, 16 +  6,
	 00 +  7, 00 +  8, 16 +  7,
	 00 +  8, 00 +  9, 16 +  8,
	 00 +  9, 00 + 10, 16 +  9,
	 00 + 10, 00 + 11, 16 + 10,
	 00 + 11, 00 + 12, 16 + 11,
	 00 + 12, 00 + 13, 16 + 12,
	 00 + 13, 00 + 14, 16 + 13,
	 00 + 14, 00 + 15, 16 + 14,
	 00 + 15, 00 + 16, 16 + 15,
	 00 + 16, 00 +  1, 16 + 16,

	 16 +  1, 16 +  2, 00 +  2,
	 16 +  2, 16 +  3, 00 +  3,
	 16 +  3, 16 +  4, 00 +  4,
	 16 +  4, 16 +  5, 00 +  5,
	 16 +  5, 16 +  6, 00 +  6,
	 16 +  6, 16 +  7, 00 +  7,
	 16 +  7, 16 +  8, 00 +  8,
	 16 +  8, 16 +  9, 00 +  9,
	 16 +  9, 16 + 10, 00 + 10,
	 16 + 10, 16 + 11, 00 + 11,
	 16 + 11, 16 + 12, 00 + 12,
	 16 + 12, 16 + 13, 00 + 13,
	 16 + 13, 16 + 14, 00 + 14,
	 16 + 14, 16 + 15, 00 + 15,
	 16 + 15, 16 + 16, 00 + 16,
	 16 + 16, 16 +  1, 00 +  1,

	 // +4
	 16 +  1, 16 +  2, 32 +  1,
	 16 +  2, 16 +  3, 32 +  2,
	 16 +  3, 16 +  4, 32 +  3,
	 16 +  4, 16 +  5, 32 +  4,
	 16 +  5, 16 +  6, 32 +  5,
	 16 +  6, 16 +  7, 32 +  6,
	 16 +  7, 16 +  8, 32 +  7,
	 16 +  8, 16 +  9, 32 +  8,
	 16 +  9, 16 + 10, 32 +  9,
	 16 + 10, 16 + 11, 32 + 10,
	 16 + 11, 16 + 12, 32 + 11,
	 16 + 12, 16 + 13, 32 + 12,
	 16 + 13, 16 + 14, 32 + 13,
	 16 + 14, 16 + 15, 32 + 14,
	 16 + 15, 16 + 16, 32 + 15,
	 16 + 16, 16 +  1, 32 + 16,

	 32 +  1, 32 +  2, 16 +  2,
	 32 +  2, 32 +  3, 16 +  3,
	 32 +  3, 32 +  4, 16 +  4,
	 32 +  4, 32 +  5, 16 +  5,
	 32 +  5, 32 +  6, 16 +  6,
	 32 +  6, 32 +  7, 16 +  7,
	 32 +  7, 32 +  8, 16 +  8,
	 32 +  8, 32 +  9, 16 +  9,
	 32 +  9, 32 + 10, 16 + 10,
	 32 + 10, 32 + 11, 16 + 11,
	 32 + 11, 32 + 12, 16 + 12,
	 32 + 12, 32 + 13, 16 + 13,
	 32 + 13, 32 + 14, 16 + 14,
	 32 + 14, 32 + 15, 16 + 15,
	 32 + 15, 32 + 16, 16 + 16,
	 32 + 16, 32 +  1, 16 +  1,

	 // +3
	 32 +  1, 32 +  2, 48 +  1,
	 32 +  2, 32 +  3, 48 +  2,
	 32 +  3, 32 +  4, 48 +  3,
	 32 +  4, 32 +  5, 48 +  4,
	 32 +  5, 32 +  6, 48 +  5,
	 32 +  6, 32 +  7, 48 +  6,
	 32 +  7, 32 +  8, 48 +  7,
	 32 +  8, 32 +  9, 48 +  8,
	 32 +  9, 32 + 10, 48 +  9,
	 32 + 10, 32 + 11, 48 + 10,
	 32 + 11, 32 + 12, 48 + 11,
	 32 + 12, 32 + 13, 48 + 12,
	 32 + 13, 32 + 14, 48 + 13,
	 32 + 14, 32 + 15, 48 + 14,
	 32 + 15, 32 + 16, 48 + 15,
	 32 + 16, 32 +  1, 48 + 16,

	 48 +  1, 48 +  2, 32 +  2,
	 48 +  2, 48 +  3, 32 +  3,
	 48 +  3, 48 +  4, 32 +  4,
	 48 +  4, 48 +  5, 32 +  5,
	 48 +  5, 48 +  6, 32 +  6,
	 48 +  6, 48 +  7, 32 +  7,
	 48 +  7, 48 +  8, 32 +  8,
	 48 +  8, 48 +  9, 32 +  9,
	 48 +  9, 48 + 10, 32 + 10,
	 48 + 10, 48 + 11, 32 + 11,
	 48 + 11, 48 + 12, 32 + 12,
	 48 + 12, 48 + 13, 32 + 13,
	 48 + 13, 48 + 14, 32 + 14,
	 48 + 14, 48 + 15, 32 + 15,
	 48 + 15, 48 + 16, 32 + 16,
	 48 + 16, 48 +  1, 32 +  1,

	 // +2
	 48 +  1, 48 +  2, 64 +  1,
	 48 +  2, 48 +  3, 64 +  2,
	 48 +  3, 48 +  4, 64 +  3,
	 48 +  4, 48 +  5, 64 +  4,
	 48 +  5, 48 +  6, 64 +  5,
	 48 +  6, 48 +  7, 64 +  6,
	 48 +  7, 48 +  8, 64 +  7,
	 48 +  8, 48 +  9, 64 +  8,
	 48 +  9, 48 + 10, 64 +  9,
	 48 + 10, 48 + 11, 64 + 10,
	 48 + 11, 48 + 12, 64 + 11,
	 48 + 12, 48 + 13, 64 + 12,
	 48 + 13, 48 + 14, 64 + 13,
	 48 + 14, 48 + 15, 64 + 14,
	 48 + 15, 48 + 16, 64 + 15,
	 48 + 16, 48 +  1, 64 + 16,

	 64 +  1, 64 +  2, 48 +  2,
	 64 +  2, 64 +  3, 48 +  3,
	 64 +  3, 64 +  4, 48 +  4,
	 64 +  4, 64 +  5, 48 +  5,
	 64 +  5, 64 +  6, 48 +  6,
	 64 +  6, 64 +  7, 48 +  7,
	 64 +  7, 64 +  8, 48 +  8,
	 64 +  8, 64 +  9, 48 +  9,
	 64 +  9, 64 + 10, 48 + 10,
	 64 + 10, 64 + 11, 48 + 11,
	 64 + 11, 64 + 12, 48 + 12,
	 64 + 12, 64 + 13, 48 + 13,
	 64 + 13, 64 + 14, 48 + 14,
	 64 + 14, 64 + 15, 48 + 15,
	 64 + 15, 64 + 16, 48 + 16,
	 64 + 16, 64 +  1, 48 +  1,

	 // +1
	 64 +  1, 64 +  2, 80 +  1,
	 64 +  2, 64 +  3, 80 +  2,
	 64 +  3, 64 +  4, 80 +  3,
	 64 +  4, 64 +  5, 80 +  4,
	 64 +  5, 64 +  6, 80 +  5,
	 64 +  6, 64 +  7, 80 +  6,
	 64 +  7, 64 +  8, 80 +  7,
	 64 +  8, 64 +  9, 80 +  8,
	 64 +  9, 64 + 10, 80 +  9,
	 64 + 10, 64 + 11, 80 + 10,
	 64 + 11, 64 + 12, 80 + 11,
	 64 + 12, 64 + 13, 80 + 12,
	 64 + 13, 64 + 14, 80 + 13,
	 64 + 14, 64 + 15, 80 + 14,
	 64 + 15, 64 + 16, 80 + 15,
	 64 + 16, 64 +  1, 80 + 16,

	 80 +  1, 80 +  2, 64 +  2,
	 80 +  2, 80 +  3, 64 +  3,
	 80 +  3, 80 +  4, 64 +  4,
	 80 +  4, 80 +  5, 64 +  5,
	 80 +  5, 80 +  6, 64 +  6,
	 80 +  6, 80 +  7, 64 +  7,
	 80 +  7, 80 +  8, 64 +  8,
	 80 +  8, 80 +  9, 64 +  9,
	 80 +  9, 80 + 10, 64 + 10,
	 80 + 10, 80 + 11, 64 + 11,
	 80 + 11, 80 + 12, 64 + 12,
	 80 + 12, 80 + 13, 64 + 13,
	 80 + 13, 80 + 14, 64 + 14,
	 80 + 14, 80 + 15, 64 + 15,
	 80 + 15, 80 + 16, 64 + 16,
	 80 + 16, 80 +  1, 64 +  1,

	 // -1
	 80 +  1, 80 +  2, 96 +  1,
	 80 +  2, 80 +  3, 96 +  2,
	 80 +  3, 80 +  4, 96 +  3,
	 80 +  4, 80 +  5, 96 +  4,
	 80 +  5, 80 +  6, 96 +  5,
	 80 +  6, 80 +  7, 96 +  6,
	 80 +  7, 80 +  8, 96 +  7,
	 80 +  8, 80 +  9, 96 +  8,
	 80 +  9, 80 + 10, 96 +  9,
	 80 + 10, 80 + 11, 96 + 10,
	 80 + 11, 80 + 12, 96 + 11,
	 80 + 12, 80 + 13, 96 + 12,
	 80 + 13, 80 + 14, 96 + 13,
	 80 + 14, 80 + 15, 96 + 14,
	 80 + 15, 80 + 16, 96 + 15,
	 80 + 16, 80 +  1, 96 + 16,

	 96 +  1, 96 +  2, 80 +  2,
	 96 +  2, 96 +  3, 80 +  3,
	 96 +  3, 96 +  4, 80 +  4,
	 96 +  4, 96 +  5, 80 +  5,
	 96 +  5, 96 +  6, 80 +  6,
	 96 +  6, 96 +  7, 80 +  7,
	 96 +  7, 96 +  8, 80 +  8,
	 96 +  8, 96 +  9, 80 +  9,
	 96 +  9, 96 + 10, 80 + 10,
	 96 + 10, 96 + 11, 80 + 11,
	 96 + 11, 96 + 12, 80 + 12,
	 96 + 12, 96 + 13, 80 + 13,
	 96 + 13, 96 + 14, 80 + 14,
	 96 + 14, 96 + 15, 80 + 15,
	 96 + 15, 96 + 16, 80 + 16,
	 96 + 16, 96 +  1, 80 +  1,

	 // -2
	 96 +  1, 96 +  2, 112 +  1,
	 96 +  2, 96 +  3, 112 +  2,
	 96 +  3, 96 +  4, 112 +  3,
	 96 +  4, 96 +  5, 112 +  4,
	 96 +  5, 96 +  6, 112 +  5,
	 96 +  6, 96 +  7, 112 +  6,
	 96 +  7, 96 +  8, 112 +  7,
	 96 +  8, 96 +  9, 112 +  8,
	 96 +  9, 96 + 10, 112 +  9,
	 96 + 10, 96 + 11, 112 + 10,
	 96 + 11, 96 + 12, 112 + 11,
	 96 + 12, 96 + 13, 112 + 12,
	 96 + 13, 96 + 14, 112 + 13,
	 96 + 14, 96 + 15, 112 + 14,
	 96 + 15, 96 + 16, 112 + 15,
	 96 + 16, 96 +  1, 112 + 16,

	 112 +  1, 112 +  2, 96 +  2,
	 112 +  2, 112 +  3, 96 +  3,
	 112 +  3, 112 +  4, 96 +  4,
	 112 +  4, 112 +  5, 96 +  5,
	 112 +  5, 112 +  6, 96 +  6,
	 112 +  6, 112 +  7, 96 +  7,
	 112 +  7, 112 +  8, 96 +  8,
	 112 +  8, 112 +  9, 96 +  9,
	 112 +  9, 112 + 10, 96 + 10,
	 112 + 10, 112 + 11, 96 + 11,
	 112 + 11, 112 + 12, 96 + 12,
	 112 + 12, 112 + 13, 96 + 13,
	 112 + 13, 112 + 14, 96 + 14,
	 112 + 14, 112 + 15, 96 + 15,
	 112 + 15, 112 + 16, 96 + 16,
	 112 + 16, 112 +  1, 96 +  1,

	 // -3
	 112 +  1, 112 +  2, 128 +  1,
	 112 +  2, 112 +  3, 128 +  2,
	 112 +  3, 112 +  4, 128 +  3,
	 112 +  4, 112 +  5, 128 +  4,
	 112 +  5, 112 +  6, 128 +  5,
	 112 +  6, 112 +  7, 128 +  6,
	 112 +  7, 112 +  8, 128 +  7,
	 112 +  8, 112 +  9, 128 +  8,
	 112 +  9, 112 + 10, 128 +  9,
	 112 + 10, 112 + 11, 128 + 10,
	 112 + 11, 112 + 12, 128 + 11,
	 112 + 12, 112 + 13, 128 + 12,
	 112 + 13, 112 + 14, 128 + 13,
	 112 + 14, 112 + 15, 128 + 14,
	 112 + 15, 112 + 16, 128 + 15,
	 112 + 16, 112 +  1, 128 + 16,

	 128 +  1, 128 +  2, 112 +  2,
	 128 +  2, 128 +  3, 112 +  3,
	 128 +  3, 128 +  4, 112 +  4,
	 128 +  4, 128 +  5, 112 +  5,
	 128 +  5, 128 +  6, 112 +  6,
	 128 +  6, 128 +  7, 112 +  7,
	 128 +  7, 128 +  8, 112 +  8,
	 128 +  8, 128 +  9, 112 +  9,
	 128 +  9, 128 + 10, 112 + 10,
	 128 + 10, 128 + 11, 112 + 11,
	 128 + 11, 128 + 12, 112 + 12,
	 128 + 12, 128 + 13, 112 + 13,
	 128 + 13, 128 + 14, 112 + 14,
	 128 + 14, 128 + 15, 112 + 15,
	 128 + 15, 128 + 16, 112 + 16,
	 128 + 16, 128 +  1, 112 +  1,

	 // -4
	 128 +  1, 128 +  2, 144 +  1,
	 128 +  2, 128 +  3, 144 +  2,
	 128 +  3, 128 +  4, 144 +  3,
	 128 +  4, 128 +  5, 144 +  4,
	 128 +  5, 128 +  6, 144 +  5,
	 128 +  6, 128 +  7, 144 +  6,
	 128 +  7, 128 +  8, 144 +  7,
	 128 +  8, 128 +  9, 144 +  8,
	 128 +  9, 128 + 10, 144 +  9,
	 128 + 10, 128 + 11, 144 + 10,
	 128 + 11, 128 + 12, 144 + 11,
	 128 + 12, 128 + 13, 144 + 12,
	 128 + 13, 128 + 14, 144 + 13,
	 128 + 14, 128 + 15, 144 + 14,
	 128 + 15, 128 + 16, 144 + 15,
	 128 + 16, 128 +  1, 144 + 16,

	 144 +  1, 144 +  2, 128 +  2,
	 144 +  2, 144 +  3, 128 +  3,
	 144 +  3, 144 +  4, 128 +  4,
	 144 +  4, 144 +  5, 128 +  5,
	 144 +  5, 144 +  6, 128 +  6,
	 144 +  6, 144 +  7, 128 +  7,
	 144 +  7, 144 +  8, 128 +  8,
	 144 +  8, 144 +  9, 128 +  9,
	 144 +  9, 144 + 10, 128 + 10,
	 144 + 10, 144 + 11, 128 + 11,
	 144 + 11, 144 + 12, 128 + 12,
	 144 + 12, 144 + 13, 128 + 13,
	 144 + 13, 144 + 14, 128 + 14,
	 144 + 14, 144 + 15, 128 + 15,
	 144 + 15, 144 + 16, 128 + 16,
	 144 + 16, 144 +  1, 128 +  1,

	 // -5
	 144 +  1, 144 +  2, 160 +  1,
	 144 +  2, 144 +  3, 160 +  2,
	 144 +  3, 144 +  4, 160 +  3,
	 144 +  4, 144 +  5, 160 +  4,
	 144 +  5, 144 +  6, 160 +  5,
	 144 +  6, 144 +  7, 160 +  6,
	 144 +  7, 144 +  8, 160 +  7,
	 144 +  8, 144 +  9, 160 +  8,
	 144 +  9, 144 + 10, 160 +  9,
	 144 + 10, 144 + 11, 160 + 10,
	 144 + 11, 144 + 12, 160 + 11,
	 144 + 12, 144 + 13, 160 + 12,
	 144 + 13, 144 + 14, 160 + 13,
	 144 + 14, 144 + 15, 160 + 14,
	 144 + 15, 144 + 16, 160 + 15,
	 144 + 16, 144 +  1, 160 + 16,

	 160 +  1, 160 +  2, 144 +  2,
	 160 +  2, 160 +  3, 144 +  3,
	 160 +  3, 160 +  4, 144 +  4,
	 160 +  4, 160 +  5, 144 +  5,
	 160 +  5, 160 +  6, 144 +  6,
	 160 +  6, 160 +  7, 144 +  7,
	 160 +  7, 160 +  8, 144 +  8,
	 160 +  8, 160 +  9, 144 +  9,
	 160 +  9, 160 + 10, 144 + 10,
	 160 + 10, 160 + 11, 144 + 11,
	 160 + 11, 160 + 12, 144 + 12,
	 160 + 12, 160 + 13, 144 + 13,
	 160 + 13, 160 + 14, 144 + 14,
	 160 + 14, 160 + 15, 144 + 15,
	 160 + 15, 160 + 16, 144 + 16,
	 160 + 16, 160 +  1, 144 +  1,

	 // -6
	 //161~176
	 177, 161, 162,
	 177, 162, 163,
	 177, 163, 164,
	 177, 164, 165,
	 177, 165, 166,
	 177, 166, 167,
	 177, 167, 168,
	 177, 168, 169,
	 177, 169, 170,
	 177, 170, 171,
	 177, 171, 172,
	 177, 172, 173,
	 177, 173, 174,
	 177, 174, 175,
	 177, 175, 176,
	 177, 176, 161,
};
const Model Model::Sphere(Vector3::Zero, sphereVertices, sphereIndices);
#pragma endregion

#endif // !MODEL_H