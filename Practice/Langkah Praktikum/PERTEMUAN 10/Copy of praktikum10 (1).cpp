// praktikum 10 grafika komputer
// adhi prahara. 2018

// deklarasikan semua header disini
#include <windows.h>
#include <math.h>
#include <glut.h>

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;					// sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;	// posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;	// posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;	// posisi kamera di sumbu Z

float objectAngleX = 0.0f;			// sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;			// sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;			// sudut tranformasi obyek di sumbu Z

#define vX 0.525731112119133696
#define vZ 0.850650808352039932
											/* vertex data array */
static GLfloat vdata[12][3] = 
{
	{ -vX, 0.0, vZ },{ vX, 0.0, vZ },{ -vX, 0.0, -vZ },{ vX, 0.0, -vZ },
	{ 0.0, vZ, vX },{ 0.0, vZ, -vX },{ 0.0, -vZ, vX },{ 0.0, -vZ, -vX },
	{ vZ, vX, 0.0 },{ -vZ, vX, 0.0 },{ vZ, -vX, 0.0 },{ -vZ, -vX, 0.0 }
};

/* triangle indices */
static int tindices[20][3] = {
	{ 1,4,0 },{ 4,9,0 },{ 4,5,9 },{ 8,5,4 },{ 1,8,4 },
	{ 1,10,8 },{ 10,3,8 },{ 8,3,5 },{ 3,2,5 },{ 3,7,2 },
	{ 3,10,7 },{ 10,6,7 },{ 6,11,7 },{ 6,0,11 },{ 6,1,0 },
	{ 10,1,6 },{ 11,0,9 },{ 2,11,9 },{ 5,2,9 },{ 11,2,7 }
};

GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_shininess = 100.0;	/* unused if specular is 0 */

GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat light_position1[] = { 1.5, 1.0, -2.0, 0.0 }; /* directional */
GLfloat light_position2[] = { 1.5, 1.0, 2.0, 0.0 }; /* directional */

int flat = 1;			/* 0 = smooth shading, 1 = flat shading */
int subdiv = 0;

// container untuk membuat tipe data 3D (X, Y, Z)
struct Vec3
{
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	//
	Vec3() { }
	~Vec3() { }
};

// fungsi untuk melakukan normalisasi koordinat posisi
Vec3 normalize(Vec3 value)
{
	Vec3 result;
	float lengths = sqrt((value.X * value.X) + (value.Y * value.Y)
		+ (value.Z * value.Z));
	result.X = value.X / lengths;
	result.Y = value.Y / lengths;
	result.Z = value.Z / lengths;

	return result;
}

// fungsi untuk melakukan operasi perkalian cross
Vec3 cross(Vec3 value1, Vec3 value2)
{
	Vec3 result;
	result.X = value1.Y * value2.Z - value2.Y * value1.Z;
	result.Y = value1.Z * value2.X - value2.Z * value1.X;
	result.Z = value1.X * value2.Y - value2.X * value1.Y;

	return result;
}

// fungsi untuk menghitung normal
void normface(Vec3 v1, Vec3 v2, Vec3 v3)
{
	Vec3 d1, d2;

	d1.X = v1.X - v2.X; d1.Y = v1.Y - v2.Y; d1.Z = v1.Z - v2.Z;
	d2.X = v2.X - v3.X; d2.Y = v2.Y - v3.Y; d2.Z = v2.Z - v3.Z;

	Vec3 tn = cross(d1, d2);
	tn = normalize(tn);
	glNormal3f(tn.X, tn.Y, tn.Z);
}

/* draw triangle using face normals */
void drawTriangleFlat(Vec3 v1, Vec3 v2, Vec3 v3)
{
	glBegin(GL_TRIANGLES);
	normface(v1, v2, v3);
	glVertex3f(v1.X, v1.Y, v1.Z);
	glVertex3f(v2.X, v2.Y, v2.Z);
	glVertex3f(v3.X, v3.Y, v3.Z);
	glEnd();
}

/* draw triangle using sphere normals */
void drawTriangleSmooth(Vec3 v1, Vec3 v2, Vec3 v3)
{
	glBegin(GL_TRIANGLES);
	glNormal3f(v1.X, v1.Y, v1.Z);
	glVertex3f(v1.X, v1.Y, v1.Z);
	glNormal3f(v2.X, v2.Y, v2.Z);
	glVertex3f(v2.X, v2.Y, v2.Z);
	glNormal3f(v3.X, v3.Y, v3.Z);
	glVertex3f(v3.X, v3.Y, v3.Z);
	glEnd();
}

/* recursively subdivide face `depth' times */
/* and draw the resulting triangles */
void subdivide(Vec3 &v1, Vec3 &v2, Vec3 &v3, int depth)
{
	Vec3 v12, v23, v31;

	if (depth == 0) 
	{
		if (flat == 1)
			drawTriangleFlat(v1, v2, v3);
		else
			drawTriangleSmooth(v1, v2, v3);
		return;
	}

	/* calculate midpoints of each side */
	v12.X = (v1.X + v2.X) / 2.0; v12.Y = (v1.Y + v2.Y) / 2.0; v12.Z = (v1.Z + v2.Z) / 2.0;
	v23.X = (v2.X + v3.X) / 2.0; v23.Y = (v2.Y + v3.Y) / 2.0; v23.Z = (v2.Z + v3.Z) / 2.0;
	v31.X = (v3.X + v1.X) / 2.0; v31.Y = (v3.Y + v1.Y) / 2.0; v31.Z = (v3.Z + v1.Z) / 2.0;

	/* extrude midpoints to lie on unit sphere */
	v12 = normalize(v12);
	v23 = normalize(v23);
	v31 = normalize(v31);

	/* recursively subdivide new triangles */
	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}

