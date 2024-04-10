#pragma once
#ifndef MODEL_H
#define MODEL_H

#include <SDL.h>
#include <vector>
#include "Math.h"

class Model {
public:
	Model(Vector3 centor, Vector3* vertices, int verCount, int* indices, int idxCount);
	Model(Vector3 centor, std::vector<Vector3> vertices, std::vector<int> indices);
	Model() {
		Model(Vector3(), NULL, 0, NULL, 0);
	}
	virtual ~Model() {
		return;
	}

	Vector3 centor;
	std::vector<Vector3> vertices;
	std::vector<int> indices;
	std::vector<int> sortedIndices;

//private:
public:
	float triangleDepth(int index, const Vector3& cameraCoord);
	void rotate(const Vector3& rotateAxis, float radian);
	void scale(float scale);
	void moveTo(const Vector3& location);
	void move(const Vector3& movement);
public:
	void sortByDepth(const Vector3& cameraCoord);

public:
	static Model buildPlane(Vector3 src, Vector3 dst);
	//static Model buildCylinder(float radius, float height);
	static Model buildSphere(float radius);

	static const Model Tetrahedron;		//4
	static const Model Cube;			//6
	static const Model Octahedron;		//8
	//static const Model Dodecahedron;	//12
	static const Model Icosahedron;		//20

	static const Model Cylinder;
	static const Model Sphere;
};

#endif // !MODEL_H