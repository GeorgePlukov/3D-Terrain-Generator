#include <stdio.h>
#include <math.h>

#include "Vector3D.h"
#include "Point3D.h"

// Constructors
Vector3D::Vector3D() {
	x = 0.0f;
	y = 0.0f;
	z = 0.0f;
	magnitude = 0.0f;
}

Vector3D::Vector3D(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
	magnitude = this->length();
}
Vector3D::Vector3D(Point3D *p1, Point3D *p2) {
	x = p2->x - p1->x;
	y = p2->y - p1->y;
	z = p2->z - p1->z;
	magnitude = p1->distanceTo(p2);
}
Vector3D::Vector3D(float _x, float _y, float _z, float _magnitude) {
	x = _x;
	y = _y;
	z = _z;
	magnitude = _magnitude;
}
// Deconstructor
Vector3D::~Vector3D() {
}
float Vector3D::length() {
	return sqrt(pow(this->x, 2) + pow(this->y, 2) + pow(this->z, 2));
}
Vector3D* Vector3D::normalize() {
	Vector3D *v;
	v = new Vector3D(this->x / this->magnitude, this->y / this->magnitude, this->z / this->magnitude);
	return v;
}
Vector3D* Vector3D::multiply(float s) {
	Vector3D *v;
	v = new Vector3D(this->x * s, this->y * s, this->z * s);
	return v;
}

void Vector3D::print() {
	printf("x:%f, y:%f,z: %f length:%f\n", x, y,z, magnitude);
}

Vector3D Vector3D::operator +(const Vector3D& other)
{
	Vector3D sum(x + other.x, y + other.y, z + other.z);
	return sum;
}