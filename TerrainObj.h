#ifndef __TerrainObj_H__
#define __MESH3D_H__
#import "Vector3D.h"
using namespace std;

class TerrainObj {
public:

	float** height;
	Vector3D*** normals;
	int rowCount, colCount;
	bool flatShade;
	float sizeFactor, maxHeight, minHeight;
	int mode;

	TerrainObj();
	//TerrainObj(int rowC, int colC, float sFact);

	void draw();
	void drawFlat();
	void calculateNormals();
	void changeHeight(int r, int c, float chg,int WIDTH, int HEIGHT);
	Vector3D* cross (Vector3D* a, Vector3D* b);
	void generateFlat();
	void generateCircles(int rep, float maxHeight, float circleSize);
	void generateFault(int numofreps);


	~TerrainObj();

private:


};
#endif