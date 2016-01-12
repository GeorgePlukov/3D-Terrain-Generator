
#include "TerrainObj.h"
#include <cstdlib>
#include <ctime>
#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <math.h>
#include <stdio.h>
#include "Point3D.h"
#include "Vector3D.h"

using namespace std;

TerrainObj::TerrainObj() {
	rowCount = 10;
	colCount = 10;
	sizeFactor = 1;
	maxHeight = 0;
	maxHeight = 0;
	mode = 0;
	flatShade = true;
	height = new float*[rowCount];
	for (int i = 0; i < rowCount; ++i) {
		height[i] = new float[colCount];
		for (int j = 0; j < colCount; ++j)
			height [i][j] = 0;
	}
	normals = new Vector3D**[rowCount];
	for (int i = 0; i < rowCount; ++i) {
		normals[i] = new Vector3D*[colCount];
		for (int j = 0; j < colCount; ++j) {
			normals [i][j] = new Vector3D(0.0f, 1.0f, 0.0f);

		}
	}

}


void TerrainObj::generateFlat() {
	height = new float*[rowCount];
	for (int i = 0; i < rowCount; ++i) {
		height[i] = new float[colCount];
		for (int j = 0; j < colCount; ++j)
			height [i][j] = 0.0f;
	}
	normals = new Vector3D**[rowCount];
	for (int i = 0; i < rowCount; ++i) {
		normals[i] = new Vector3D*[colCount];
		for (int j = 0; j < colCount; ++j) {
			normals [i][j] = new Vector3D(0.0f, 1.0f, 0.0f);

		}
	}
}
void TerrainObj::generateCircles(int rep, float mHeight, float circleSize) {

	for (int r = 0; r < rep; r++) {
		//mHeight = rand() % 9 + 1;
		//circleSize = rand() % 3 + 5;
		int x = rand() % rowCount - 1;
		int z = rand() % colCount - 1 ;
		for (int i = 0; i < rowCount; ++i) {
			for (int j = 0; j < colCount; ++j) {
				float pd = sqrt(pow(x - i , 2) + pow(z - j, 2)) * 2 / circleSize;
				if (fabs(pd) <= 1.0) {
					height[i][j] += mHeight / 2 + cos(pd * 3.14159265) * 1 / 2;
				}
			}

		}
	}
	this->maxHeight = -110000000;
	this->minHeight = 10000000;
	for (int x1 = 0; x1 < rowCount; ++x1) {
		for (int y1 = 0; y1 < colCount; ++y1) {
			if (height[x1][y1] > maxHeight)
				maxHeight = height[x1][y1];
			if (height[x1][y1] < minHeight)
				minHeight = height[x1][y1];
		}
	}
	calculateNormals();
}


// lighthouse 3d page on fault generation was used
void TerrainObj::generateFault(int numofreps) {
	for (int reps = 0 ; reps < numofreps ; reps++) {
		float displacement = 0.4;
		float v = rand();
		float a = sin(v);
		float b = cos(v);
		float d = sqrt(rowCount * colCount);
		// rand() / RAND_MAX gives a random number between 0 and 1.
		// therefore c will be a random number between -d/2 and d/2
		float f = ((float)rand() / RAND_MAX) * d - d / 2;

		for (int r = 0; r < rowCount; r++) {
			//mHeight = rand() % 9 + 1;
			//circleSize = rand() % 3 + 5;
			for (int c = 0; c < colCount; ++c) {
				if (((a * r) + (b * c) - f ) > 0) {
					height[r][c] += displacement;
					// printf("raise\n");
				}
				else {
					height[r][c] -= displacement;
					// printf("Lower\n");
				}
			}
		}
	}
	calculateNormals();
}
TerrainObj::~TerrainObj() {}
// Used to increase or decrease a vertex on the heightmap
void TerrainObj::changeHeight( int r, int c, float change, int WIDTH, int HEIGHT) {

	
	// printf("x: %i\n", (int) (r / rowCount));
	// printf("y: %i\n", (int) (rowCount * (HEIGHT - c) / HEIGHT));
	int xpos = (int) (((float)r  * (float)rowCount)/ (float)WIDTH);
	int ypos = (int) (((float) HEIGHT - (float)c) * (float)colCount / (float) HEIGHT);

	//printf("%f\n", height[xpos][ypos]);
	height[xpos][ypos] += change;
	calculateNormals();
}

