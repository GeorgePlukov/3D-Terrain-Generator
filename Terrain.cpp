
#include "TerrainObj.h"

#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif
#include <stdio.h>
#include <stdlib.h>
#include <math.h>


TerrainObj t;
int ang = 10;	//angle for rotating cube
int cnt = 0;
int WIDTH;
int HEIGHT;
int gridW = 100;
int gridH = 100;
int terrainState = 1;
float eye[3];
bool teapot = true;
float lookat[] = {0.0f, 0.0f, 0.0f};
int xRot = 0, yRot = 0, zRot = 0;
int window_1, window_2;
/* LIGHTING */


int light1 = 1;
int light2 = 0;


float light_pos0[] = { 0.0f, 50.0f, 0.0f, 1.0f};
float amb0[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float diff0[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float spec0[4] = {1.0f, 1.0f, 1.0f, 1.0f};


float light_pos1[] = { 100.0f, 50.0f, 100.0f, 1.0f};
float amb1[4] = {0.1f, 0.2f, 0.1f, 1.0f};
float diff1[4] = {0.5f, 0.5f, 0.5f, 1.0f};
float spec1[4] = {0.5f, 0.5f, 0.5f, 1.0f};


// MATERIALS

float m_amb[] = {0.1, 0.4, 0.1, 1.0};
float m_diff[] = {0.9, 0.4, 0.4, 1.0};
float m_spec[] = {0.4, 0.9f, 0.2f, 1.0};
float shiny = 10;
float charList [1] = {0.0};
/* display() - the OpenGL display function, this draws the screen
 *  it displays a spinning cube
 */


void display_1()
{
	glMatrixMode(GL_MODELVIEW);

	//clear the screen

	glLoadIdentity();

	cnt++;
	// gluLookAt(eye[0], eye[1], eye[2], lookat[0], lookat[1], lookat[2], 0, 1, 0);
	glRotatef(xRot, 0, 1, 0);
	glRotatef(yRot, 1, 0, 0);

	glTranslatef(-eye[0], -eye[1], -eye[2]);

	/* LIGHTING */
	// light 1
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diff0);
	glLightfv(GL_LIGHT0, GL_SPECULAR, spec0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_pos0);

	// light 2
	glLightfv(GL_LIGHT1, GL_AMBIENT, amb1);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diff1);
	glLightfv(GL_LIGHT1, GL_SPECULAR, spec1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_pos1);

	// Reset the matrix
	glPushMatrix();

	// /* MATERIALS */
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, m_amb);
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, m_diff);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, m_spec);
	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shiny);

	if (teapot) {
		// move light
		glPushMatrix();
		glTranslatef(light_pos0[0], light_pos0[1], light_pos0[2]);
		glutSolidTeapot(1);
		glPopMatrix();

		// move light 1
		glPushMatrix();
		glTranslatef(light_pos1[0], light_pos1[1], light_pos1[2]);
		glutSolidTeapot(1);
		glPopMatrix();
	}
	glColor3f(0.0f, 1.0f, 0.0f);
	// Draw the terrain
	t.draw();

	// items

	// float xHeight = t.height[(int) ceil(charList[0])][0] - t.height[(int) floor(charList[0])][0];
	// float hChar = xHeight * (charList[0] - floor(charList[0]));
	// glTranslatef(charList[0],hChar + 0.5f,0.0f);

	// glutSolidTeapot(1);


	// charList[0] += 0.05;
	//pop the matrix back to what it was prior to the rotation
	glPopMatrix();//swap buffers - rendering is done to the back buffer, bring it forward to display


}
void display_2() {
	// glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	t.drawFlat();


}
void display_master() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	display_1();

	display_2();

	glutSwapBuffers();
	//force a redisplay, to keep the animation running
	glutSetWindow( window_1 );
	glutPostRedisplay();
	glutSetWindow( window_2 );

	glutPostRedisplay();
}

void mouse(int btn, int state, int x, int y) {
	switch (btn)
	{
	case GLUT_LEFT_BUTTON:
		t.changeHeight(x, y, 1.0f, WIDTH, HEIGHT);
		break;
	case GLUT_RIGHT_BUTTON:
		t.changeHeight(x, y, -1.0f, WIDTH, HEIGHT);
		break;
	default:
		break;
	}
}
void special(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_LEFT:
		xRot -= 3;
		break;
	case GLUT_KEY_RIGHT:
		xRot += 3;
		break;
	case GLUT_KEY_UP:
		yRot -= 3;
		break;
	case GLUT_KEY_DOWN:
		yRot += 3;
		break;
	case 'z':
	case 'Z':
		break;
	case 'x':
	case 'X':
		eye[1] -= 1;
		break;


	}
}

/* kbd -- the GLUT keyboard function
 *  key -- the key pressed
 *  x and y - mouse x and y coordinates at the time the function is called
 */

