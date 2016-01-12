#ifndef __Vector3D_H__
#define __Vector3D_H__
#include "Point3D.h"

class Vector3D {

public:
	// Constructors
	Vector3D();
	Vector3D(float x, float y, float z);
	Vector3D(float x, float y, float z, float magnitude);

	Vector3D(Point3D *p1, Point3D *p2);
	// Deconstructor
	~Vector3D();

	// Public Variables
	float x, y,z, magnitude;

	// Public Functions
	Vector3D* normalize();
	Vector3D moveVector(Point3D v);
	Vector3D* multiply(float s);
	float length();
	void print();
	Vector3D operator +(const Vector3D&);

private:

};



#endif