void TerrainObj::calculateNormals() {
	if (flatShade)
		for (int r = 0; r < rowCount; ++r) {
			for (int c = 0; c < colCount; ++c) {
				if (r + 1 < rowCount && c + 1 < colCount) {

					Point3D *p1 = new Point3D(r, height[r][c], c);
					Point3D *p2 = new Point3D(r + 1, height[r + 1][c], c);
					Vector3D *v1 = new Vector3D(p1, p2);

					Point3D *p3 = new Point3D(r, height[r][c], c);
					Point3D *p4 = new Point3D(r, height[r][c + 1], c + 1);
					Vector3D *v2 = new Vector3D(p3, p4);

					Vector3D *v3 = cross(v1, v2);

					v3 = v3->normalize();
					v3->x = -1 * v3->x;
					v3->y = -1 * v3->y;
					v3->z = -1 * v3->z;

					normals[r][c] = v3;


				}
			}
		}
	if (!flatShade) {
		for (int r = 0; r < rowCount; ++r) {
			for (int c = 0; c < colCount; ++c) {
				// accumulate xyz coordinates inside abc to be made into vector at
				float x = 0.0f;
				float y = 0.0f;
				float z = 0.0f;
				if (r + 1 < rowCount && c + 1 > 0) {
					// printf("r+1 c+1\n");
					Point3D *p1 = new Point3D(r, height[r][c], c);
					Point3D *p2 = new Point3D(r + 1, height[r + 1][c], c);
					Vector3D *v1 = new Vector3D(p1, p2);

					Point3D *p3 = new Point3D(r, height[r][c], c);
					Point3D *p4 = new Point3D(r, height[r][c + 1], c + 1);
					Vector3D *v2 = new Vector3D(p3, p4);

					Vector3D *v3 = cross(v1, v2);

					v3 = v3->normalize();
					v3->x = -1 * v3->x;
					v3->y = -1 * v3->y;
					v3->z = -1 * v3->z;

					x += v3->x;
					y += v3->y;
					z += v3->z;


				}
				if (r + 1 < rowCount && c - 1 > 0) {
					// printf("r+1 c-1\n");

					Point3D *p1 = new Point3D(r, height[r][c], c);
					Point3D *p2 = new Point3D(r + 1, height[r + 1][c], c);
					Vector3D *v1 = new Vector3D(p1, p2);

					Point3D *p3 = new Point3D(r, height[r][c], c);
					Point3D *p4 = new Point3D(r, height[r][c - 1], c - 1);
					Vector3D *v2 = new Vector3D(p3, p4);

					Vector3D *v3 = cross(v1, v2);

					v3 = v3->normalize();
					v3->x = -1 * v3->x;
					v3->y = -1 * v3->y;
					v3->z = -1 * v3->z;
					x += v3->x;
					y += v3->y;
					z += v3->z;
				}
				if (r - 1 > 0 && c + 1 < colCount) {
					// printf("r-1 c+1\n");

					Point3D *p1 = new Point3D(r, height[r][c], c);
					Point3D *p2 = new Point3D(r - 1, height[r - 1][c], c);
					Vector3D *v1 = new Vector3D(p1, p2);

					Point3D *p3 = new Point3D(r, height[r][c], c);
					Point3D *p4 = new Point3D(r, height[r][c + 1], c + 1);
					Vector3D *v2 = new Vector3D(p3, p4);

					Vector3D *v3 = cross(v1, v2);

					v3 = v3->normalize();
					v3->x = -1 * v3->x;
					v3->y = -1 * v3->y;
					v3->z = -1 * v3->z;
					x += v3->x;
					y += v3->y;
					z += v3->z;
					// printf("end r+1 c+1\n");

				}
				if (r - 1 > 0 && c - 1 > 0) {
					// printf("r-1 c-1\n");

					Point3D *p1 = new Point3D(r, height[r][c], c);
					Point3D *p2 = new Point3D(r - 1, height[r - 1][c], c);
					Vector3D *v1 = new Vector3D(p1, p2);

					Point3D *p3 = new Point3D(r, height[r][c], c);
					Point3D *p4 = new Point3D(r, height[r][c + 1], c + 1);
					Vector3D *v2 = new Vector3D(p3, p4);

					Vector3D *v3 = cross(v1, v2);

					v3 = v3->normalize();
					v3->x = -1 * v3->x;
					v3->y = -1 * v3->y;
					v3->z = -1 * v3->z;
					x += v3->x;
					y += v3->y;
					z += v3->z;
				}

				Vector3D *totalV = new Vector3D(x, y, z);
				totalV = totalV->normalize();
				totalV->x = -1 * totalV->x;
				totalV->y = -1 * totalV->y;
				totalV->z = -1 * totalV->z;
				// totalV->print();

				normals[r][c] = totalV;
			}
		}

	}
}