// fungsi untuk menggambar obyek kubus
void drawObject()
{
	// obyek bisa dimasukkan diantara glPushMatrix() dan glPopMatrix() 
	// fungsinya agar obyek tidak terpengaruh atau mempengaruhi obyek lain
	// saat diwarnai, ditransformasi dan sebagainya
	glPushMatrix();

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

	// operasi transformasi rotasi obyek ke arah kanan-kiri
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);

	glPushMatrix();

	// operasi transformasi rotasi obyek ke arah atas-bawah
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

	// buat daftar vertex
	for (int i = 0; i < 20; i++) 
	{
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

// taruh semua obyek yang akan digambar di fungsi display()
void display()
{
	// bersihkan dan reset layar dan buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// posisikan kamera pandang
	// dalam hal ini sumbu Y ada diatas dan posisi kamera pandang di (posX, posY, posZ)
	gluLookAt(posX, posY, posZ, posX + rotX, posY + rotY, posZ + rotZ, 0.0f, 1.0f, 0.0f);

	// panggil fungsi untuk menggambar obyek
	drawObject();
	
	// tampilkan obyek ke layar
	// gunakan glFlush() bila memakai single buffer
	// gunakan glutSwapBuffers() bila memakai double buffer
	glutSwapBuffers();
}

// inisialisasikan variabel, pencahayaan, tekstur dan pengaturan kamera pandang di fungsi init()
void init(void)
{
	// inisialisasi warna latar belakang layar dalam hal ini warna putih (1.0, 1.0, 1.0, 0.0)
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glEnable(GL_DEPTH_TEST);				// mengaktifkan depth buffer
	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();
	gluPerspective(45.0, 1.0, 1.0, 100.0);	// set proyeksi ke perspektif
	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();						
	// inisialisasi kamera pandang
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);

	// inisialisasi pencahayaan
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

	glShadeModel(GL_SMOOTH);	/* enable smooth shading */
	glEnable(GL_LIGHTING);	/* enable lighting */
	glEnable(GL_LIGHT0);		/* enable light 0 */
	glEnable(GL_LIGHT1);		/* enable light 0 */
}

// fungsi ini digunakan bila layar akan diresize (default)
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glMatrixMode(GL_MODELVIEW);
}

// fungsi untuk mengatur masukan dari keyboard 
// untuk arah kiri, kanan, atas, bawah, PgUp, dan PgDn
void keyboard(int key, int x, int y)
{
	float fraction = 0.1f;

	switch (key) 
	{
	// masukkan perintah disini bila tombol kiri ditekan
	case GLUT_KEY_LEFT:
		// dalam hal ini perintah rotasi obyek ke kiri sebanyak 1 derajat 
		objectAngleY -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol kanan ditekan
	case GLUT_KEY_RIGHT:		
		// dalam hal ini perintah rotasi obyek ke kanan sebanyak 1 derajat 
		objectAngleY += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol atas ditekan
	case GLUT_KEY_UP:		
		// dalam hal ini perintah rotasi obyek ke atas sebanyak 1 derajat 
		objectAngleX -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
	// masukkan perintah disini bila tombol bawah ditekan
	case GLUT_KEY_DOWN:		
		// dalam hal ini perintah rotasi obyek ke bawah sebanyak 1 derajat 
		objectAngleX += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
		// masukkan perintah disini bila tombol PgUp ditekan
	case GLUT_KEY_PAGE_UP:
		// masukkan perintah disini bila tombol PgUp ditekan
		posX += rotX * fraction;		
		posZ += rotZ * fraction;
		glutPostRedisplay();	// update obyek
		break;
		// masukkan perintah disini bila tombol PgDn ditekan
	case GLUT_KEY_PAGE_DOWN:
		// masukkan perintah disini bila tombol PgDn ditekan
		posX -= rotX * fraction;
		posZ -= rotZ * fraction;
		glutPostRedisplay();	// update obyek
	case GLUT_KEY_F1:
		subdiv++;
		glutPostRedisplay();	// update obyek
		break;
	case GLUT_KEY_F2:
		subdiv--;
		glutPostRedisplay();	// update obyek
		break;
	}

	if (subdiv<0) subdiv = 0;
}

// timer untuk animasi (gunakan bila perlu)
void timer(int value)
{
	glutPostRedisplay();
	glutTimerFunc(55, timer, 0);
}

// program utama
int main(int argc, char** argv)
{
	// inisialisasi jendela OpenGL
	// GLUT_SINGLE berarti memakai single buffer
	// GLUT_DOUBLE berarti memakai double buffer
	// GLUT_RGB berarti mode tampilan yang dipakai RGB
	// GLUT_RGBA berarti mode tampilan yang dipakai RGBA
	// GLUT_DEPTH berarti memakai depth buffer
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);

	// set ukuran jendela tampilan
	glutInitWindowSize(480, 480);		// besarnya jendela dalam piksel dalam hal ini 300x300
	glutInitWindowPosition(100, 100);	// posisi jendela dilayar komputer dalam piksel
	// judul jendela (wajib diubah dengan informasi NAMA / NIM - JUDUL PRAKTIKUM masing-masing)
	glutCreateWindow("NAMA / NIM - PRAKTIKUM 10 GRAFIKA KOMPUTER");
	
	// panggil fungsi init untuk inisialisasi awal
	init();

	// event handler untuk display, reshape dan keyboard
	glutDisplayFunc(display);   // display
	glutReshapeFunc(reshape);   // reshape
	glutSpecialFunc(keyboard);  // keyboard
	//glutTimerFunc(0, timer, 0); // aktifkan timer bila perlu

	// looping
	glutMainLoop();

	return 0;
}