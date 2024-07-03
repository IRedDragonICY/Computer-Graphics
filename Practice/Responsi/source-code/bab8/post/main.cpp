#include <windows.h>
#include "glut.h"

float angle = 0.0f, posX = 0.0f, rotX = 0.0f, posY = 0.0f, rotY = 0.0f, posZ = 5.0f, rotZ = -1.0f;
float objectAngleX = 0.0f, objectAngleY = 0.0f, objectAngleZ = 0.0f;

struct Vec3 {
    float X, Y, Z;
    Vec3(float x, float y, float z) : X(x), Y(y), Z(z) {}
    Vec3() {}
    ~Vec3() {}
};

Vec3 controlPoint[4][4];
bool showPoints = false;

GLfloat mat_diffuse[] = {1.0f, 1.0f, 0.0f, 1.0f};
GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
GLfloat mat_shininess[] = {100.0f};

GLUnurbsObj* theNurb;

void drawObject() {
    glPushMatrix();
    glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);
    glScalef(0.5f, 0.5f, 0.5f);



    GLfloat knots[8] = {0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f};
    gluBeginSurface(theNurb);
    gluNurbsSurface(theNurb, 8, knots, 8, knots, 4 * 3, 3, &controlPoint[0][0].X, 4, 4, GL_MAP2_VERTEX_3);
    gluEndSurface(theNurb);

    if (showPoints) {
        glPointSize(5.0f);
        glDisable(GL_LIGHTING);
        glColor3f(1.0f, 1.0f, 0.0f);
        glBegin(GL_POINTS);
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 4; j++) {
                glVertex3f(controlPoint[i][j].X, controlPoint[i][j].Y, controlPoint[i][j].Z);
            }
        }
        glEnd();
        glEnable(GL_LIGHTING);
    }

    glPopMatrix();
    glPopMatrix();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
    drawObject();
    glutSwapBuffers();
}

void init() {
    glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, 1.0f, 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);

    for (int u = 0; u < 4; u++) {
        for (int v = 0; v < 4; v++) {
            controlPoint[u][v].X = 2.0f * static_cast<float>(u) - 3.0f;
            controlPoint[u][v].Y = 2.0f * static_cast<float>(v) - 3.0f;
            controlPoint[u][v].Z =  (v == 1 || v == 2) ? 3.0f : -3.0f;
        }
    }

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_AUTO_NORMAL);
    glEnable(GL_NORMALIZE);

    theNurb = gluNewNurbsRenderer();
    gluNurbsProperty(theNurb, GLU_SAMPLING_TOLERANCE, 25.0);
    gluNurbsProperty(theNurb, GLU_DISPLAY_MODE, GLU_FILL);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, static_cast<float>(w) / static_cast<float>(h), 1.0f, 100.0f);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(int key, int x, int y) {
    float fraction = 0.1f;
    switch (key) {
        case GLUT_KEY_LEFT:
            objectAngleY -= 1.0f;
            break;
        case GLUT_KEY_RIGHT:
            objectAngleY += 1.0f;
            break;
        case GLUT_KEY_UP:
            objectAngleX -= 1.0f;
            break;
        case GLUT_KEY_DOWN:
            objectAngleX += 1.0f;
            break;
        case GLUT_KEY_PAGE_UP:
            posX += rotX * fraction;
            posZ += rotZ * fraction;
            break;
        case GLUT_KEY_PAGE_DOWN:
            posX -= rotX * fraction;
            posZ -= rotZ * fraction;
            break;
        case GLUT_KEY_F1:
            showPoints = !showPoints;
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(480, 480);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 8 GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutMainLoop();
    return 0;
}