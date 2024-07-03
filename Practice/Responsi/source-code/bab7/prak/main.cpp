#include <windows.h>
#include <math.h>
#include <GL/glut.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

float angle = 0.0f;
float posX = 0.0f, rotX =  0.0f;
float posY = 0.0f, rotY =  0.0f;
float posZ = 5.0f, rotZ = -1.0f;

float objectAngleX = 0.0f;
float objectAngleY = 0.0f;
float objectAngleZ = 0.0f;

struct Vec3
{
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	Vec3() { }
	~Vec3() { }
};

void markPoint(Vec3 points, Vec3 colors, float width)
{
	glPushMatrix();
	glColor3f(colors.X, colors.Y, colors.Z);
	glBegin(GL_QUADS);
	glVertex3f(points.X - width, points.Y - width, points.Z);
	glVertex3f(points.X + width, points.Y - width, points.Z);
	glVertex3f(points.X + width, points.Y + width, points.Z);
	glVertex3f(points.X - width, points.Y + width, points.Z);
	glEnd();
	glPopMatrix();
}

bool inverse(float inMat[16], float outMat[16])
{
	float inv[16], det;
	int i;

	inv[0] = 
		inMat[5] * inMat[10] * inMat[15] -
		inMat[5] * inMat[11] * inMat[14] -
		inMat[9] * inMat[6] * inMat[15] +
		inMat[9] * inMat[7] * inMat[14] +
		inMat[13] * inMat[6] * inMat[11] -
		inMat[13] * inMat[7] * inMat[10];

	inv[4] = 
	   -inMat[4] * inMat[10] * inMat[15] +
		inMat[4] * inMat[11] * inMat[14] +
		inMat[8] * inMat[6] * inMat[15] -
		inMat[8] * inMat[7] * inMat[14] -
		inMat[12] * inMat[6] * inMat[11] +
		inMat[12] * inMat[7] * inMat[10];

	inv[8] = 
		inMat[4] * inMat[9] * inMat[15] -
		inMat[4] * inMat[11] * inMat[13] -
		inMat[8] * inMat[5] * inMat[15] +
		inMat[8] * inMat[7] * inMat[13] +
		inMat[12] * inMat[5] * inMat[11] -
		inMat[12] * inMat[7] * inMat[9];

	inv[12] = 
	   -inMat[4] * inMat[9] * inMat[14] +
		inMat[4] * inMat[10] * inMat[13] +
		inMat[8] * inMat[5] * inMat[14] -
		inMat[8] * inMat[6] * inMat[13] -
		inMat[12] * inMat[5] * inMat[10] +
		inMat[12] * inMat[6] * inMat[9];

	inv[1] = 
	   -inMat[1] * inMat[10] * inMat[15] +
		inMat[1] * inMat[11] * inMat[14] +
		inMat[9] * inMat[2] * inMat[15] -
		inMat[9] * inMat[3] * inMat[14] -
		inMat[13] * inMat[2] * inMat[11] +
		inMat[13] * inMat[3] * inMat[10];

	inv[5] = 
		inMat[0] * inMat[10] * inMat[15] -
		inMat[0] * inMat[11] * inMat[14] -
		inMat[8] * inMat[2] * inMat[15] +
		inMat[8] * inMat[3] * inMat[14] +
		inMat[12] * inMat[2] * inMat[11] -
		inMat[12] * inMat[3] * inMat[10];

	inv[9] = 
	   -inMat[0] * inMat[9] * inMat[15] +
		inMat[0] * inMat[11] * inMat[13] +
		inMat[8] * inMat[1] * inMat[15] -
		inMat[8] * inMat[3] * inMat[13] -
		inMat[12] * inMat[1] * inMat[11] +
		inMat[12] * inMat[3] * inMat[9];

	inv[13] = 
		inMat[0] * inMat[9] * inMat[14] -
		inMat[0] * inMat[10] * inMat[13] -
		inMat[8] * inMat[1] * inMat[14] +
		inMat[8] * inMat[2] * inMat[13] +
		inMat[12] * inMat[1] * inMat[10] -
		inMat[12] * inMat[2] * inMat[9];

	inv[2] = 
		inMat[1] * inMat[6] * inMat[15] -
		inMat[1] * inMat[7] * inMat[14] -
		inMat[5] * inMat[2] * inMat[15] +
		inMat[5] * inMat[3] * inMat[14] +
		inMat[13] * inMat[2] * inMat[7] -
		inMat[13] * inMat[3] * inMat[6];

	inv[6] = 
	   -inMat[0] * inMat[6] * inMat[15] +
		inMat[0] * inMat[7] * inMat[14] +
		inMat[4] * inMat[2] * inMat[15] -
		inMat[4] * inMat[3] * inMat[14] -
		inMat[12] * inMat[2] * inMat[7] +
		inMat[12] * inMat[3] * inMat[6];

	inv[10] = 
		inMat[0] * inMat[5] * inMat[15] -
		inMat[0] * inMat[7] * inMat[13] -
		inMat[4] * inMat[1] * inMat[15] +
		inMat[4] * inMat[3] * inMat[13] +
		inMat[12] * inMat[1] * inMat[7] -
		inMat[12] * inMat[3] * inMat[5];

	inv[14] = 
	   -inMat[0] * inMat[5] * inMat[14] +
		inMat[0] * inMat[6] * inMat[13] +
		inMat[4] * inMat[1] * inMat[14] -
		inMat[4] * inMat[2] * inMat[13] -
		inMat[12] * inMat[1] * inMat[6] +
		inMat[12] * inMat[2] * inMat[5];

	inv[3] = 
	   -inMat[1] * inMat[6] * inMat[11] +
		inMat[1] * inMat[7] * inMat[10] +
		inMat[5] * inMat[2] * inMat[11] -
		inMat[5] * inMat[3] * inMat[10] -
		inMat[9] * inMat[2] * inMat[7] +
		inMat[9] * inMat[3] * inMat[6];

	inv[7] = 
		inMat[0] * inMat[6] * inMat[11] -
		inMat[0] * inMat[7] * inMat[10] -
		inMat[4] * inMat[2] * inMat[11] +
		inMat[4] * inMat[3] * inMat[10] +
		inMat[8] * inMat[2] * inMat[7] -
		inMat[8] * inMat[3] * inMat[6];

	inv[11] = 
	   -inMat[0] * inMat[5] * inMat[11] +
		inMat[0] * inMat[7] * inMat[9] +
		inMat[4] * inMat[1] * inMat[11] -
		inMat[4] * inMat[3] * inMat[9] -
		inMat[8] * inMat[1] * inMat[7] +
		inMat[8] * inMat[3] * inMat[5];

	inv[15] = 
		inMat[0] * inMat[5] * inMat[10] -
		inMat[0] * inMat[6] * inMat[9] -
		inMat[4] * inMat[1] * inMat[10] +
		inMat[4] * inMat[2] * inMat[9] +
		inMat[8] * inMat[1] * inMat[6] -
		inMat[8] * inMat[2] * inMat[5];

	det = inMat[0] * inv[0] + inMat[1] * inv[4] + inMat[2] * inv[8] + inMat[3] * inv[12];

	if (det == 0)
		return false;

	det = 1.0 / det;

	for (i = 0; i < 16; i++)
		outMat[i] = inv[i] * det;

	return true;
}

