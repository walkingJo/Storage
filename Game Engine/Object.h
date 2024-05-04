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

	//basisConversionTo //>> ��� ���� �ִ°� �´°ǰ�
	void lookAt(Vector3 target) {
		Vector3 targetDirection = target - location; targetDirection.normalize();
		//�ܼ��� �ϳ��� ������ ȸ���ϴ� ���� �ƴ�, z��� xy���θ� ȸ���� �� �ֵ��� ����
		//  1. direction�� targetDirection�� xy ���и��� �̿��� ȸ��
		//  2. 1�� �� ȸ��
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