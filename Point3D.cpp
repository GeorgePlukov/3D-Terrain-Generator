#include <stdio.h>
#include <math.h>

#include "Point3D.h"

// Constructors
Point3D::Point3D() {
	x = 0.0f;
	y = 0.0f;
}

Point3D::Point3D(float _x, float _y, float _z) {
	x = _x;
	y = _y;
	z = _z;
}


// Deconstructor
Point3D::~Point3D() {
}

// Public Functions
float Point3D::distanceTo(Point3D *pt) {
	return sqrt(pow(pt->x - this->x, 2) + pow(pt->y - this->y, 2) + pow(pt->z - this->z, 2));
}
float Point3D::fastDistance(Point3D *pt) {
	return pow(pt->x - this->x, 2) + pow(pt->y - this->y, 2) + pow(pt->z - this->z, 2);
}

void Point3D::print() {
	printf("x:%f, y:%f, z:%f\n", x, y, z);
}

