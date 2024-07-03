#include <windows.h>
#include <GL/glut.h>
#include "COLOR.h"
#include "CUBE_GRID.h"
#include "METABALL.h"
#include "VECTOR3D.h"
#include "TIMER.h"

float angle = 0.0f;					
float posX = 0.0f, rotX =  0.0f;	
float posY = 0.0f, rotY =  0.0f;	
float posZ = 5.0f, rotZ = -1.0f;	

float objectAngleX = 0.0f;			
float objectAngleY = 0.0f;			
float objectAngleZ = 0.0f;			

const int minGridSize = 10;
int gridSize = 40;
float threshold = 1.0f;

const int numDiffuseColors = 3;
COLOR diffuseColors[numDiffuseColors];
int currentDiffuseColor = 0;

COLOR backgroundColor(0.773f, 0.773f, 0.773f, 1.0f);
CUBE_GRID cubeGrid;

const int numMetaballs = 4; 
METABALL metaballs[numMetaballs];
TIMER timers;

float shininess = 32.0f;
float ambient[] = { 0.0f, 0.0f, 0.2f, 1.0f };
float position[] = { -1.0f, 1.0f, 1.0f, 0.0f };
float specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };

void drawObject() {
	glPushMatrix();
	
	glRotatef(objectAngleY, 0.0, 1.0f, 0.0f);
	
	glEnable(GL_NORMALIZE);
	glEnable(GL_CULL_FACE);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, diffuseColors[currentDiffuseColor]);
	glLightfv(GL_LIGHT1, GL_POSITION, position);
	glLightfv(GL_LIGHT1, GL_SPECULAR, specular);
	glEnable(GL_LIGHT1);
	
	glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, white);
	glMaterialfv(GL_FRONT, GL_SPECULAR, white);
	glMaterialfv(GL_FRONT, GL_SHININESS, &shininess);
	
	float c = 2.0f * (float)cos(timers.GetTime() / 600);
	
	float animationTime = timers.GetTime() / 1000.0f;  
	float maxDistance = 6.0f;  
	float minDistance = 0.0f;  
	
	metaballs[0].position.x = -maxDistance + (maxDistance - minDistance) * (1 - cos(animationTime));
	metaballs[0].position.y = 0.0f;
	
	metaballs[1].position.x = maxDistance - (maxDistance - minDistance) * (1 - cos(animationTime));
	metaballs[1].position.y = 0.0f;
	
	metaballs[2].position.x = 0.0f;
	metaballs[2].position.y = maxDistance - (maxDistance - minDistance) * (1 - cos(animationTime));
	
	metaballs[3].position.x = 0.0f;
	metaballs[3].position.y = -maxDistance + (maxDistance - minDistance) * (1 - cos(animationTime));
		
	for (int i = 0; i < cubeGrid.numVertices; i++) { 
		cubeGrid.vertices[i].value = 0.0f;
		cubeGrid.vertices[i].normal.LoadZero();
	}
	
	VECTOR3D ballToPoint;
	float squaredRadius;
	VECTOR3D ballPosition;
	float normalScale;
	for (int i = 0; i < numMetaballs; i++) {
		squaredRadius = metaballs[i].squaredRadius;
		ballPosition = metaballs[i].position;
		for (int j = 0; j < cubeGrid.numVertices; j++) {
			ballToPoint.x = cubeGrid.vertices[j].position.x - ballPosition.x;
			ballToPoint.y = cubeGrid.vertices[j].position.y - ballPosition.y;
			ballToPoint.z = cubeGrid.vertices[j].position.z - ballPosition.z;
			float squaredDistance = ballToPoint.x * ballToPoint.x + ballToPoint.y * ballToPoint.y + ballToPoint.z * ballToPoint.z;
			if (squaredDistance == 0.0f)
				squaredDistance = 0.0001f;
			cubeGrid.vertices[j].value += squaredRadius / squaredDistance;
			normalScale = squaredRadius / (squaredDistance * squaredDistance);
			cubeGrid.vertices[j].normal.x += ballToPoint.x * normalScale;
			cubeGrid.vertices[j].normal.y += ballToPoint.y * normalScale;
			cubeGrid.vertices[j].normal.z += ballToPoint.z * normalScale;
		}
	}
	glPopMatrix();
}

void display() {
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
	
	glShadeModel(GL_SMOOTH);
	
	drawObject();
	
	glEnable(GL_LIGHTING);
	glTranslatef(0.0f, 0.0f, -30.0f);
	glRotatef((float)timers.GetTime() / 30, 1.0f, 0.0f, 1.0f);
	cubeGrid.DrawSurface(threshold);
	glDisable(GL_LIGHTING);
	
	cubeGrid.DrawSurface(threshold);
	
	glutSwapBuffers();
}

void init(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	
	if (!cubeGrid.CreateMemory())
		return;
	if (!cubeGrid.Init(gridSize))
		return;
		
	for (int i = 0; i < numMetaballs; i++)
		metaballs[i].Init(VECTOR3D(0.0f, 0.0f, 0.0f), 2.5f); 
	
	diffuseColors[0].Set(1.0f, 0.0f, 0.0f, 1.0f); 
	diffuseColors[1].Set(0.047f, 0.839f, 0.271f, 1.0f);
	diffuseColors[2].Set(0.976f, 0.213f, 0.847f, 1.0f);
	
	timers.Reset();
}

void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(int key, int x, int y) {
	float fraction = 0.1f;

	switch (key) {
	case GLUT_KEY_LEFT:
		objectAngleY -= 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_RIGHT:		
		objectAngleY += 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_UP:		
		objectAngleX -= 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_DOWN:		
		objectAngleX += 1.0f;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_UP:
		posX += rotX * fraction;		
		posZ += rotZ * fraction;
		glutPostRedisplay();
		break;
	case GLUT_KEY_PAGE_DOWN:
		posX -= rotX * fraction;
		posZ -= rotZ * fraction;
		glutPostRedisplay();
		break;
	}
}

void timer(int value) {
	glutPostRedisplay();
	glutTimerFunc(55, timer, 0);
}

int main(int argc, char** argv) {
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	glutInitWindowSize(480, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 9 GRAFIKA KOMPUTER");
	
	init();

	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);
	
	glutTimerFunc(55, timer, 0);  


	glutMainLoop();

	return 0;
}
