#include <windows.h>
#include <GL/glut.h>
#include <math.h>

float angle = 0.0f;
float posX = 0.0f, rotX = 0.0f;
float posY = 0.0f, rotY = 0.0f;
float posZ = 5.0f, rotZ = -1.0f;

float objectAngleX = 0.0f;
float objectAngleY = 0.0f;
float objectAngleZ = 0.0f;

#define PHI 3.141592654
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

struct Vec3
{
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	Vec3() { }
	~Vec3() { }
};

enum INTERP_TYPE 
{
	INTERP_POINTS = 0,
	INTERP_LINES = 1,
	INTERP_LINEAR = 2,
	INTERP_COSINE = 3,
	INTERP_CUBIC = 4
};

float linearInterpolate(float y0, float y1, float u) 
{
	return (y0 * (1 - u) + y1 * u);
}

float cosineInterpolate(float y0, float y1, float u) 
{
	float cosineU = (1 - cos(u * PHI)) / 2;
	return (y0 * (1 - cosineU) + y1 * cosineU);
}

float cubicInterpolate(float y0, float y1, float y2, float y3, float u) 
{
	float a = y3 - y2 - y0 + y1;
	float b = 2 * y0 - 2 * y1 - y3 + y2;
	float c = y2 - y0;
	float d = y1;
	return(a * u * u * u + b * u * u + c * u + d);
}

void drawInterpolation( 
	Vec3 point0, 
	Vec3 point1,
	Vec3 point2,
	Vec3 point3,
	int n,
	INTERP_TYPE type1,
	INTERP_TYPE type2)
{
	float u = 0;
	float stepU = 1.0f / n; 
	float stepX = fabs(point2.X - point1.X) / n; 
	float pX = point1.X, pY = point1.Y, pZ = point1.Z; 
	glPointSize(5);
	if (type1 == INTERP_POINTS)
		glBegin(GL_POINTS);
	else if (type1 == INTERP_LINES)
		glBegin(GL_LINES);
	for (int i = 0; i < n; i++)
	{
		glVertex3f(pX, pY, pZ);
		pX = pX + stepX;
		u = u + stepU;
		if (type2 == INTERP_LINEAR)
			pY = linearInterpolate(point1.Y, point2.Y, u);
		else if (type2 == INTERP_COSINE)
			pY = cosineInterpolate(point1.Y, point2.Y, u);
		else if (type2 == INTERP_CUBIC)
			pY = cubicInterpolate(point0.Y, point1.Y, point2.Y, point3.Y, u);
		glVertex3f(pX, pY, pZ);
	}
	glEnd();
}

void drawObject() 
{
	glPushMatrix();
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);
	glColor3f(0.0f, 1.0f, 0.0f);
	
	Vec3 point0 = Vec3(-300.0f, -200.0f, 0.0f); 
	Vec3 point1 = Vec3(-200.0f, -100.0f, 0.0f);
	Vec3 point2 = Vec3( 200.0f, 150.0f, 0.0f);
	Vec3 point3 = Vec3( 300.0f, 250.0f, 0.0f);

	glPopMatrix();
	glPopMatrix();
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
	drawObject();
	glutSwapBuffers();
}

void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0); 
	glEnable(GL_DEPTH_TEST);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((GLfloat)-SCREEN_WIDTH / 2, (GLfloat)SCREEN_WIDTH / 2, (GLfloat)-SCREEN_HEIGHT / 2, (GLfloat)SCREEN_HEIGHT / 2, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho((GLfloat)-w / 2, (GLfloat)w / 2, (GLfloat)-h / 2, (GLfloat)h / 2, 1.0, 100.0);
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

void displayPointsLinear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

    Vec3 point0 = Vec3(-300.0f, -200.0f, 0.0f);
    Vec3 point1 = Vec3(-200.0f, -100.0f, 0.0f);
    Vec3 point2 = Vec3(200.0f, 150.0f, 0.0f);
    Vec3 point3 = Vec3(300.0f, 250.0f, 0.0f);
    drawInterpolation(point0, point1, point2, point3, 10, INTERP_POINTS, INTERP_LINEAR);

    glutSwapBuffers();
}

void displayLinesLinear() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

    Vec3 point0 = Vec3(-300.0f, -200.0f, 0.0f);
    Vec3 point1 = Vec3(-200.0f, -100.0f, 0.0f);
    Vec3 point2 = Vec3(200.0f, 150.0f, 0.0f);
    Vec3 point3 = Vec3(300.0f, 250.0f, 0.0f);
    drawInterpolation(point0, point1, point2, point3, 10, INTERP_LINES, INTERP_LINEAR);

    glutSwapBuffers();
}

void displayPointsCosine() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

    Vec3 point0 = Vec3(-300.0f, -200.0f, 0.0f);
    Vec3 point1 = Vec3(-200.0f, -100.0f, 0.0f);
    Vec3 point2 = Vec3(200.0f, 150.0f, 0.0f);
    Vec3 point3 = Vec3(300.0f, 250.0f, 0.0f);
    drawInterpolation(point0, point1, point2, point3, 10, INTERP_POINTS, INTERP_COSINE);

    glutSwapBuffers();
}

void displayLinesCosine() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

    Vec3 point0 = Vec3(-300.0f, -200.0f, 0.0f);
    Vec3 point1 = Vec3(-200.0f, -100.0f, 0.0f);
    Vec3 point2 = Vec3(200.0f, 150.0f, 0.0f);
    Vec3 point3 = Vec3(300.0f, 250.0f, 0.0f);
    drawInterpolation(point0, point1, point2, point3, 10, INTERP_LINES, INTERP_COSINE);

    glutSwapBuffers();
}

void displayCubic() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

    Vec3 point0 = Vec3(-300.0f, -200.0f, 0.0f);
    Vec3 point1 = Vec3(-200.0f, -100.0f, 0.0f);
    Vec3 point2 = Vec3(200.0f, 150.0f, 0.0f);
    Vec3 point3 = Vec3(300.0f, 250.0f, 0.0f);
    drawInterpolation(point0, point1, point2, point3, 10, INTERP_LINES, INTERP_CUBIC);
    drawInterpolation(point0, point1, point2, point3, 10, INTERP_POINTS, INTERP_CUBIC);


    glutSwapBuffers();
}

int main(int argc, char** argv)
{
	glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

    glutInitWindowSize(480, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Points Linear - MOHAMMAD FARID HENDIANTO || 2200018401  - BAB 3 PRAKTIKUM GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(displayPointsLinear);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);

    glutInitWindowSize(480, 480);
    glutInitWindowPosition(600, 100);
    glutCreateWindow("Lines Linear - MOHAMMAD FARID HENDIANTO || 2200018401  - BAB 3 PRAKTIKUM GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(displayLinesLinear);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);

    glutInitWindowSize(480, 480);
    glutInitWindowPosition(100, 500);
    glutCreateWindow("Points Cosine - MOHAMMAD FARID HENDIANTO || 2200018401  - BAB 3 PRAKTIKUM GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(displayPointsCosine);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);

    glutInitWindowSize(480, 480);
    glutInitWindowPosition(600, 500);
    glutCreateWindow("Lines Cosine - MOHAMMAD FARID HENDIANTO || 2200018401  - BAB 3 PRAKTIKUM GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(displayLinesCosine);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);

    glutInitWindowSize(480, 480);
    glutInitWindowPosition(1100, 250);
    glutCreateWindow("Cubic - MOHAMMAD FARID HENDIANTO || 2200018401  - BAB 3 PRAKTIKUM GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(displayCubic);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);

	glutMainLoop();
	return 0;
}