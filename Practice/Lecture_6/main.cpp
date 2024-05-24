// kode dasar untuk semua praktikum grafika komputer
// adhi prahara. 2018

// deklarasikan semua header disini
#include <windows.h>
#include <GL/glut.h>

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;					// sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;	// posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;	// posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;	// posisi kamera di sumbu Z

float objectAngleX = 0.0f;			// sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;			// sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;			// sudut tranformasi obyek di sumbu Z

// container untuk membuat tipe data 3D (X, Y, Z)
struct Vec3
{
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	//
	Vec3() { }
	~Vec3() { }
};

void drawCube()
{
	glBegin(GL_QUADS);
	// beri warna merah di sisi depan
	glColor3f(1.0f, 0.0f, 0.0f);
	// buat sisi depan
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// beri warna hijau di sisi belakang
	glColor3f(0.0f, 1.0f, 0.0f);
	// buat sisi belakang
	glVertex3f(-1.0f, -1.0f,-1.0f);
	glVertex3f( 1.0f, -1.0f,-1.0f);
	glVertex3f( 1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	// beri warna biru di sisi kiri
	glColor3f(0.0f, 0.0f, 1.0f);
	// buat sisi kiri
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f(-1.0f, -1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f, 1.0f);
	// beri warna cyan di sisi kanan
	glColor3f(0.0f, 1.0f, 1.0f);
	// buat sisi kanan
	glVertex3f( 1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f,-1.0f);
	glVertex3f( 1.0f, 1.0f,-1.0f);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	// beri warna kuning di sisi atas
	glColor3f(1.0f, 1.0f, 0.0f);
	// buat sisi atas
	glVertex3f(-1.0f, 1.0f, 1.0f);
	glVertex3f( 1.0f, 1.0f, 1.0f);
	glVertex3f( 1.0f, 1.0f,-1.0f);
	glVertex3f(-1.0f, 1.0f,-1.0f);
	// beri warna magenta di sisi bawah
	glColor3f(1.0f, 0.0f, 1.0f);
	// buat sisi bawah
	glVertex3f(-1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f, 1.0f);
	glVertex3f( 1.0f, -1.0f,-1.0f);
	glVertex3f(-1.0f, -1.0f,-1.0f);
	glEnd();
}

// fungsi untuk menggambar silinder
void drawCylinder(float radius, float height, int slices, int stacks)
{
	glPushMatrix();
	GLUquadricObj* cyl = gluNewQuadric();
	gluQuadricDrawStyle(cyl, GLU_FILL);
	gluQuadricNormals(cyl, GLU_SMOOTH);
	gluQuadricOrientation(cyl, GLU_INSIDE);
	// buat tutup atas silinder
	glTranslatef(0.0f, -height/2, 0.0f);
	glRotatef(-90, 1.0f, 0.0f, 0.0f);
//	glColor3f(1.0f, 1.0f, 0.0f); // warna kuning
	gluDisk(cyl, 0.0f, radius, slices, stacks);
	// buat badan silinder
	glColor3f(2.0f, 0.0f, 1.0f); // warna merah
	gluCylinder(cyl, radius, 0.0f, height, slices, stacks);
	// buat tutup bawah silinder
	glColor3f(2.0f, 0.0f, 1.0f); // warna kuning
	glTranslatef(0.0f, 0.0f, height);
//	gluDisk(cyl, 0.0f, radius, slices, stacks);
	glPopMatrix();
}

// fungsi untuk menggambar bola
void drawSphere(float radius, int slices, int stacks)
{
	glPushMatrix();
	glColor3f(1.0f, 0.0f, 0.0f); // warna merah
	GLUquadric *sphere = gluNewQuadric();
	gluQuadricDrawStyle(sphere, GLU_FILL);
	gluQuadricNormals(sphere, GLU_SMOOTH);
	gluSphere(sphere, radius, slices, stacks);
	glPopMatrix();
}

// fungsi untuk menggambar obyek
void drawObject()
{
	// obyek bisa dimasukkan diantara glPushMatrix() dan glPopMatrix() 
	// fungsinya agar obyek tidak terpengaruh atau mempengaruhi obyek lain
	// saat diwarnai, ditransformasi dan sebagainya
	glPushMatrix();

	// operasi transformasi rotasi obyek ke arah kanan-kiri
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);

	glPushMatrix();

	// operasi transformasi rotasi obyek ke arah atas-bawah
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

	// set warna obyek ke warna hijau (0.0f, 1.0f, 0.0f)
	glColor3f(2.0f, 0.0f, 1.0f);

	// bila menggambar obyek harus diawali glBegin(tipe obyek) dan diakhiri dengan glEnd()
	// kecuali menggunakan fungsi yang sudah ada di GLUT-OpenGL seperti dibawah ini
	//glutSolidCube(1.0f); // menggambar obyek kubus
	//glutWireTeapot(1.0f);
//	drawCube(); //panggil fungsi untuk membuat obyek kubus
	drawCylinder(1.0f, 2.0f, 4, 20); // fungsi untuk membuat obyek silinder
	//drawSphere(1.0f, 50, 50); // fungsi untuk membuat obyek bola

	glPopMatrix();

	glPopMatrix();
	
	// membuat obyek polyhedron
	//glutSolidTetrahedron();
	//glutSolidOctahedron();
	//glutSolidDodecahedron();
	//glutSolidIcosahedron();
	//glutSolidCube(1.0f);
	//glutSolidCone(1.0f, 1.0f, 50, 50);
	//glutSolidSphere(1.0f, 50, 50);
	//glutSolidTeapot(1.0f);
	//glutSolidTorus(0.5f, 1.0f, 20, 20);
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

	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
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
	// zoom in
	case GLUT_KEY_PAGE_UP:
		// masukkan perintah disini bila tombol PgUp ditekan
		posX += rotX * fraction;		
		posZ += rotZ * fraction;
		glutPostRedisplay();	// update obyek
		break;
	// zoom out
	case GLUT_KEY_PAGE_DOWN:
		// masukkan perintah disini bila tombol PgDn ditekan
		posX -= rotX * fraction;
		posZ -= rotZ * fraction;
		glutPostRedisplay();	// update obyek
		break;
	}
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
	glutCreateWindow("Mohammad Farid Hendianto / 2200018401 - KODE DASAR PRAKTIKUM GRAFIKA KOMPUTER");
	
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