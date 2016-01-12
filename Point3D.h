#ifndef __Point3D_H__
#define __Point3D_H__

class Point3D {

public:
	// Constructors
	Point3D();
	Point3D(float x, float y, float z);

	// Deconstructor 
	~Point3D();

	// Public Variables
	float x, y, z;

	// Public Functions
	float distanceTo(Point3D *pt);
	float fastDistance(Point3D *pt);
	//Point3D movePoint(Vector3D v);

	void print();
private:

};



#endif