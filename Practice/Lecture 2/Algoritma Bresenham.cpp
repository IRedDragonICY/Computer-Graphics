// kode dasar untuk semua praktikum grafika komputer
// adhi prahara. 2018

// deklarasikan semua header disini
#include <windows.h>
#include <GL/glut.h>

#include <stdio.h>
#include <iostream>

int SCREEN_WIDTH = 400; 
int SCREEN_HEIGHT = 400;

// inisialisasi variabel untuk transformasi seperti translasi, rotasi atau scaling
float angle = 0.0f;					// sudut transformasi kamera
float posX = 0.0f, rotX =  0.0f;	// posisi kamera di sumbu X
float posY = 0.0f, rotY =  0.0f;	// posisi kamera di sumbu Y
float posZ = 5.0f, rotZ = -1.0f;	// posisi kamera di sumbu Z

float objectAngleX = 0.0f;			// sudut tranformasi obyek di sumbu X
float objectAngleY = 0.0f;			// sudut tranformasi obyek di sumbu Y
float objectAngleZ = 0.0f;			// sudut tranformasi obyek di sumbu Z

// container untuk membuat tipe data 3D (X, Y, Z) 
struct Vec3{ 
	float X; float Y; float Z;
	Vec3(float x, float y, float z) { X = x; Y = y; Z = z; }
	//
	Vec3() { }
	~Vec3() { }
}; 

// fungsi untuk menggambar garis dengan algoritma Bresenham
// bila slopenya terhadap X
void lineBresenhamX(Vec3 point1, Vec3 point2)
{
	// hitung selisih panjang
	int dY = point2.Y - point1.Y;
	int dX = point2.X - point1.X;
	
	int yi = 1; // skala penambahan
	// bila delta Y kurang dari 0
	if (dY < 0) 
	{
		yi = -1; 
		dY = -dY;
	}
	
	// mulai menggambar titik-titik
	glBegin(GL_POINTS);
	// koordinat titik awal
	glVertex3f(point1.X, point1.Y, point1.Z);
	
	int pX = point1.X, pY = point1.Y, pZ = point1.Z;
	int dY2 = 2*dY; 		// hitung 2*deltaY 
	int dX2 = 2*dX; 		// hitung 2*deltaX
	int pk = dY2 - dX; 		// hitung p0
	// kenaikan terhadap X 
	for (int i = point1.X; i < point2.X; i++)
	{
		if (pk < 0) 		// bila p < 0
		{
			pk = pk + dY2; 	// update pk+1 = pk + 2dY
			pX = pX + 1; 	// Xn+1 = Xn + 1
			pY = pY;		// Yn+1 = Yn
		}
		else // bila p >= 0
		{
			pk = pk + dY2 - dX2; 	// update pk+1 = pk + 2dY - 2dX
			pX = pX + 1; 			// Xn+1 = Xn + 1
			pY = pY + yi; 			// Yn+1 = Yn + yi
		}
		glVertex3f(pX, pY, pZ);
	}
	// koordinat titik akhir
	glVertex3f(point2.X, point2.Y, point2.Z);
	glEnd();
}

// fungsi untuk menggambar garis dengan algoritma Bresenham
// bila slopenya terhadap Y
void lineBresenhamY(Vec3 point1, Vec3 point2)
{
	// hitung selisih panjang
	int dY = point2.Y - point1.Y;
	int dX = point2.X - point1.X;
	
	int xi = 1; // skala penambahan
	// bila delta X kurang dari 0
	if (dX < 0) 
	{
		xi = -1; 
		dX = -dX;
	}
	
	// mulai menggambar titik-titik
	glBegin(GL_POINTS);
	// koordinat titik awal
	glVertex3f(point1.X, point1.Y, point1.Z);
	
	int pX = point1.X, pY = point1.Y, pZ = point1.Z;
	int dY2 = 2*dY; // hitung 2*deltaY 
	int dX2 = 2*dX; // hitung 2*deltaX
	int pk = dX2 - dY; // hitung p0
	// kenaikan terhadap Y
	for (int i = point1.Y; i < point2.Y; i++)
	{
		if (pk < 0) // bila p < 0
		{
			pk = pk + dX2; 		// update pk+1 = pk + 2dX
			pX = pX; 			// Xn+1 = Xn
			pY = pY + 1; 		// Yn+1 = Yn + 1
		}
		else // bila p >= 0
		{
			pk = pk + dX2 - dY2; 	// update pk+1 = pk + 2dX - 2dY
			pX = pX + xi; 			// Xn+1 = Xn + xi
			pY = pY + 1; 			// Yn+1 = Yn + 1
		}
		glVertex3f(pX, pY, pZ);
	}
	// koordinat titik akhir
	glVertex3f(point2.X, point2.Y, point2.Z);
	glEnd();
}