void kbd(unsigned char key, int x, int y)
{
	//if the "q" key is pressed, quit the program
	if (key == 'Q' || key == 'q') {
		exit(0);
	}
	else if ( key == 'R' || key == 'r') {
		if (terrainState == 1) {
			t.generateFlat();
			t.generateCircles(1500, 1.0f, 10);

		} else if (terrainState == 2) {
			t.generateFlat();
			t.generateFault(1000);
		}
		else if (terrainState == 3) {
			t.generateFlat();

		}
	} else if (key == 'w' || key == 'W') {
		// Switch Between full render, wireframe render, duel render
		if (t.mode == 0 )
			t.mode = 1;
		else if (t.mode == 1)
			t.mode = 2;
		else if (t.mode == 2)
			t.mode = 0;

	} else if (key == 's' || key == 'S') {
		if (t.flatShade) {
			glShadeModel(GL_SMOOTH);

			t.flatShade = !t.flatShade;
		} else {
			glShadeModel(GL_FLAT);

			t.flatShade = !t.flatShade;
		}
	}
	// These are all for the movement of a light source
	else if (key == 'l' || key == 'L') {
		if (light1 == 1) {
			light2 = 1;
			light1 = 0;
		} else if (light2 == 1) {
			light1 = 1;
			light2 = 0;
		}
	} else if (key == 'i' || key == 'I') {
		//move x for current light plus
		if (light1 == 1)
			light_pos0[0] += 1;
		else if (light1 == 0)
			light_pos1[0] += 1;
	} else if (key == 'o' || key == 'O') {
		//move x for current light minus
		if (light1 == 1)
			light_pos0[0] -= 1;
		else if (light1 == 0)
			light_pos1[0] -= 1;
	} else if (key == 'p' || key == 'P') {
		//move y for current light
		if (light1 == 1)
			light_pos0[1] += 1;
		else if (light1 == 0)
			light_pos1[1] += 1;
	} else if (key == '[' || key == '{') {
		//move y for current light
		if (light1 == 1)
			light_pos0[1] -= 1;
		else if (light1 == 0)
			light_pos1[1] -= 1;
	} else if (key == ']' || key == '}') {
		//move x for current light
		if (light1 == 1)
			light_pos0[2] += 1;
		else if (light1 == 0)
			light_pos1[2] += 1;
	} else if (key == '\\' || key == '|') {
		//move x for current light
		if (light1 == 1)
			light_pos0[2] -= 1;
		else if (light1 == 0)
			light_pos1[2] -= 1;
	} else if (key == 'z' || key == 'Z') {
		eye[1] += 1;

	}
	else if (key == 'x' || key == 'X') {
		eye[1] -= 1;

	} else if (key == 't' || key == 'T') {
		teapot = !teapot;
	}
	if (key == '1') {
		terrainState = 1;

	} else if (key == '2') {
		terrainState = 2;
	}


}


int main(int argc, char** argv)
{

	// INIT WINDOW 1
	glutInit(&argc, argv);
	glutInitWindowSize(600, 600);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	window_1 = glutCreateWindow("3D Terrain");
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	printf("Controls (sorry they are shit/so many) \n!!!!!");
	printf("!!!!MOVE THE WHITE WINDOW OUT OF THE WAY TO SEE THE SCENE!!!!!\n!!!!!!");
	printf("Camera: \n");
	printf("--------------------------------------------\n");
	printf("rotate y: left and right arrows\n");
	printf("rotate x: up and down arrows\n");
	printf("up and down: z and x\n");

	printf("\n");
	printf("Light sources movement: \n");
	printf("--------------------------------------------\n");
	printf("Toggle between L1 and L2 move: l key\n");
	printf("There is teapots drawn at the points of light so you can see them\n");
	printf("t: toggle teapots\n");
	printf("x: i and o keys\n");
	printf("y: p and [ keys\n");
	printf("z: ] and \\ keys\n");

	printf("\n");
	printf("Generating terrain\n");
	printf("--------------------------------------------\n");
	printf("Hit a number from 1 or 2 to select a generating alg\n");
	printf("1: circle alg\n");
	printf("2: Fault Alg\n");
	printf("Then hit R to generate the new terrain\n");

	printf("\n");
	printf("Submap\n");
	printf("--------------------------------------------\n");
	printf("You then need to refocus onto the window by clicking to use any commands\n");
	printf("Click on the map to change the terrain (right click lower left click increase!\n");

	printf("The changing of terrain should be the third item as the second terrain/heightmap are more properly functioning\n");
	printf("\n");


	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	// glEnable(GL_NORMALIZE);
	glShadeModel(GL_FLAT);
	//glEnable(GL_COLOR_MATERIAL);

	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);


	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	//Initialize terrain
	t.rowCount = gridW;
	t.colCount = gridH;


	HEIGHT = t.colCount * 8;
	WIDTH = t.rowCount * 8;
	t.sizeFactor = 1;
	t.flatShade = true;

	t.generateFlat();
	t.generateCircles(2000, 1.0f, 10);
	gluPerspective(90, 1, 0.5, 1000);


	eye[0] = (float) t.rowCount / 2;
	eye[1] =  t.maxHeight;
	eye[2] = (float) t.colCount / 2;
	// t.height[0][0] = 14.0f;
	glutKeyboardFunc(kbd);
	glutDisplayFunc(display_master);
	glutSpecialFunc(special);


	window_2 = glutCreateWindow("Height Map");	//creates the window
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

	/* select clearing (background) color */
	glutInitWindowSize (WIDTH, WIDTH);

	glViewport( 0, 0, WIDTH, HEIGHT);
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D( 0.0, WIDTH, 0.0, WIDTH);
	glutDisplayFunc(display_master);
	glutMouseFunc(mouse);




	//start the program!
	glutMainLoop();

	return 0;
}