void DotMatrix(float inMat1[16], float inMat2[4], float outMat[4])
{
	outMat[0] = inMat1[0] * inMat2[0] + inMat1[1] * inMat2[1] +
		inMat1[2] * inMat2[2] + inMat1[3] * inMat2[3];
	outMat[1] = inMat1[4] * inMat2[0] + inMat1[5] * inMat2[1] +
		inMat1[6] * inMat2[2] + inMat1[7] * inMat2[3];
	outMat[2] = inMat1[8] * inMat2[0] + inMat1[9] * inMat2[1] +
		inMat1[10] * inMat2[2] + inMat1[11] * inMat2[3];
	outMat[3] = inMat1[12] * inMat2[0] + inMat1[13] * inMat2[1] +
		inMat1[14] * inMat2[2] + inMat1[15] * inMat2[3];
}

void drawSplineCubic(Vec3 point1, Vec3 point2, Vec3 point3, Vec3 point4, int nPoint)
{
	float utotal = (abs((int)(point2.X - point1.X)) + abs((int)(point3.X - point2.X)) + abs((int)(point4.X - point3.X)));
	float u2 = abs((int)(point2.X - point1.X)) / utotal;
	float u3 = (abs((int)(point2.X - point1.X)) + abs((int)(point3.X - point2.X))) / utotal;

	float inverseMat[16];
	float coeffMat[16] = {
		1.00f, 0.00f, 0.00f, 0.00f,
		1.00f, u2, pow(u2, 2), pow(u2, 3),
		1.00f, u3, pow(u3, 2), pow(u3, 3),
		1.00f, 1.00f, 1.00f, 1.00f  };
	bool status = inverse(coeffMat, inverseMat);

	if (status == true)
	{
		float outMatX[4], outMatY[4], outMatZ[4];
		float inMatX[4] = { point1.X, point2.X, point3.X, point4.X };
		float inMatY[4] = { point1.Y, point2.Y, point3.Y, point4.Y };
		float inMatZ[4] = { point1.Z, point2.Z, point3.Z, point4.Z };
		DotMatrix(inverseMat, inMatX, outMatX);
		DotMatrix(inverseMat, inMatY, outMatY);
		DotMatrix(inverseMat, inMatZ, outMatZ);

		float step = 1.0f / nPoint; 
		float pX = point1.X, pY = point1.Y, pZ = point1.Z; 
		float u = 0.0f;
		for (int i = 0; i <= nPoint; i++)
		{
			glVertex3f(pX, pY, pZ); 
			pX = outMatX[3] * pow(u, 3) + outMatX[2] * pow(u, 2) 
				+ outMatX[1] * u + outMatX[0]; 
			pY = outMatY[3] * pow(u, 3) + outMatY[2] * pow(u, 2) 
				+ outMatY[1] * u + outMatY[0]; 
			pZ = outMatZ[3] * pow(u, 3) + outMatZ[2] * pow(u, 2) 
				+ outMatZ[1] * u + outMatZ[0]; 
			u += step;
		}
	}
}