// fungsi untuk menggambar garis dengan algoritma Bresenham
void lineBresenham(Vec3 point1, Vec3 point2)
{
	// hitung selisih panjang
	int dY = point2.Y - point1.Y;
	int dX = point2.X - point1.X;
	if (abs(dY) < abs(dX)) // bila deltaY lebih pendek dari deltaX
	{
		if (point1.X < point2.X) // bila X1 < X2
			lineBresenhamX(point1, point2);
		else // bila X1 > X2 maka dibalik
			lineBresenhamX(point2, point1);
	}
	else // bila deltaY lebih panjang dari deltaX
	{
		if (point1.Y < point2.Y) // bila Y1 < Y2
			lineBresenhamY(point1, point2);
		else // bila Y1 > Y2 maka dibalik
			lineBresenhamY(point2, point1);
	}
}


// fungsi untuk menggambar obyek
void drawObject()
{
	glPushMatrix();
	// operasi transformasi rotasi obyek ke arah kanan-kiri
	glRotatef(objectAngleY, 0.0f, 1.0f, 0.0f);
	glPushMatrix();
	// operasi transformasi rotasi obyek ke arah atas-bawah
	glRotatef(objectAngleX, 1.0f, 0.0f, 0.0f);
	
	// set warna obyek ke warna hijau (0.0f, 1.0f, 0.0f)
	glColor3f(0.0f, 1.0f, 0.0f);
	
	 // gambar sumbu
	Vec3 sbY1 = Vec3( 0.0f,-300.0f, 0.0f);
	Vec3 sbY2 = Vec3( 0.0f, 300.0f, 0.0f);
	Vec3 sbX1 = Vec3(-300.0f, 0.0f, 0.0f);
	Vec3 sbX2 = Vec3( 300.0f, 0.0f, 0.0f);
	lineBresenham(sbX1, sbX2);
	lineBresenham(sbY1, sbY2);
	// kuadran 1
	Vec3 point1 = Vec3( 100.0f, 100.0f, 0.0f);
	Vec3 point2 = Vec3( 200.0f, 120.0f, 0.0f);
	lineBresenham(point1, point2);
	// kuadran 2
	point1 = Vec3(-100.0f, 100.0f, 0.0f);
	point2 = Vec3(-120.0f, 200.0f, 0.0f);
	lineBresenham(point1, point2);
	// kuadran 3
	point1 = Vec3(-100.0f, -100.0f, 0.0f);
	point2 = Vec3(-200.0f, -120.0f, 0.0f);
	lineBresenham(point1, point2);
	// kuadran 4
	point1 = Vec3( 100.0f, -100.0f, 0.0f);
	point2 = Vec3( 120.0f, -200.0f, 0.0f);
	lineBresenham(point1, point2);
	
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
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glEnable(GL_DEPTH_TEST);				// mengaktifkan depth buffer
	glMatrixMode(GL_PROJECTION);		
	glLoadIdentity();
	//gluPerspective(45.0, 1.0, 1.0, 100.0);	
	glOrtho((GLfloat)-SCREEN_WIDTH/2, (GLfloat)SCREEN_WIDTH/2, (GLfloat)-SCREEN_HEIGHT/2, (GLfloat)SCREEN_HEIGHT/2, 1.0, 100.0); // set proyeksi ke perspektif
	glMatrixMode(GL_MODELVIEW);				
	glLoadIdentity();						
	// inisialisasi kamera pandang
	gluLookAt(0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

// fungsi ini digunakan bila layar akan diresize (default)
void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//gluPerspective(45, (GLfloat)w / (GLfloat)h, 1.0, 100.0);
	glOrtho((GLfloat)-w/2, (GLfloat)w/2, (GLfloat)-h/2, (GLfloat)h/2, 1.0, 100.0);
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
	glutCreateWindow("Algoritma Bresenham || M Farid H - 2200018401");
	
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