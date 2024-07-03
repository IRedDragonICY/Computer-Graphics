#include <windows.h>
#include <math.h>
#include <glut.h>

float angle = 0.0f;
float posX = 0.0f, rotX =  0.0f;
float posY = 0.0f, rotY =  0.0f;
float posZ = 5.0f, rotZ = -1.0f;

float objectAngleX = 0.0f;
float objectAngleY = 0.0f;
float objectAngleZ = 0.0f;

static GLfloat vdata[4][3] = {
    {0.0f, 1.0f, 0.0f},
    {-1.0f, -1.0f, 1.0f},
    {1.0f, -1.0f, 1.0f},
    {0.0f, -1.0f, -1.0f}
};

static int tindices[4][3] = {
    {0, 1, 2},
    {0, 2, 3},
    {0, 3, 1},
    {1, 2, 3}
};

GLfloat mat_specular[] = {0.0, 0.0, 0.0, 1.0};
GLfloat mat_diffuse[] = {1.0, 0.0, 0.0, 1.0};
GLfloat mat_ambient[] = {1.0, 0.0, 0.0, 1.0};
GLfloat mat_shininess = 100.0;

GLfloat light_ambient[] = {0.2, 0.2, 0.2, 1.0};
GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_specular[] = {0.0, 0.0, 0.0, 1.0};

GLfloat light_position1[] = {1.5, 1.0, -2.0, 0.0};
GLfloat light_position2[] = {1.5, 1.0, 2.0, 0.0};

int flat = 1;
int subdiv = 0;

struct Vec3 {
    float X; float Y; float Z;
    Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
    Vec3() {}
    ~Vec3() {}
};

Vec3 normalize(Vec3 value) {
    Vec3 result;
    float lengths = sqrt((value.X * value.X) + (value.Y * value.Y) + (value.Z * value.Z));
    result.X = value.X / lengths;
    result.Y = value.Y / lengths;
    result.Z = value.Z / lengths;
    return result;
}

Vec3 cross(Vec3 value1, Vec3 value2) {
    Vec3 result;
    result.X = value1.Y * value2.Z - value2.Y * value1.Z;
    result.Y = value1.Z * value2.X - value2.Z * value1.X;
    result.Z = value1.X * value2.Y - value2.X * value1.Y;
    return result;
}

void normface(Vec3 v1, Vec3 v2, Vec3 v3) {
    Vec3 d1, d2;
    d1.X = v1.X - v2.X; d1.Y = v1.Y - v2.Y; d1.Z = v1.Z - v2.Z;
    d2.X = v2.X - v3.X; d2.Y = v2.Y - v3.Y; d2.Z = v2.Z - v3.Z;
    Vec3 tn = cross(d1, d2);
    tn = normalize(tn);
    glNormal3f(tn.X, tn.Y, tn.Z);
}

void drawTriangleFlat(Vec3 v1, Vec3 v2, Vec3 v3) {
    glBegin(GL_TRIANGLES);
    normface(v1, v2, v3);
    glVertex3f(v1.X, v1.Y, v1.Z);
    glVertex3f(v2.X, v2.Y, v2.Z);
    glVertex3f(v3.X, v3.Y, v3.Z);
    glEnd();
}

void drawTriangleSmooth(Vec3 v1, Vec3 v2, Vec3 v3) {
    glBegin(GL_TRIANGLES);
    glNormal3f(v1.X, v1.Y, v1.Z);
    glVertex3f(v1.X, v1.Y, v1.Z);
    glNormal3f(v2.X, v2.Y, v2.Z);
    glVertex3f(v2.X, v2.Y, v2.Z);
    glNormal3f(v3.X, v3.Y, v3.Z);
    glVertex3f(v3.X, v3.Y, v3.Z);
    glEnd();
}

void subdivide(Vec3 &v1, Vec3 &v2, Vec3 &v3, int depth) {
    Vec3 v12, v23, v31;
    if (depth == 0) {
        if (flat == 1) 
            drawTriangleFlat(v1, v2, v3);
        else 
            drawTriangleSmooth(v1, v2, v3);
        return;
    }
    v12.X = (v1.X + v2.X) / 2.0; v12.Y = (v1.Y + v2.Y) / 2.0; v12.Z = (v1.Z + v2.Z) / 2.0;
    v23.X = (v2.X + v3.X) / 2.0; v23.Y = (v2.Y + v3.Y) / 2.0; v23.Z = (v2.Z + v3.Z) / 2.0;
    v31.X = (v3.X + v1.X) / 2.0; v31.Y = (v3.Y + v1.Y) / 2.0; v31.Z = (v3.Z + v1.Z) / 2.0;

    v12 = normalize(v12);
    v23 = normalize(v23);
    v31 = normalize(v31);

    subdivide(v1, v12, v31, depth - 1);
    subdivide(v2, v23, v12, depth - 1);
    subdivide(v3, v31, v23, depth - 1);
    subdivide(v12, v23, v31, depth - 1);
}

void drawObject() {
    glPushMatrix();
    glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
    glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

    glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);

    glPushMatrix();
    glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

    for (int i = 0; i < 4; i++) { // Ubah iterasi menjadi 4 (jumlah sisi tetrahedron)
        Vec3 vdata1 = Vec3(
            vdata[tindices[i][0]][0],
            vdata[tindices[i][0]][1],
            vdata[tindices[i][0]][2]);
        Vec3 vdata2 = Vec3(
            vdata[tindices[i][1]][0],
            vdata[tindices[i][1]][1],
            vdata[tindices[i][1]][2]);
        Vec3 vdata3 = Vec3(
            vdata[tindices[i][2]][0],
            vdata[tindices[i][2]][1],
            vdata[tindices[i][2]][2]);
        subdivide(vdata1, vdata2, vdata3, subdiv);
        vdata[tindices[i][0]][0] = vdata1.X;
        vdata[tindices[i][0]][1] = vdata1.Y;
        vdata[tindices[i][0]][2] = vdata1.Z;
        vdata[tindices[i][1]][0] = vdata2.X;
        vdata[tindices[i][1]][1] = vdata2.Y;
        vdata[tindices[i][1]][2] = vdata2.Z;
        vdata[tindices[i][2]][0] = vdata3.X;
        vdata[tindices[i][2]][1] = vdata3.Y;
        vdata[tindices[i][2]][2] = vdata3.Z;
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

void init(void) {
    glClearColor(0.0, 0.0, 0.0, 0.0); 
    glEnable(GL_DEPTH_TEST);
    glMatrixMode(GL_PROJECTION);		
    glLoadIdentity();
    gluPerspective(45.0, 1.0, 1.0, 100.0);
    glMatrixMode(GL_MODELVIEW);				
    glLoadIdentity();						
    gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

    glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
    glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
    glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
    glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
    glMaterialf(GL_LIGHT0, GL_SHININESS, mat_shininess);
    glMaterialf(GL_LIGHT1, GL_SHININESS, mat_shininess);

    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
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
        case GLUT_KEY_F1:
            subdiv++;
            glutPostRedisplay();
            break;
        case GLUT_KEY_F2:
            subdiv--;
            glutPostRedisplay();
            break;
    }
    if (subdiv<0) subdiv = 0;
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
    glutCreateWindow("MOHAMMAD FARID HENDIANTO || 2200018401 - BAB 10 GRAFIKA KOMPUTER");
    init();
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutSpecialFunc(keyboard);
    glutMainLoop();
    return 0;
}