void drawSplineBezier(Vec3 point1, Vec3 point2, Vec3 point3, Vec3 point4, int nPoint)
{
	float utotal = (abs((int)(point2.X - point1.X)) + abs((int)(point3.X - point2.X)) + abs((int)(point4.X - point3.X)));
	float u2 = abs((int)(point2.X - point1.X)) / utotal;
	float u3 = (abs((int)(point2.X - point1.X)) + abs((int)(point3.X - point2.X))) / utotal;

	float inverseMat[16];
	float coeffMat[16] = {
		1.0000f, 0.0000f, 0.0000f, 0.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f,
		0.0000f, 1.0000f, 0.0000f, 0.0000f,
		0.0000f, 1.0000f, 2.0000f, 3.0000f  };
	bool status = inverse(coeffMat, inverseMat);

	if (status == true)
	{
		float outMatX[4], outMatY[4], outMatZ[4];
		float inMatX[4] = { point1.X, point4.X, 
			1.0f/(u2)*(point2.X - point1.X), 
			1.0f/(1-u3)*(point4.X - point3.X) };
		float inMatY[4] = { point1.Y, point4.Y, 
			1.0f/(u2)*(point2.Y - point1.Y),
			1.0f/(1-u3)*(point4.Y - point3.Y) };
		float inMatZ[4] = { point1.Z, point4.Z, 
			1.0f/(u2)*(point2.Z - point1.Z),
			1.0f/(1-u3)*(point4.Z - point3.Z) };
		DotMatrix(inverseMat, inMatX, outMatX);
		DotMatrix(inverseMat, inMatY, outMatY);
		DotMatrix(inverseMat, inMatZ, outMatZ);

		float step = 1.0f / nPoint;
		float pX = point1.X, pY = point1.Y, pZ = point1.Z; 
		float u = 0.0f;
		for (int i = 0; i <= nPoint; i++)
		{
			glVertex3f(pX, pY, pZ); 
			pX = outMatX[3] * pow(u, 3) + outMatX[2] * pow(u, 2) 
				+ outMatX[1] * u + outMatX[0]; 
			pY = outMatY[3] * pow(u, 3) + outMatY[2] * pow(u, 2) 
				+ outMatY[1] * u + outMatY[0]; 
			pZ = outMatZ[3] * pow(u, 3) + outMatZ[2] * pow(u, 2) 
				+ outMatZ[1] * u + outMatZ[0]; 
			u += step;
		}
	}
}

