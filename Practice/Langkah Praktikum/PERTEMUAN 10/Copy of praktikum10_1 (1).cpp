// PRAKTIKUM KE-10 GRAFIKA KOMPUTER
// PEMODELAN MULTI RESOLUSI
// ADHI PRAHARA
// TEKNIK INFORMATIKA
// UNIVERSITAS AHMAD DAHLAN
// 2017

// deklarasi header 
#include <windows.h>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

// header untuk GLUT
#include <glut.h>

using namespace std;

#define PHI 3.141592654		
#define SCREEN_WIDTH 480
#define SCREEN_HEIGHT 480

// container untuk membuat tipe data 3D (X, Y, Z)
struct Vec3
{
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	//
	Vec3() { }
	~Vec3() { }
};

// inisialisasi variabel untuk kecepatan pergerakan kamera
// (akan digunakan di praktikum ke-5 tentang Proyeksi)
float fov = 45;						// sudut proyeksi
float moveSpeed = 0.5f;				// kecepatan pergerakan kamera
float rotateSpeed = 0.05f;			// kecepatan rotasi kamera

// inisialisasi variabel untuk proyeksi
// (akan digunakan di praktikum ke-5 tentang Proyeksi)			
// posisi kamera
Vec3 camPosition = Vec3(0.0f, 0.0f, 5.0f);
Vec3 camLookAt = Vec3(0.0f, 0.0f, -1.0f);
Vec3 camUp = Vec3(0.0f, 1.0f, 0.0f);	
// sudut transformasi kamera
float camAngle = 0.0f;

// inisialisasi variabel untuk proyeksi dengan mouse
bool firstMouse = true;
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCREEN_WIDTH / 2.0f;
float lastY = SCREEN_HEIGHT / 2.0f;

// inisialisasi variabel untuk transformasi 
// seperti translasi, rotasi atau scaling
// (akan digunakan di praktikum ke-4 tentang transformasi 2D/3D)
float objectAngleX = 0.0f;				// sudut tranformasi obyek
float objectAngleY = 0.0f;				// sudut tranformasi obyek
Vec3 objectRotation = Vec3(0.0f, 1.0f, 0.0f); // posisi rotasi
Vec3 objectPosition = Vec3(0.0f, 0.0f, 0.0f);	// posisi obyek

#define vX 0.525731112119133696
#define vZ 0.850650808352039932

// daftar titik yang membentuk obyek
// 12 titik dengan posisi yang membentuk icosahedron
static GLfloat vdata[12][3] = 
{
	{ -vX, 0.0, vZ },{ vX, 0.0, vZ },{ -vX, 0.0, -vZ },{ vX, 0.0, -vZ },
	{ 0.0, vZ, vX },{ 0.0, vZ, -vX },{ 0.0, -vZ, vX },{ 0.0, -vZ, -vX },
	{ vZ, vX, 0.0 },{ -vZ, vX, 0.0 },{ vZ, -vX, 0.0 },{ -vZ, -vX, 0.0 }
};

// daftar indeks dari titik-titik yang membentuk suatu sisi
// karena obyek berupa icosahedron (mempunyai 20 sisi)
// dan mesh berupa segitiga maka satu sisi terdiri dari 3 titik
// indeks disini mewakili setiap sisi dari icosahedron
static int tindices[20][3] = {
	{ 1,4,0 },{ 4,9,0 },{ 4,5,9 },{ 8,5,4 },{ 1,8,4 },
	{ 1,10,8 },{ 10,3,8 },{ 8,3,5 },{ 3,2,5 },{ 3,7,2 },
	{ 3,10,7 },{ 10,6,7 },{ 6,11,7 },{ 6,0,11 },{ 6,1,0 },
	{ 10,1,6 },{ 11,0,9 },{ 2,11,9 },{ 5,2,9 },{ 11,2,7 }
};

GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat mat_diffuse[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_ambient[] = { 0.8, 0.6, 0.4, 1.0 };
GLfloat mat_shininess = 100.0;	

GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
GLfloat light_specular[] = { 0.0, 0.0, 0.0, 1.0 };

GLfloat light_position1[] = { 1.5, 1.0, -2.0, 0.0 }; // posisi sumber cahaya 1
GLfloat light_position2[] = { 1.5, 1.0, 2.0, 0.0 }; // posisi sumber cahaya 2

int flat = 1;	// 0 = smooth shading, 1 = flat shading
int subdiv = 0;	// kedalaman subdivisi

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

// fungsi untuk melakukan normalisasi bidang
void normface(Vec3 v1, Vec3 v2, Vec3 v3)
{
	Vec3 d1, d2;

	d1.X = v1.X - v2.X; d1.Y = v1.Y - v2.Y; d1.Z = v1.Z - v2.Z;
	d2.X = v2.X - v3.X; d2.Y = v2.Y - v3.Y; d2.Z = v2.Z - v3.Z;

	Vec3 tn = cross(d1, d2);
	tn = normalize(tn);
	glNormal3f(tn.X, tn.Y, tn.Z);
}

// gambar triangle / segitiga dengan normal bidang
void drawTriangleFlat(Vec3 v1, Vec3 v2, Vec3 v3)
{
	glBegin(GL_TRIANGLES);
	normface(v1, v2, v3);
	glVertex3f(v1.X, v1.Y, v1.Z);
	glVertex3f(v2.X, v2.Y, v2.Z);
	glVertex3f(v3.X, v3.Y, v3.Z);
	glEnd();
}

// gambar triangle / segitiga dengan normal disetiap vertexnya
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

// melakukan subdivisi dengan rekursi sebanyak kedalaman subdivisi 
// yang diinginkan kemudian gambar hasil subdivisinya
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

	// hitung titik tengah disetiap segment garis pada segitiga
	v12.X = (v1.X + v2.X) / 2.0; 
	v12.Y = (v1.Y + v2.Y) / 2.0; 
	v12.Z = (v1.Z + v2.Z) / 2.0;
	v23.X = (v2.X + v3.X) / 2.0; 
	v23.Y = (v2.Y + v3.Y) / 2.0; 
	v23.Z = (v2.Z + v3.Z) / 2.0;
	v31.X = (v3.X + v1.X) / 2.0; 
	v31.Y = (v3.Y + v1.Y) / 2.0; 
	v31.Z = (v3.Z + v1.Z) / 2.0;

	// hitung normalnya
	v12 = normalize(v12);
	v23 = normalize(v23);
	v31 = normalize(v31);

	// lakukan subdivisi dengan titik-titik baru diatas
	subdivide(v1, v12, v31, depth - 1);
	subdivide(v2, v23, v12, depth - 1);
	subdivide(v3, v31, v23, depth - 1);
	subdivide(v12, v23, v31, depth - 1);
}

// fungsi ini digunakan untuk menggambar obyek 
void drawObject()
{
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);

	glPushMatrix();

	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);

	glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position2);

	// gambar ke-20 sisi icosahedron
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
		// subdivisi setiap sisinya
		subdivide(vdata1, vdata2, vdata3, subdiv);
		// update informasi titik-titik yang baru
		// setelah subdivisi
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
}

// taruh semua fungsi obyek yang akan digambar di fungsi display()
void display()
{
	// bersihkan dan reset layar dan buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	// posisikan pandangan kamera
	// dalam hal ini sumbu Y kamera ada di atas 
	// dan posisi kamera di camPosition
	gluLookAt(camPosition.X, camPosition.Y, camPosition.Z,
		camPosition.X + camLookAt.X, 
		camPosition.Y + camLookAt.Y, 
		camPosition.Z + camLookAt.Z,
		camUp.X, camUp.Y, camUp.Z);

	// panggil fungsi untuk menggambar obyek
	drawObject();
	
	// tampilkan obyek ke layar
	// gunakan glFlush() bila memakai single buffer
	// gunakan glutSwapBuffers() bila memakai double buffer
	glutSwapBuffers();
}

// inisialisasikan variabel, pencahayaan, tekstur, 
// pengaturan pandangan kamera dan sebagainya di fungsi init()
void init(void)
{
	// inisialisasi warna latar belakang layar 
	// dalam hal ini warna putih warna putih (1.0, 1.0, 1.0, 0.0)
	glClearColor(1.0, 1.0, 1.0, 0.0);
	// mengaktifkan depth buffer
	glEnable(GL_DEPTH_TEST);				
	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();
	// set proyeksi ke proyeksi perspektif
	gluPerspective(fov, 1.0, 1.0, 100.0);	
	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();						
	// inisialisasi kamera pandang
	// kamera berada di posisi (0.0f, 0.0f, 0.0f)
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

	glShadeModel(GL_SMOOTH);	/* enable smooth shading */
	glEnable(GL_LIGHTING);	/* enable lighting */
	glEnable(GL_LIGHT0);		/* enable light 0 */
	glEnable(GL_LIGHT1);		/* enable light 0 */
}

// fungsi ini digunakan bila layar akan diresize (default)
// (akan dijelaskan pada praktikum ke-5)
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fov, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	//if (w <= h)
	//	glOrtho(-1.25, 1.25, 
	//	   -1.25 * (GLfloat)w / (GLfloat)h, 
	//		1.25 * (GLfloat)w / (GLfloat)h, -2.0, 2.0);
	//else
	//	glOrtho(-1.25 * (GLfloat)w / (GLfloat)h, 
	//		1.25 * (GLfloat)w / (GLfloat)h, 
	//	   -1.25, 1.25, -2.0, 2.0);
	glMatrixMode(GL_MODELVIEW);
}

