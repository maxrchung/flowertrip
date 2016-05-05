#ifndef VECTOR3_HPP
#define VECTOR3_HPP

#include "Matrix.hpp"

class Vector3 {
public:
	Vector3(float x = 0.0f, float y = 0.0f, float z = 0.0f);
	Vector3(Matrix m);
	Vector3 Rotate(float xRad, float yRad, float zRad);
	Vector3 RotateX(float xRad);
	Vector3 RotateY(float yRad);
	Vector3 RotateZ(float zRad);
	Vector3 RotateAround(Vector3 v, float rotAmount);
	void Normalize();
	float Magnitude();

	float x;
	float y;
	float z;
};

#endif//VECTOR3_HPP