void drawSplineCatmullRom(Vec3 point1, Vec3 point2, Vec3 point3, Vec3 point4, int nPoint)
{
	float utotal = (abs((int)(point2.X - point1.X)) + abs((int)(point3.X - point2.X)) + abs((int)(point4.X - point3.X)));
	float u2 = abs((int)(point2.X - point1.X)) / utotal;
	float u3 = (abs((int)(point2.X - point1.X)) + abs((int)(point3.X - point2.X))) / utotal;

	float inverseMat[16];
	float coeffMat[16] = {
		1.0000f, 0.0000f, 0.0000f, 0.0000f,
		1.0000f, 1.0000f, 1.0000f, 1.0000f,
		0.0000f, 1.0000f, 0.0000f, 0.0000f,
		0.0000f, 1.0000f, 2.0000f, 3.0000f  };
	bool status = inverse(coeffMat, inverseMat);

	if (status == true)
	{
		float outMatX[4], outMatY[4], outMatZ[4];
		float inMatX[4] = { point1.X, point4.X, 
			1.0f/(u3)*(point3.X - point1.X), 
			1.0f/(1-u2)*(point4.X - point2.X) };
		float inMatY[4] = { point1.Y, point4.Y, 
			1.0f/(u3)*(point3.Y - point1.Y),
			1.0f/(1-u2)*(point4.Y - point2.Y) };
		float inMatZ[4] = { point1.Z, point4.Z, 
			1.0f/(u3)*(point3.Z - point1.Z),
			1.0f/(1-u2)*(point4.Z - point2.Z) };
		DotMatrix(inverseMat, inMatX, outMatX);
		DotMatrix(inverseMat, inMatY, outMatY);
		DotMatrix(inverseMat, inMatZ, outMatZ);

		float step = 1.0f / nPoint; 
		float pX = point1.X, pY = point1.Y, pZ = point1.Z; 
		float u = 0.0f;
		for (int i = 0; i <= nPoint; i++)
		{
			glVertex3f(pX, pY, pZ); 
			pX = outMatX[3] * pow(u, 3) + outMatX[2] * pow(u, 2) 
				+ outMatX[1] * u + outMatX[0]; 
			pY = outMatY[3] * pow(u, 3) + outMatY[2] * pow(u, 2) 
				+ outMatY[1] * u + outMatY[0]; 
			pZ = outMatZ[3] * pow(u, 3) + outMatZ[2] * pow(u, 2) 
				+ outMatZ[1] * u + outMatZ[0]; 
			u += step;
		}
	}
}

void drawObject(int mode)
{
	glPushMatrix();
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

	Vec3 point1 = Vec3(-150.0f, -10.0f, 0.0f);
	Vec3 point2 = Vec3( -100.0f,  50.0f, 0.0f);
	Vec3 point3 = Vec3(  50.0f,  10.0f, 0.0f);
	Vec3 point4 = Vec3( 150.0f, -50.0f, 0.0f);

	markPoint(point1, Vec3(1.0f, 0.0f, 0.0f), 5.0f);
	markPoint(point2, Vec3(0.0f, 1.0f, 0.0f), 5.0f);
	markPoint(point3, Vec3(0.0f, 0.0f, 1.0f), 5.0f);
	markPoint(point4, Vec3(1.0f, 1.0f, 0.0f), 5.0f);

	glColor3f(1.0f, 1.0f, 1.0f);

	glBegin(GL_LINES);
	if(mode == 1)
		drawSplineCubic(point1, point2, point3, point4, 30);
	else if (mode == 2)
		drawSplineBezier(point1, point2, point3, point4, 30);
	else if (mode == 3)
		drawSplineCatmullRom(point1, point2, point3, point4, 30);
	glEnd();

	glPopMatrix();
	glPopMatrix();
}

void display1()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
	drawObject(1);
	glutSwapBuffers();
}

void display2()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
	drawObject(2);
	glutSwapBuffers();
}

void display3()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
	drawObject(3);
	glutSwapBuffers();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();
	glOrtho((GLfloat)-SCREEN_WIDTH / 2, (GLfloat)SCREEN_WIDTH / 2,
		(GLfloat)-SCREEN_HEIGHT / 2, (GLfloat)SCREEN_HEIGHT / 2, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();						
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((GLfloat)-SCREEN_WIDTH / 2, (GLfloat)SCREEN_WIDTH / 2,
		(GLfloat)-SCREEN_HEIGHT / 2, (GLfloat)SCREEN_HEIGHT / 2, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

void keyboard(int key, int x, int y)
{
	float fraction = 0.1f;

	switch (key) 
	{
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

void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(55, timer, 0);
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(480, 480);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("Cubic Spline - MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 7 GRAFIKA KOMPUTER");
	init();
	glutDisplayFunc(display1);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(480, 480);
	glutInitWindowPosition(600, 100);
	glutCreateWindow("Bezier Spline - MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 7 GRAFIKA KOMPUTER");
	init();
	glutDisplayFunc(display2);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowSize(480, 480);
	glutInitWindowPosition(1120, 100);
	glutCreateWindow("Catmull-Rom Spline - MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 7 GRAFIKA KOMPUTER");
	init();
	glutDisplayFunc(display3);
	glutReshapeFunc(reshape);
	glutSpecialFunc(keyboard);

	glutMainLoop();

	return 0;
}