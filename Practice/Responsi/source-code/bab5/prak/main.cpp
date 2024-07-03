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

  Object() : angleX(0.0f), angleY(0.0f), angleZ(0.0f) {}

  virtual void Draw() = 0; 

  void Rotate(float angle, char axis) {
    switch (axis) {
      case 'x': angleX += angle; break;
      case 'y': angleY += angle; break;
      case 'z': angleZ += angle; break;
    }
  }
};

class Dodecahedron : public Object {
public:
  void Draw() override {
    glPushMatrix();
    glRotatef(angleY, 0.0f, 1.0f, 0.0f);
    glPushMatrix();
    glRotatef(angleX, 1.0f, 0.0f, 0.0f);
    glColor3f(1.0f, 1.0f, 1.0f);
    glutSolidDodecahedron();
    glPopMatrix();
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
    glClearColor(1.0, 1.0, 1.0, 0.0);
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
    glClearColor(1.0, 1.0, 1.0, 0.0);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHT0);
    glLightfv(GL_LIGHT0, GL_POSITION, position);
  }
};

Camera camera;
Dodecahedron dodecahedron;
PerspectiveProjection perspectiveProjection;
OrthoProjection orthoProjection;

void displayPerspective() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  camera.LookAt();
  dodecahedron.Draw();
  glutSwapBuffers();
}

void displayOrtho() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  camera.LookAt();
  glPushMatrix();
  glScalef(50.0f, 50.0f, 50.0f); 
  dodecahedron.Draw();
  glPopMatrix();
  glutSwapBuffers();
}


void reshapePerspective(int w, int h) { perspectiveProjection.Setup(w, h); }
void reshapeOrtho(int w, int h) { orthoProjection.Setup(w, h); }

void keyboard(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT:    dodecahedron.Rotate(-1.0f, 'y'); break;
    case GLUT_KEY_RIGHT:   dodecahedron.Rotate( 1.0f, 'y'); break;
    case GLUT_KEY_UP:      dodecahedron.Rotate(-1.0f, 'x'); break;
    case GLUT_KEY_DOWN:    dodecahedron.Rotate( 1.0f, 'x'); break;
    case GLUT_KEY_PAGE_UP:   camera.Move(0.1f, key); break;
    case GLUT_KEY_PAGE_DOWN: camera.Move(0.1f, key); break;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("Perspective Projection - MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 4 GRAFIKA KOMPUTER");
  perspectiveProjection.Init();
  glutDisplayFunc(displayPerspective);
  glutReshapeFunc(reshapePerspective);
  glutSpecialFunc(keyboard);

  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
  glutInitWindowPosition(600, 100);
  glutCreateWindow("Orthogonal Projection - MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 4 GRAFIKA KOMPUTER");
  orthoProjection.Init();
  glutDisplayFunc(displayOrtho);
  glutReshapeFunc(reshapeOrtho);
  glutSpecialFunc(keyboard);

  glutMainLoop();
  return 0;
}