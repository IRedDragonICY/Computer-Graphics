#include <GL/glut.h>

struct Vec3 {
  float x, y, z;
  Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
  Vec3 operator+(const Vec3& v) const { return Vec3(x + v.x, y + v.y, z + v.z); }
  Vec3 operator-(const Vec3& v) const { return Vec3(x - v.x, y - v.y, z - v.z); }
  Vec3 operator*(float s) const { return Vec3(x * s, y * s, z * s); }
};

class Camera {
public:
  Vec3 pos, rot;
  Camera(const Vec3& pos = Vec3(0, 0, 5), const Vec3& rot = Vec3(0, 0, -1)) : pos(pos), rot(rot) {}
  void move(float amount) { pos = pos + rot * amount; }
  void lookAt() { gluLookAt(pos.x, pos.y, pos.z, pos.x + rot.x, pos.y + rot.y, pos.z + rot.z, 0, 1, 0); }
};

class Object {
public:
  Vec3 rot;
  virtual void draw() = 0;
  void rotate(float x, float y) { rot.x += x; rot.y += y; }
};

class Dodecahedron : public Object {
public:
  void draw() override {
    glPushMatrix();
    glRotatef(rot.y, 0, 1, 0);
    glRotatef(rot.x, 1, 0, 0);
    glColor3f(0, 0, 0);
    glutWireDodecahedron();
    glPopMatrix();
  }
};

Camera camera;
Object* object = new Dodecahedron();

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  camera.lookAt();
  object->draw();
  glutSwapBuffers();
}

void init() {
  glClearColor(1, 1, 1, 0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  gluPerspective(45, 1, 1, 100);
  glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, (float)w / h, 1, 100);
  glMatrixMode(GL_MODELVIEW);
}

void keyboard(int key, int x, int y) {
  switch (key) {
    case GLUT_KEY_LEFT: object->rotate(0, -1); break;
    case GLUT_KEY_RIGHT: object->rotate(0, 1); break;
    case GLUT_KEY_UP: object->rotate(-1, 0); break;
    case GLUT_KEY_DOWN: object->rotate(1, 0); break;
    case GLUT_KEY_PAGE_UP: camera.move(0.1f); break;
    case GLUT_KEY_PAGE_DOWN: camera.move(-0.1f); break;
  }
  glutPostRedisplay();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(480, 480);
  glutCreateWindow("MOHAMMAD FARID HENDIANTO || 2200018401  - PERTEMUAN 1 PRAKTIKUM GRAFIKA KOMPUTER");
  init();
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutSpecialFunc(keyboard);
  glutMainLoop();
  delete object;
  return 0;
}