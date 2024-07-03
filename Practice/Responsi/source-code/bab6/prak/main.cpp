#include <windows.h>
#include <glut.h>
#include <cmath>

struct Vec3 {
  float x, y, z;

  Vec3(float x = 0.0f, float y = 0.0f, float z = 0.0f)
      : x(x), y(y), z(z) {}
};

class Camera {
 public:
  Vec3 position;
  Vec3 rotation;

  Camera(const Vec3& position = Vec3(0.0f, 0.0f, 5.0f),
         const Vec3& rotation = Vec3(0.0f, 0.0f, -1.0f))
      : position(position), rotation(rotation) {}

  void LookAt() const {
    gluLookAt(position.x, position.y, position.z,
              position.x + rotation.x, position.y + rotation.y,
              position.z + rotation.z, 0.0f, 1.0f, 0.0f);
  }

  void Move(float dx, float dy, float dz) {
    position.x += dx;
    position.y += dy;
    position.z += dz;
  }

  void Rotate(float angle, float x, float y, float z) {
    rotation.x += angle * x;
    rotation.y += angle * y;
    rotation.z += angle * z;
  }
};

class Object {
 public:
  Vec3 position;
  Vec3 rotation;
  Vec3 color;

  Object(const Vec3& position = Vec3(),
         const Vec3& rotation = Vec3(),
         const Vec3& color = Vec3(0.54f, 0.27f, 0.07f)) // Coklat
      : position(position), rotation(rotation), color(color) {}

  virtual void Draw() const = 0;

  void Rotate(float angle, float x, float y, float z) {
    rotation.x += angle * x;
    rotation.y += angle * y;
    rotation.z += angle * z;
  }
};

class Torus : public Object {
 public:
  float innerRadius;
  float outerRadius;
  int sides;
  int rings;

  Torus(float innerRadius = 0.5f,
        float outerRadius = 1.0f,
        int sides = 30,
        int rings = 30,
        const Vec3& position = Vec3(),
        const Vec3& rotation = Vec3(),
        const Vec3& color = Vec3(0.54f, 0.27f, 0.07f)) // Coklat
      : Object(position, rotation, color),
        innerRadius(innerRadius),
        outerRadius(outerRadius),
        sides(sides),
        rings(rings) {}

  void Draw() const override {
    glPushMatrix();
    glTranslatef(position.x, position.y, position.z);
    glRotatef(rotation.x, 1.0f, 0.0f, 0.0f);
    glRotatef(rotation.y, 0.0f, 1.0f, 0.0f);
    glRotatef(rotation.z, 0.0f, 0.0f, 1.0f);
    glColor3f(color.x, color.y, color.z);

    for (int i = 0; i < rings; ++i) {
      float theta1 = (i * 2 * M_PI) / rings;
      float theta2 = ((i + 1) * 2 * M_PI) / rings;

      glBegin(GL_QUAD_STRIP);
      for (int j = 0; j <= sides; ++j) {
        float phi = (j * 2 * M_PI) / sides;

        float x = (outerRadius + innerRadius * cos(phi)) * cos(theta2);
        float y = (outerRadius + innerRadius * cos(phi)) * sin(theta2);
        float z = innerRadius * sin(phi);
        glNormal3f(x, y, z);
        glVertex3f(x, y, z);

        x = (outerRadius + innerRadius * cos(phi)) * cos(theta1);
        y = (outerRadius + innerRadius * cos(phi)) * sin(theta1);
        glNormal3f(x, y, z);
        glVertex3f(x, y, z);
      }
      glEnd();
    }
    glPopMatrix();
  }
};

Camera camera;
Torus torus;

void Display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();

  camera.LookAt();

  torus.Draw();

  glutSwapBuffers();
}

void Reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
}

void Keyboard(int key, int x, int y) {
  float fraction = 0.1f;
  float rotationSpeed = 2.0f;

  switch (key) {
    case GLUT_KEY_LEFT:
      torus.Rotate(rotationSpeed, 0.0f, 1.0f, 0.0f);
      break;
    case GLUT_KEY_RIGHT:
      torus.Rotate(-rotationSpeed, 0.0f, 1.0f, 0.0f);
      break;
    case GLUT_KEY_UP:
      torus.Rotate(rotationSpeed, 1.0f, 0.0f, 0.0f);
      break;
    case GLUT_KEY_DOWN:
      torus.Rotate(-rotationSpeed, 1.0f, 0.0f, 0.0f);
      break;
    case GLUT_KEY_PAGE_UP:
      camera.Move(camera.rotation.x * fraction,
                  camera.rotation.y * fraction,
                  camera.rotation.z * fraction);
      break;
    case GLUT_KEY_PAGE_DOWN:
      camera.Move(-camera.rotation.x * fraction,
                  -camera.rotation.y * fraction,
                  -camera.rotation.z * fraction);
      break;
  }
  glutPostRedisplay();
}

void Init() {
  glClearColor(1.0, 1.0, 1.0, 0.0);
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(45.0, 1.0, 1.0, 100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
}

int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(480, 480);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 6 GRAFIKA KOMPUTER");

  Init();

  glutDisplayFunc(Display);
  glutReshapeFunc(Reshape);
  glutSpecialFunc(Keyboard);

  glutMainLoop();

  return 0;
}