#pragma once
#ifndef VECTOR3_H
#define VECTOR3_H

#include <cmath>

class Vector3 {
public:
	float x;
	float y;
	float z;

	explicit Vector3() {
		x = y = z = 0.0f;
	}
	explicit Vector3(float x, float y, float z) {
		this->x = x;
		this->y = y;
		this->z = z;
	}

	friend Vector3 operator+(const Vector3& a, const Vector3& b) {
		return Vector3(a.x + b.x, a.y + b.y, a.z + b.z);
	}
	friend Vector3 operator-(const Vector3& a, const Vector3& b) {
		return Vector3(a.x - b.x, a.y - b.y, a.z - b.z);
	}
	friend Vector3 operator*(const Vector3& left, const Vector3& right) {
		return Vector3(
			left.x * right.x, 
			left.y * right.y, 
			left.z * right.z);
	}
	friend Vector3 operator*(const Vector3& vec, float scalar) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}
	friend Vector3 operator*(float scalar, const Vector3& vec) {
		return Vector3(vec.x * scalar, vec.y * scalar, vec.z * scalar);
	}

	Vector3& operator*=(float scalar) {
		x *= scalar;
		y *= scalar;
		z *= scalar;
		return *this;
	}
	Vector3& operator*=(const Vector3& right) {
		x *= right.x;
		y *= right.y;
		z *= right.z;
		return *this;
	}
	Vector3& operator+=(const Vector3& right) {
		x += right.x;
		y += right.y;
		z += right.z;
		return *this;
	}
	Vector3& operator-=(const Vector3& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;
		return *this;
	}

	float lengthSq() {
		return (x * x + y * y + z * z);
	}
	float length() {
		return sqrtf(x * x + y * y + z * z);
	}

	void normalize() {
		float len = length();
		x /= len;
		y /= len;
		z /= len;
	}
	static Vector3 normalize(const Vector3& vec) {
		Vector3 temp = vec;
		temp.normalize();
		return temp;
	}

	static float Dot(const Vector3& a, const Vector3& b) {
		return (a.x * b.x + a.y * b.y + a.z * b.z);
	}
	static Vector3 Cross(const Vector3& a, const Vector3& b) {
		Vector3 temp;
		temp.x = a.y * b.z - a.z * b.y;
		temp.y = a.z * b.x - a.x * b.z;
		temp.z = a.x * b.y - a.y * b.x;
		return temp;
	}

	static Vector3 Lerp(const Vector3& a, const Vector3& b, float f) {
		return Vector3(a + f * (b - a));
	}
	static Vector3 Reflect(const Vector3& v, const Vector3& n) {
		return v - 2.0f * Vector3::Dot(v, n) * n;
	}

	Vector3 rotate(const Vector3& axis, float radian) {
		//https://jebae.github.io/quaternion-rotation
		Vector3 qv = sin(radian / 2.0f) * axis;
		float   qw = cos(radian / 2.0f);
		*this += 2.0f * Vector3::Cross(qv, Vector3::Cross(qv, *this) + qw * *this);

		return *this;
	}

	static const Vector3 Zero;
	static const Vector3 UnitX;
	static const Vector3 UnitY;
	static const Vector3 UnitZ;
	static const Vector3 NegUnitX;
	static const Vector3 NegUnitY;
	static const Vector3 NegUnitZ;
};

const Vector3 Vector3::Zero(0.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitX(1.0f, 0.0f, 0.0f);
const Vector3 Vector3::UnitY(0.0f, 1.0f, 0.0f);
const Vector3 Vector3::UnitZ(0.0f, 0.0f, 1.0f);
const Vector3 Vector3::NegUnitX(-1.0f, 0.0f, 0.0f);
const Vector3 Vector3::NegUnitY(0.0f, -1.0f, 0.0f);
const Vector3 Vector3::NegUnitZ(0.0f, 0.0f, -1.0f);

#endif // !VECTOR3_H