// fungsi untuk mengatur masukan dari keyboard 
void keyboard(unsigned char key, int x, int y)
{
	Vec3 result = normalize(cross(camLookAt, camUp));

	switch (key)
	{
	case 'w':	// bila tombol 'w' pada keyboard ditekan
				// geser pandangan kamera ke depan
			camPosition.X += moveSpeed * camLookAt.X;
			camPosition.Y += moveSpeed * camLookAt.Y;
			camPosition.Z += moveSpeed * camLookAt.Z;
			glutPostRedisplay();
			break;
	case 's': 	// bila tombol 's' pada keyboard ditekan
				// geser pandangan kamera ke belakang
			camPosition.X -= moveSpeed * camLookAt.X;
			camPosition.Y -= moveSpeed * camLookAt.Y;
			camPosition.Z -= moveSpeed * camLookAt.Z;
			glutPostRedisplay();
		break;
	case 'a':	// bila tombol 'a' pada keyboard ditekan
				// geser pandangan kamera ke ke kiri
			camPosition.X -= moveSpeed * result.X;
			camPosition.Y -= moveSpeed * result.Y;
			camPosition.Z -= moveSpeed * result.Z;
			glutPostRedisplay();
		break;
	case 'd':	// bila tombol 'd' pada keyboard ditekan
				// geser pandangan kamera ke kanan
			camPosition.X += moveSpeed * result.X;
			camPosition.Y += moveSpeed * result.Y;
			camPosition.Z += moveSpeed * result.Z;
			glutPostRedisplay();
		break;
	case 'f':	// bila tombol 'f' pada keyboard ditekan
				// percepat laju pergerakan kamera
			moveSpeed = moveSpeed + 0.5f;
			glutPostRedisplay();
		break;
	case 'g':	// bila tombol 'g' pada keyboard ditekan
				// perlambat laju pergerakan kamera
			moveSpeed = moveSpeed - 0.5f > 0.5f ? moveSpeed - 0.5f : 0.5f;
			glutPostRedisplay();
		break;
	case 27:	// bila tombol 'esc' pada keyboard ditekan
				// keluar program
			exit(0);
		break;
	}
}

// fungsi untuk mengatur masukan tombol dari mouse
void mousebutton(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON)
	{
		if (state == GLUT_UP)
			firstMouse = false;
		else
			firstMouse = true;
	}
}

// fungsi untuk mengatur masukan pergerakan dari mouse
void mousemove(int x, int y)
{
	if (firstMouse)
	{
		lastX = x;
		lastY = y;
		firstMouse = false;
	}

	GLfloat xoffset = x - lastX;
	GLfloat yoffset = lastY - y;
	lastX = x;
	lastY = y;

	GLfloat sensitivity = 0.05;
	xoffset *= sensitivity;
	yoffset *= sensitivity;

	yaw += xoffset;
	pitch += yoffset;

	if (pitch > 89.0f)
		pitch = 89.0f;
	if (pitch < -89.0f)
		pitch = -89.0f;

	Vec3 front;
	front.X = cos(yaw * PHI / 180) * cos(pitch * PHI / 180);
	front.Y = sin(pitch * PHI / 180);
	front.Z = sin(yaw * PHI / 180) * cos(pitch * PHI / 180);
	camLookAt = normalize(front);

	glutPostRedisplay();
}

// fungsi untuk mengatur masukan dari keyboard 
// untuk arah panah kiri, kanan, atas, bawah, PgDn dan PgUp
void keyboardGLUT(int key, int x, int y)
{
	float fraction = 0.1f;

	switch (key)
	{
		// masukkan perintah disini bila tombol panah kiri ditekan
	case GLUT_KEY_LEFT:
		// perintah rotasi obyek ke kiri 
		objectAngleY -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
		// masukkan perintah disini bila tombol panah kanan ditekan
	case GLUT_KEY_RIGHT:
		// perintah rotasi obyek ke kanan
		objectAngleY += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
		// masukkan perintah disini bila tombol panah atas ditekan
	case GLUT_KEY_UP:
		// perintah rotasi obyek ke atas
		objectAngleX -= 1.0f;
		glutPostRedisplay();	// update obyek
		break;
		// masukkan perintah disini bila tombol panah bawah ditekan
	case GLUT_KEY_DOWN:
		// perintah rotasi obyek ke bawah
		objectAngleX += 1.0f;
		glutPostRedisplay();	// update obyek
		break;
		// masukkan perintah disini bila tombol PgUp ditekan
	case GLUT_KEY_PAGE_UP:
		subdiv++;
		glutPostRedisplay();	// update obyek
		break;
		// masukkan perintah disini bila tombol PgDn ditekan
	case GLUT_KEY_PAGE_DOWN:
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
	// besarnya jendela dalam piksel dalam hal ini 480x480
	glutInitWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);		
	// posisi jendela dilayar komputer dalam piksel
	glutInitWindowPosition(100, 100);	

	// judul jendela (wajib diubah dengan informasi 
	// NAMA / NIM - JUDUL PRAKTIKUM masing-masing)
	glutCreateWindow("NAMA / NIM - KODE DASAR PRAKTIKUM GRAFIKA KOMPUTER");
	
	// panggil fungsi init untuk melakukan inisialisasi awal
	init();

	// event handler untuk display, reshape dan keyboard
	glutDisplayFunc(display);   // display
	glutReshapeFunc(reshape);   // reshape
	glutKeyboardFunc(keyboard);  // keyboard
	glutMouseFunc(mousebutton);	// mouse button
	glutMotionFunc(mousemove);	// mouse movement
	glutSpecialFunc(keyboardGLUT);
	//glutTimerFunc(0, timer, 0); // aktifkan timer bila perlu

	// looping
	glutMainLoop();

	return 0;
}