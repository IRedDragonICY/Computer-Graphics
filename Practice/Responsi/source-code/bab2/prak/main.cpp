#include <GL/glut.h>
#include <cmath>
#include <vector>

const int SCREEN_WIDTH = 400;
const int SCREEN_HEIGHT = 400;

struct Vec3 {
    float x, y, z;
    Vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {}
};

class Line {
public:
    virtual void draw(const Vec3& p1, const Vec3& p2) = 0;
};

class DDALine : public Line {
public:
    void draw(const Vec3& p1, const Vec3& p2) override {
        float dx = p2.x - p1.x, dy = p2.y - p1.y;
        float steps = std::fmax(std::fabs(dx), std::fabs(dy));
        float xInc = dx / steps, yInc = dy / steps;

        glBegin(GL_POINTS);
        for (float x = p1.x, y = p1.y, i = 0; i <= steps; i++, x += xInc, y += yInc) {
            glVertex3f(x, y, 0);
        }
        glEnd();
    }
};

class BresenhamLine : public Line {
public:
    void draw(const Vec3& p1, const Vec3& p2) override {
        int x = p1.x, y = p1.y;
        int dx = abs(p2.x - p1.x), dy = abs(p2.y - p1.y);
        int sx = p1.x < p2.x ? 1 : -1, sy = p1.y < p2.y ? 1 : -1;
        int err = (dx > dy ? dx : -dy) / 2;

        glBegin(GL_POINTS);
        while (true) {
            glVertex3f(x, y, 0);
            if (x == p2.x && y == p2.y) break;
            int e2 = err;
            if (e2 > -dx) { err -= dy; x += sx; }
            if (e2 < dy) { err += dx; y += sy; }
        }
        glEnd();
    }
};

class Object {
public:
    float angleX = 0, angleY = 0;
    std::vector<Vec3> points;
    Line* line;

    Object(Line* line, const std::vector<Vec3>& points) : line(line), points(points) {}

    void draw() {
        glPushMatrix();
        glRotatef(angleY, 0, 1, 0);
        glRotatef(angleX, 1, 0, 0);
        glColor3f(1, 0, 0);

        for (size_t i = 0; i < points.size() - 1; i += 2) {
            line->draw(points[i], points[i + 1]);
            line->draw(Vec3(-points[i].x, points[i].y), Vec3(-points[i + 1].x, points[i + 1].y));
        }

        glPopMatrix();
    }
};

float camPosX = 0, camPosY = 0, camPosZ = 5;
float camRotX = 0, camRotY = 0, camRotZ = -1;

Object* ddaObject;
Object* bresenhamObject;

void displayDDA() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ, camPosX + camRotX, camPosY + camRotY, camPosZ + camRotZ, 0, 1, 0);
    ddaObject->draw();
    glutSwapBuffers();
}

void displayBresenham() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(camPosX, camPosY, camPosZ, camPosX + camRotX, camPosY + camRotY, camPosZ + camRotZ, 0, 1, 0);
    bresenhamObject->draw();
    glutSwapBuffers();
}

void init() {
    glClearColor(0, 0, 0, 0);
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-SCREEN_WIDTH / 2, SCREEN_WIDTH / 2, -SCREEN_HEIGHT / 2, SCREEN_HEIGHT / 2, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void reshape(int w, int h) {
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-w / 2, w / 2, -h / 2, h / 2, 1, 100);
    glMatrixMode(GL_MODELVIEW);
}

void keyboard(int key, int x, int y) {
    float fraction = 0.1f;

    switch (key) {
        case GLUT_KEY_LEFT: ddaObject->angleY -= 1; bresenhamObject->angleY -= 1; break;
        case GLUT_KEY_RIGHT: ddaObject->angleY += 1; bresenhamObject->angleY += 1; break;
        case GLUT_KEY_UP: ddaObject->angleX -= 1; bresenhamObject->angleX -= 1; break;
        case GLUT_KEY_DOWN: ddaObject->angleX += 1; bresenhamObject->angleX += 1; break;
        case GLUT_KEY_PAGE_UP: camPosX += camRotX * fraction; camPosZ += camRotZ * fraction; break;
        case GLUT_KEY_PAGE_DOWN: camPosX -= camRotX * fraction; camPosZ -= camRotZ * fraction; break;
    }
    glutPostRedisplay();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);

 std::vector<Vec3> points = {
        Vec3(300, 0), Vec3(-300, 0),  
        Vec3(0, 300), Vec3(0, -300),
        Vec3(200, 120), Vec3(100, 100), 
        Vec3(-120, 200), Vec3(-100, 100), 
        Vec3(-200, -120), Vec3(-100, -100), 
        Vec3(120, -200), Vec3(100, -100)  
    };

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(100, 100);
    glutCreateWindow("Algoritma DDA - MOHAMMAD FARID HENDIANTO || 2200018401  - BAB 2 PRAKTIKUM GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(displayDDA);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    ddaObject = new Object(new DDALine(), points);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
    glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
    glutInitWindowPosition(600, 100);
    glutCreateWindow("Algoritma Bresenham - MOHAMMAD FARID HENDIANTO || 2200018401  - BAB 2 PRAKTIKUM GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(displayBresenham);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    bresenhamObject = new Object(new BresenhamLine(), points);

    glutMainLoop();

    delete ddaObject;
    delete bresenhamObject;

    return 0;
}