#pragma once
#ifndef OBJECT_H
#define OBJECT_H

#include "Model.h"

class Object {
public:
	Object(Vector3 location = Vector3::Zero, Model* modelData = nullptr) {
		this->location = location;
		if (modelData)	this->model = new Model(*modelData);
		else			this->model = new Model(Model::Cube);
	}
	void setLocation(const Vector3& location) {
		this->location = location;
	}
	void setModel(Model* modelData) {
		model = new Model(*modelData);
	}

	/*model�� centor�� �ֻ��� model�� ���� �����ǥ�̴�.
	* ���� object�� �������� model�� �������� �ʴ´�.
	*/
	void move(const Vector3& movement) {
		location += movement;
	}
	void moveTo(const Vector3& location) {
		this->location = location;
	}
	void rotate(const Vector3& axis, float radian) {
		model->rotate(axis, radian);
	}
	//�����غ��ϱ� �̰� model�� ���� ���� �־���ϴ°� �ƴѰ�?
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
		move(-1 * rotateCentor);
		this->location.rotate(axis, radian);
		rotate(axis, radian);
		move(rotateCentor);
	}

	//basisConversionTo //>> ��� ���� �ִ°� �´°ǰ�
	void lookAt(Vector3 target) {
		Vector3 targetDirection = target - location;
		targetDirection.normalize();

		//!

		//�ܼ��� �ϳ��� ������ ȸ���ϴ� ���� �ƴ�, z��� xy���θ� ȸ���� �� �ֵ��� ����
		//  1. direction�� targetDirection�� xy ���и��� �̿��� ȸ��
		//  2. 1�� �� ȸ��
		//����-�ڵ� ��� �߿��� ���� : �����࿡ ������� ����� ȸ���� ��ĥ �� �ֳ�?
		//  �װ� �ƴϴ�. ���� �ٸ� ȸ���� ��ȯ�� ���� �־, �׵��� ��ģ �Ͱ� ������ �ٸ��� ���� �� ��
	}
	
	void update() {
		return;
	}

private:
	Vector3 location;

	Model* model;
};

#endif // !OBJECT_H