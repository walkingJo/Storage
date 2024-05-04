#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"

class Object {
public:
	Object() {
		location = Vector3::Zero;
		direction = Vector3::UnitY;
		rotation = 0.0f;

		*model = Model::Cube;
	}
	Object(Model* modelData) {
		*this = Object();
		setModel(modelData);
	}
	void setModel(Model* modelData) {
		model = modelData;
	}

	void move(const Vector3& movement) {
		location += movement;
		model->move(movement);
	}
	void moveTo(const Vector3& location) {
		this->location = location;
		model->moveTo(location);
	}
	void rotate(const Vector3& axis, float radian) {
		direction.rotate(axis, radian);
		model->rotate(axis, radian);
	}
	void rotateX(float radian) {
		rotate(Vector3::UnitX, radian);
	}
	void rotateY(float radian) {
		rotate(Vector3::UnitY, radian);
	}
	void rotateZ(float radian) {
		rotate(Vector3::UnitZ, radian);
	}
	void rotate(const Vector3& axis, float radian, const Vector3& rotateCentor) {
		rotate(axis, radian);
		this->location = (this->location - rotateCentor).rotate(axis, radian) + rotateCentor;
	}

	//basisConversionTo //>> 얘는 어디다 넣는게 맞는건가
	void lookAt(Vector3 target) {
		Vector3 targetDirection = target - location; targetDirection.normalize();
		//단순히 하나의 축으로 회전하는 것이 아닌, z축과 xy으로만 회전할 수 있도록 구현
		//  1. direction과 targetDirection의 xy 성분만을 이용해 회전
		//  2. 1번 더 회전
	}
	
	void update() {
		return;
	}

private:
	Vector3 location;
	Vector3 direction;
	float rotation;

	Model* model;
};

#endif // !OBJECT_H