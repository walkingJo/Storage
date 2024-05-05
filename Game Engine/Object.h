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

	/*model의 centor는 최상위 model에 대한 상대좌표이다.
	* 따라서 object의 움직임은 model에 간섭하지 않는다.
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
	//생각해보니까 이거 model에 방향 벡터 넣어야하는거 아닌가?
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

	//basisConversionTo //>> 얘는 어디다 넣는게 맞는건가
	void lookAt(Vector3 target) {
		Vector3 targetDirection = target - location;
		targetDirection.normalize();

		//!

		//단순히 하나의 축으로 회전하는 것이 아닌, z축과 xy으로만 회전할 수 있도록 구현
		//  1. direction과 targetDirection의 xy 성분만을 이용해 회전
		//  2. 1번 더 회전
		//수학-코딩 모두 중요한 질문 : 최전축에 관계없이 사원수 회전은 합칠 수 있나?
		//  그건 아니다. 축이 다른 회전은 교환할 수는 있어도, 그들을 합친 것과 엄연히 다르다 봐야 할 듯
	}
	
	void update() {
		return;
	}

private:
	Vector3 location;

	Model* model;
};

#endif // !OBJECT_H