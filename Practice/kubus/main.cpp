#include <GL/glut.h>

void display() {
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);
    glVertex2f(-0.5f, -0.5f);    // Titik kiri bawah
    glVertex2f(0.5f, -0.5f);     // Titik kanan bawah
    glVertex2f(0.5f, 0.5f);      // Titik kanan atas
    glVertex2f(-0.5f, 0.5f);     // Titik kiri atas
    glEnd();

    glFlush();
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutCreateWindow("Segiempat OpenGL");
    glutDisplayFunc(display);
    glutMainLoop();
    return 0;
}
