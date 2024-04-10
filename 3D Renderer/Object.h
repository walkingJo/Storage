#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"

class Object {
public:
	Object(class Game* game);
	Object();
	virtual	~Object();

	void processInput(const Uint8* keyBoardState);
	void update();

	void moveTo(const Vector3 location);
	void move(const Vector3& movement);

	//void rotateBySelf(const Vector3& rotateAxis, float radian);
	void rotate(const Vector3& rotateAxis, float radian);
	void rotateX(float radian);
	void rotateY(float radian);
	void rotateZ(float radian);
	void lookAt(Vector3 target);

	const Model& getModel();
	void setModel(Model model);
	void scale(float scale);
	Matrix4 getBasisConversion() const;

private:
	Model model;
public:
	Vector3 location;
	Vector3 direction;
	float rotation;

	Game* game;
};

#endif // !OBJECT_H