Vector3D* TerrainObj::cross(Vector3D * v1, Vector3D * v2) {

	float nx, nz, ny;
	nx = v1->y * v2->z - v1->z * v2->y;
	ny = v1->z * v2->x - v1->x * v2->z;
	nz = v1->x * v2->y - v1->y * v2->x;

	Vector3D* n = new Vector3D(nx, ny, nz);

	return n;
}
// Draw a flattened version of the terrain in two dimensions
void TerrainObj::drawFlat() {
	for (int r = 0; r < rowCount; ++r) {
		for (int c = 0; c < colCount; ++c) {
			glColor3f(1, 0, 1);
			glBegin(GL_QUADS);
			if (r + 1 < rowCount && c + 1 < colCount) {
				// top left corner
				glColor3f((height[r][c] -  minHeight) / (maxHeight - minHeight) , (height[r][c] -  minHeight) / (maxHeight - minHeight), (height[r][c] -  minHeight) / (maxHeight - minHeight));
				glVertex2f(8 * r, 8 * c);
				// Top right corner
				glColor3f((height[r + 1][c] -  minHeight) / (maxHeight - minHeight) , (height[r + 1][c] -  minHeight) / (maxHeight - minHeight), (height[r + 1][c] -  minHeight) / (maxHeight - minHeight));
				glVertex2f(8 * (r + 1), 8 * c);
				// Bottom Right Corner
				glColor3f((height[r + 1][c + 1] -  minHeight) / (maxHeight - minHeight) , (height[r + 1][c + 1] -  minHeight) / (maxHeight - minHeight), (height[r + 1][c + 1] -  minHeight) / (maxHeight - minHeight));
				glVertex2f(8 * (r + 1), 8 * (c + 1));
				// Bottom left
				glColor3f((height[r][c + 1] -  minHeight) / (maxHeight - minHeight) , (height[r][c + 1] -  minHeight) / (maxHeight - minHeight), (height[r][c + 1] -  minHeight) / (maxHeight - minHeight));
				glVertex2f(8 * r, 8 * (c + 1));
			}
			glEnd();
		}
	}
}
void TerrainObj::draw() {
	if (mode == 0 || mode == 2)
		for (int r = 0; r < rowCount; ++r) {
			if (mode == 0 || mode == 2)
				glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

			glBegin(GL_QUADS);
			for (int c = 0; c < colCount; ++c) {
				// Check to see if getting the outer edge of a square will leave the terrain
				// If it does then we know that we have hit the far edge
				if (r + 1 < rowCount && c + 1 < colCount) {
					// Top left corner
					if (flatShade)
						glNormal3f(normals[r][c]->x, normals[r][c]->y, normals[r][c]->z);
					else
						glNormal3f(normals[r][c]->x, normals[r][c]->y, normals[r][c]->z);

					glVertex3f(sizeFactor * r, height[r][c], sizeFactor * c);

					// Top right corner
					// glColor3f((height[r + 1][c] -  minHeight) / (maxHeight - minHeight) , 0, 0);
					if (flatShade)
						glNormal3f(normals[r][c]->x, normals[r][c]->y, normals[r][c]->z);
					else
						glNormal3f(normals[r + 1][c]->x, normals[r + 1][c]->y, normals[r + 1][c]->z);
					glVertex3f(sizeFactor * (r + 1), height[r + 1][c], sizeFactor * c);



					// Bottom Right Corner

					// glColor3f((height[r + 1][c + 1] -  minHeight) / (maxHeight - minHeight) , 0, 0);
					if (flatShade)
						glNormal3f(normals[r][c]->x, normals[r][c]->y, normals[r][c]->z);
					else
						glNormal3f(normals[r + 1][c + 1]->x, normals[r + 1][c + 1]->y, normals[r + 1][c + 1]->z);
					glVertex3f(sizeFactor * (r + 1), height[r + 1][c + 1], sizeFactor * (c + 1));


					// Bottom left
					// glColor3f((height[r][c + 1] -  minHeight) / (maxHeight - minHeight) , 0, 0);
					if (flatShade)
						glNormal3f(normals[r][c]->x, normals[r][c]->y, normals[r][c]->z);
					else
						glNormal3f(normals[r][c + 1]->x, normals[r][c + 1]->y, normals[r][c + 1]->z);
					glVertex3f(sizeFactor * r, height[r][c + 1], sizeFactor * (c + 1));


				}
			}

			glEnd();

		}
	// This is used to render the duel mode with wire and
	if (mode == 1 || mode == 2) {
		for (int r = 0; r < rowCount; ++r) {
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBegin(GL_QUADS);

			for (int c = 0; c < colCount; ++c) {
				// Check to see if getting the outer edge of a square will leave the terrain
				// If it does then we know that we have hit the far edge
				if (r + 1 < rowCount && c + 1 < colCount) {

					// Top left corner
					// glColor3f((height[r][c] -  minHeight) / (maxHeight - minHeight) , (height[r][c] -  minHeight) / (maxHeight - minHeight), 0);
					glVertex3f(sizeFactor * r, height[r][c], sizeFactor * c);
					// Top right corner
					// glColor3f((height[r + 1][c] -  minHeight) / (maxHeight - minHeight) , (height[r][c] -  minHeight) / (maxHeight - minHeight), 0);
					glVertex3f(sizeFactor * (r + 1), height[r + 1][c], sizeFactor * c);
					// Bottom Right Corner

					// glColor3f((height[r + 1][c + 1] -  minHeight) / (maxHeight - minHeight) , (height[r][c] -  minHeight) / (maxHeight - minHeight), 0);
					glVertex3f(sizeFactor * (r + 1), height[r + 1][c + 1], sizeFactor * (c + 1));
					// Bottom left
					// glColor3f((height[r][c + 1] -  minHeight) / (maxHeight - minHeight) , (height[r][c] -  minHeight) / (maxHeight - minHeight), 0);
					glVertex3f(sizeFactor * r, height[r][c + 1], sizeFactor * (c + 1));
				}
			}

			glEnd();

		}
	}
}