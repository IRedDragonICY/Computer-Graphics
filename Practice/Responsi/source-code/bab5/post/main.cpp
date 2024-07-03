#include <windows.h>
#include <GL/glut.h>

#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

class Camera {
public:
    float posX, posY, posZ;
    float rotX, rotY, rotZ;

    Camera() : posX(0.0f), posY(0.0f), posZ(5.0f),
               rotX(0.0f), rotY(0.0f), rotZ(-1.0f) {}

    void LookAt() {
        gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);
    }

    void Move(float fraction, int key) {
        switch (key) {
            case GLUT_KEY_PAGE_UP:
                posX += rotX * fraction;
                posZ += rotZ * fraction;
                break;
            case GLUT_KEY_PAGE_DOWN:
                posX -= rotX * fraction;
                posZ -= rotZ * fraction;
                break;
        }
    }
};

class Object {
public:
    float angleX, angleY, angleZ;
    float posX, posY, posZ;
    float color[3];

    Object() : angleX(0.0f), angleY(0.0f), angleZ(0.0f),
               posX(0.0f), posY(0.0f), posZ(0.0f) {}

    virtual void Draw() = 0;

    void Rotate(float angle, char axis) {
        switch (axis) {
            case 'x': angleX += angle; break;
            case 'y': angleY += angle; break;
            case 'z': angleZ += angle; break;
        }
    }

    void SetColor(float r, float g, float b) {
        color[0] = r;
        color[1] = g;
        color[2] = b;
    }
};

class Teapot : public Object {
public:
    void Draw() override {
        glPushMatrix();
        glTranslatef(posX, posY, posZ); 
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glColor3fv(color);
        glutSolidTeapot(0.5f);
        glPopMatrix();
    }
};

class Cube : public Object {
public:
    void Draw() override {
        glPushMatrix();
        glTranslatef(posX, posY, posZ); 
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glColor3fv(color);
        glutSolidCube(0.5f);
        glPopMatrix();
    }
};

class Cone : public Object {
public:
    void Draw() override {
        glPushMatrix();
        glTranslatef(posX, posY, posZ); 
        glRotatef(angleY, 0.0f, 1.0f, 0.0f);
        glRotatef(angleX, 1.0f, 0.0f, 0.0f);
        glColor3fv(color);
        glutSolidCone(0.5f, 1.0f, 32, 32); 
        glPopMatrix();
    }
};


class Projection {
public:
    virtual void Setup(int w, int h) = 0;
    virtual void Init() = 0;
};

class PerspectiveProjection : public Projection {
    float position[4];

public:
    PerspectiveProjection() : position{0.0f, 5.0f, 5.0f, 1.0f} {}

    void Setup(int w, int h) override {
        glViewport(0, 0, (GLsizei)w, (GLsizei)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
    }

    void Init() override {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
    }
};

class OrthoProjection : public Projection {
    float position[4];

public:
    OrthoProjection() : position{0.0f, 100.0f, -100.0f, 1.0f} {}

    void Setup(int w, int h) override {
        glViewport(0, 0, (GLsizei)w, (GLsizei)h);
        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        glOrtho(-w / 2, w / 2, -h / 2, h / 2, 1.0, 100.0);
        glMatrixMode(GL_MODELVIEW);
    }

    void Init() override {
        glClearColor(0.0, 0.0, 0.0, 0.0); 
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_LIGHTING);
        glEnable(GL_COLOR_MATERIAL);
        glEnable(GL_LIGHT0);
        glLightfv(GL_LIGHT0, GL_POSITION, position);
    }
};

//---------------------------------
// Global objects
Camera camera;
PerspectiveProjection perspectiveProjection;
OrthoProjection orthoProjection;

Teapot teapotPerspective;
Cube cubeOrtho;
Teapot teapotOrtho;
Cone conePerspective;
//---------------------------------

void displayPerspectiveTopLeft() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera.LookAt();

    teapotPerspective.Draw();

    glutSwapBuffers();
}

void displayOrthoTopRight() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera.LookAt();

    glPushMatrix();
    glScalef(50.0f, 50.0f, 50.0f);
    cubeOrtho.Draw();
    glPopMatrix();

    glutSwapBuffers();
}

void displayOrthoBottomLeft() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera.LookAt();

    glPushMatrix();
    glScalef(50.0f, 50.0f, 50.0f);
    teapotOrtho.Draw();
    glPopMatrix();

    glutSwapBuffers();
}

void displayPerspectiveBottomRight() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    camera.LookAt();

    conePerspective.Draw();

    glutSwapBuffers();
}

void reshapePerspective(int w, int h) { perspectiveProjection.Setup(w, h); }
void reshapeOrtho(int w, int h) { orthoProjection.Setup(w, h); }

void keyboard(int key, int x, int y) {
    switch (key) {
        case GLUT_KEY_LEFT:
            teapotPerspective.Rotate(-1.0f, 'y');
            cubeOrtho.Rotate(-1.0f, 'y');
            teapotOrtho.Rotate(-1.0f, 'y'); 
            conePerspective.Rotate(-1.0f, 'y'); 
            break;
        case GLUT_KEY_RIGHT:
            teapotPerspective.Rotate(1.0f, 'y');
            cubeOrtho.Rotate(1.0f, 'y');
            teapotOrtho.Rotate(1.0f, 'y'); 
            conePerspective.Rotate(1.0f, 'y'); 
            break;
        case GLUT_KEY_UP:
            teapotPerspective.Rotate(-1.0f, 'x');
            cubeOrtho.Rotate(-1.0f, 'x');
            teapotOrtho.Rotate(-1.0f, 'x'); 
            conePerspective.Rotate(-1.0f, 'x'); 
            break;
        case GLUT_KEY_DOWN:
            teapotPerspective.Rotate(1.0f, 'x');
            cubeOrtho.Rotate(1.0f, 'x');
            teapotOrtho.Rotate(1.0f, 'x'); 
            conePerspective.Rotate(1.0f, 'x'); 
            break;
        case GLUT_KEY_PAGE_UP:
            camera.Move(0.1f, key);
            break;
        case GLUT_KEY_PAGE_DOWN:
            camera.Move(0.1f, key);
            break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Perspective Projection - Teapot");
    perspectiveProjection.Init();
    glutDisplayFunc(displayPerspectiveTopLeft);
    glutReshapeFunc(reshapePerspective);
    glutSpecialFunc(keyboard);

    teapotPerspective.SetColor(1.0f, 1.0f, 1.0f);


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(600, 100);
    glutCreateWindow("Orthogonal Projection - Blue Cube");
    orthoProjection.Init();
    glutDisplayFunc(displayOrthoTopRight);
    glutReshapeFunc(reshapeOrtho);
    glutSpecialFunc(keyboard);

    cubeOrtho.SetColor(0.0f, 0.0f, 1.0f); 


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 500);
    glutCreateWindow("Orthogonal Projection - Teapot (Rotated)");
    orthoProjection.Init();
    glutDisplayFunc(displayOrthoBottomLeft);
    glutReshapeFunc(reshapeOrtho);
    glutSpecialFunc(keyboard);

    teapotOrtho.Rotate(90.0f, 'y'); 
    teapotOrtho.SetColor(1.0f, 1.0f, 1.0f);


    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(600, 500); 
    glutCreateWindow("Perspective Projection - Cone");
    perspectiveProjection.Init();
    glutDisplayFunc(displayPerspectiveBottomRight);
    glutReshapeFunc(reshapePerspective);
    glutSpecialFunc(keyboard);

    conePerspective.SetColor(1.0f, 1.0f, 1.0f); 

    glutMainLoop();
    return 0;
}