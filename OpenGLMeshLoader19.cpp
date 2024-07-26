#include <iostream>
#include <cstdlib>
#include "TextureBuilder.h"
#include "Model_3DS.h"
#include "GLTexture.h"
#include <glut.h>
#include <utility>

using namespace std;

#define GLUT_KEY_ESCAPE 27
#define DEG2RAD(a) (a * 0.0174532925)

int WIDTH = 1280;
int HEIGHT = 720;

GLuint tex;
GLuint  tex_sun;
GLuint  tex_moon;
char title[] = "3D Model Loader Sample";

// 3D Projection Options
GLdouble fovy = 45.0;
GLdouble aspectRatio = (GLdouble)WIDTH / (GLdouble)HEIGHT;
GLdouble zNear = 0.1;
GLdouble zFar = 100;


class Vector3f {
public:
	float x, y, z;

	Vector3f(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) {
		x = _x;
		y = _y;
		z = _z;
	}

	Vector3f operator+(Vector3f& v) {
		return Vector3f(x + v.x, y + v.y, z + v.z);
	}

	Vector3f operator-(Vector3f& v) {
		return Vector3f(x - v.x, y - v.y, z - v.z);
	}

	Vector3f operator*(float n) {
		return Vector3f(x * n, y * n, z * n);
	}

	Vector3f operator/(float n) {
		return Vector3f(x / n, y / n, z / n);
	}

	Vector3f unit() {
		return *this / sqrt(x * x + y * y + z * z);
	}

	Vector3f cross(Vector3f v) {
		return Vector3f(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
	}
};

int level = 1;
bool start1 = true;
bool start2 = true ;
class Camera {
public:
	Vector3f eye, center, up;

	Camera(float eyeX = 0.0f, float eyeY = 10.0f, float eyeZ = -30.0f, float centerX = 0.0f, float centerY = 0.0f, float centerZ = 0.0f, float upX = 0.0f, float upY = 1.0f, float upZ = 0.0f) {
		if (level == 1) {
			eyeX = 20; 
			eyeZ = 0;
		}
		eye = Vector3f(eyeX, eyeY, eyeZ);
		center = Vector3f(centerX, centerY, centerZ);
		up = Vector3f(upX, upY, upZ);
	}

	void moveX(float d) {
		Vector3f right = up.cross(center - eye).unit();
		eye = eye + right * d;
		center = center + right * d;
	}

	void moveY(float d) {
		eye = eye + up.unit() * d;
		center = center + up.unit() * d;
	}

	void moveZ(float d) {
		Vector3f view = (center - eye).unit();
		eye = eye + view * d;
		center = center + view * d;
	}

	void rotateX(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + up * sin(DEG2RAD(a));
		up = view.cross(right);
		center = eye + view;
	}

	void rotateY(float a) {
		Vector3f view = (center - eye).unit();
		Vector3f right = up.cross(view).unit();
		view = view * cos(DEG2RAD(a)) + right * sin(DEG2RAD(a));
		right = view.cross(up);
		center = eye + view;
	}

	void look(int level) {
		if (level == 1) {
			if (start1) {
				start1 = false;
				eye.x = 20.0f;
				eye.z = 0.0f;
				center.x = 0;  center.z = 0;
			}
			gluLookAt(
				eye.x , eye.y, eye.z ,
				center.x, center.y, center.z,
				up.x, up.y, up.z
			);
		}
		else {
			if (start2) {
				start2 = false;
				eye.x = 0.0f;
				eye.z = -30.0f;
				center.x = 0;  center.z = 0;
		}
			gluLookAt(
				eye.x, eye.y, eye.z,
				center.x, center.y, center.z,
				up.x, up.y, up.z
			);
		}
		
	}
};

Camera camera;
class Vector
{
public:
	GLdouble x, y, z;
	Vector() {}
	Vector(GLdouble _x, GLdouble _y, GLdouble _z) : x(_x), y(_y), z(_z) {}
	//================================================================================================//
	// Operator Overloading; In C++ you can override the behavior of operators for you class objects. //
	// Here we are overloading the += operator to add a given value to all vector coordinates.        //
	//================================================================================================//
	void operator +=(float value)
	{
		x += value;
		y += value;
		z += value;
	}
};

Vector Eye(20, 5, 20);
Vector At(0, 0, 0);
Vector Up(0, 1, 0);

int cameraZoom = 0;

// Model Variables
Model_3DS model_house;
Model_3DS model_tree;
Model_3DS lamp;
Model_3DS basket;
Model_3DS atm;
Model_3DS fountain;
Model_3DS bench;
Model_3DS fence;
float sonicx;
float sonicz;
float sonicposx;
float sonicposz;
bool mcmoving;
float shadowx;
float shadowz;
float shadowposx;
float shadowposz;
float targetacquiredx;
float targetacquiredz;
bool droneattacking = false;
float cannonx;
float cannonz;
float m;
float c;
float droneposx;
float droneposz;
float bossposx;
float bossposz;
float dronerotation;
float bossrotation;
float originalmposx;
float mposx;
float sonicrotation = 0;
float sonicrotation2 = 90;
float shadowrotation = 0;
float shadowrotation2 = 180;
bool set = false;
int dronehp = 1;
bool dronedes = false;
bool bossdes = true;
bool readytoattack = true;
int dronenum = 1;
int mchp = 10;
bool mcdied;
bool mchit = false;
bool HPlow = false;
float rot = 0;
float x = 0;

int bosshp = 5;
bool view = true;
bool cameraView = false;
float camCenterX = camera.center.x;
float camCenterZ = camera.center.z;
float camEyeX = camera.eye.x;
float camEyeZ = camera.eye.z;
string look = "i";
float light = 1.0;
bool dayNight;

// Textures
GLTexture tex_ground;
GLTexture tex_building;
GLTexture tex_path;
GLTexture tex_asphalt;
GLTexture tex_street;
GLTexture sonic_fur;
GLTexture sonic_belly;
GLTexture white;
GLTexture sonic_iris;
GLTexture whiteLeather;
GLTexture redLeather;
GLTexture metal;
GLTexture gold;
GLTexture brownHair;
GLTexture redFur;

void cylinder(GLfloat base, GLfloat top, GLfloat h, float r, float g, float b) {

	glColor3f(r, g, b);
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_FILL);
	gluCylinder(qobj, base, top, h, 25, 25);

}

void circle(float centerx, float centery, float ra, float r, float g, float b) {
	glPushMatrix();
	glTranslatef(centerx, centery, 0.0f);
	glColor3f(r, g, b);
	GLUquadric* quad = gluNewQuadric();
	gluDisk(quad, 0, ra, 50, 50);
	glPopMatrix();



}

void drawCylin(double a, double b, double c, double d, double e) {
	GLUquadricObj* qobj;
	qobj = gluNewQuadric();
	gluQuadricDrawStyle(qobj, GLU_LINE); //GLU_FILL, GLU_SILHOUETTE, GLU_POINT
	gluCylinder(qobj, a, b, c, d, e);
}
void drawCircle(float r) {
	glPushMatrix();
	GLUquadric* quadObj = gluNewQuadric();
	gluDisk(quadObj, 0, r, 50, 50);
	glPopMatrix();
}

void sonichead() { //sonic head method

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_fur.texture[0]);




	glPushMatrix();
	glTranslated(0, 0.7, -0.6);
	glRotated(-70, 1, 0, 0);
	glRotated(-90, 1, 0, 0);                    //  hair 
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -1.3);
	glRotated(-90, 1, 0, 0);
	glRotated(-90, 1, 0, 0);                  //  hair 
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.7, -0.8);
	glRotated(-110, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.2f, 0.3f, 0.9f);              //  hair 
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -1.3);
	glRotated(-90, 1, 0, 0);
	glRotated(-90, 1, 0, 0);                 //  hair 
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.5, 0, -0.6);
	glRotated(-10, 0, 1, 0);
	glRotated(-80, 1, 0, 0);                    //  hair 
	glRotated(-90, 1, 0, 0);
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5, -0.5, -0.6);
	glRotated(-40, 0, 1, 0);
	glRotated(-100, 1, 0, 0);
	glRotated(-90, 1, 0, 0);                     //  hair 
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, -0.5, -0.6);
	glRotated(40, 0, 1, 0);
	glRotated(-100, 1, 0, 0);                        //  hair 
	glRotated(-90, 1, 0, 0);
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, 0, -0.6);
	glRotated(10, 0, 1, 0);
	glRotated(-80, 1, 0, 0);
	glRotated(-90, 1, 0, 0);                        //  hair 
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);



	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_belly.texture[0]);
	glPushMatrix();
	glTranslated(1.0, 1, 0);
	glRotated(45, 0, 0, 1);
	glRotated(90, 0, 1, 0);                          // right ear
	glColor3f(1.0f, 0.8f, 0.5f);
	glutSolidCone(0.5, 1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.7, 0.7);
	glScaled(1.8, 1, 1);
	glColor3f(1.0f, 0.8f, 0.5f);
	glutSolidSphere(0.8, 25, 25);                 // mouth part of the head
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1.0, 1, 0);
	glRotated(-45, 0, 0, 1);
	glRotated(-90, 0, 1, 0);                       // left ear
	glColor3f(1.0f, 0.8f, 0.5f);
	glutSolidCone(0.5, 1, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glTranslated(-0.7, -0.9, 1.5);
	glRotated(90, 0, 1, 0);                               // mouth
	cylinder(0.03, 0.03, 1.5, 0, 0, 0);
	glPopMatrix();


	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_fur.texture[0]);
	glPushMatrix();
	glColor3f(0.2f, 0.3f, 0.9f);
	glutSolidSphere(1.5, 25, 25);                 // blue part of the head
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);



	glPushMatrix();
	glTranslated(0, -0.3, 1.6);
	glColor3f(0.0f, 0.0f, 0.0f);                    // nose
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glPushMatrix();
	glScaled(2, 2, 2);
	glTranslated(-0.3, 0.1, 0.6);
	glRotated(45, 0, 0, 1);
	glScaled(1, 2, 1);                                // left eye 
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();  // left eye components
	glScaled(2, 2, 1);
	glTranslated(0.3, -0.5, 0.2);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_iris.texture[0]);
	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(-0.6, 0.3, 1.4);                  //left iris                         
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(-0.6, 0.3, 1.5);
	glColor3f(0.0f, 0.0f, 0.0f);                     //left pupil
	glutSolidSphere(0.05, 25, 25);
	glPopMatrix();

	glPopMatrix(); // end of left eye components

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glPushMatrix();
	glScaled(2, 2, 2);
	glTranslated(0.3, 0.1, 0.6);
	glRotated(-45, 0, 0, 1);
	glScaled(1, 2, 1);                                 // right eye
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);



	glPushMatrix(); //  right eye component
	glScaled(2, 2, 1);
	glTranslated(-0.3, -0.5, 0.2);


	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_iris.texture[0]);
	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(0.6, 0.3, 1.4);
	glColor3f(0.0f, 1.0f, 0.0f);                    //right iris
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(0.6, 0.3, 1.5);
	glColor3f(0.0f, 0.0f, 0.0f);                     // right pupil
	glutSolidSphere(0.05, 25, 25);
	glPopMatrix();

	glPopMatrix();   // end of right eye components
} // end of sonic head method
void Eggman() {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, metal.texture[0]);
	glPushMatrix();                            // Eggman Drone

	glColor3f(0.5, 0.5, 0.5);   // Ground
	glPushMatrix();
	glScaled(1, 0.1, 1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();            // Front
	glTranslated(0, 0.225, 0.23);
	glScaled(1, 1, 0.1);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();            // Control Panel

	glBegin(GL_QUADS);
	glVertex3d(0.25, 0.3, 0.205);
	glVertex3d(-0.25, 0.3, 0.205);
	glVertex3d(-0.25, 0.3, 0.1);
	glVertex3d(0.25, 0.3, 0.1);
	glVertex3d(0.25, 0.475, 0.205);
	glVertex3d(-0.25, 0.475, 0.205);
	glVertex3d(-0.25, 0.3, 0.1);
	glVertex3d(0.25, 0.3, 0.1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3d(0.25, 0.3, 0.205);
	glVertex3d(0.25, 0.475, 0.205);
	glVertex3d(0.25, 0.3, 0.1);
	glVertex3d(-0.25, 0.475, 0.205);
	glVertex3d(-0.25, 0.3, 0.205);
	glVertex3d(-0.25, 0.3, 0.1);
	glEnd();
	glColor3f(0, 0, 0);
	glPushMatrix();   // Handle
	glTranslated(-0.15, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.05);
	drawCylin(0.01, 0.01, 0.05, 70, 70);
	glPopMatrix();
	glColor3f(1, 0, 0); // Handle Ball
	glPushMatrix();
	glTranslated(-0.15, 0.4257519037, 0.117141635);
	glutSolidSphere(0.02, 10, 10);
	glPopMatrix();
	glPushMatrix();   // Red Button
	glTranslated(0, 0.4, 0.16);
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	glColor3f(0, 0, 1);   // Button
	glPushMatrix();
	glTranslated(0.09, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCylin(0.01, 0.01, 0.005, 70, 70);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.09, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCircle(0.01);
	glPopMatrix();
	glColor3f(0, 1, 0);   // Button 2
	glPushMatrix();
	glTranslated(0.15, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCylin(0.01, 0.01, 0.005, 70, 70);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.15, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCircle(0.01);
	glPopMatrix();

	glPopMatrix();
	glColor3f(0, 0, 0.5);
	glPushMatrix();             // El 7adid
	glTranslated(0.228, 0.47, -0.228);
	glRotated(90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 2
	glTranslated(-0.228, 0.47, -0.228);
	glRotated(90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 3
	glTranslated(0.225, 0.45, -0.225);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 4
	glTranslated(-0.225, 0.45, -0.225);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 5
	glTranslated(-0.225, 0.45, -0.225);
	glRotated(90, 0, 1, 0);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glColor3f(0.5, 0.5, 0.5);     // Corner
	glPushMatrix();
	glTranslated(0.225, 0.45, -0.225);
	glutSolidCube(0.05);
	glPopMatrix();
	glPushMatrix();               // Corner 2
	glTranslated(-0.225, 0.45, -0.225);
	glutSolidCube(0.05);
	glPopMatrix();
	glColor3f(1, 1, 0);          // Bazookaaa
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, gold.texture[0]);
	glPushMatrix();
	glTranslated(0, 0.2, 0.24);
	drawCylin(0.05, 0.05, 0.15, 200, 200);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	glPushMatrix();                                // Eggman
	glColor3f(0.9, 0.7, 0.5);       // Head

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, metal.texture[0]);
	glPushMatrix();
	glTranslated(0, 1, 0);
	glutSolidSphere(0.05, 50, 50);
	glPopMatrix();

	glPushMatrix();                      // Nose
	glColor3f(0.5, 0.4, 0);
	glTranslated(0, 0.3, 0);
	glBegin(GL_TRIANGLES);
	glVertex3d(0, 0.715, 0.04);
	glVertex3d(0.01, 0.695, 0.04);
	glVertex3d(0, 0.695, 0.07);
	glVertex3d(0, 0.715, 0.04);
	glVertex3d(-0.01, 0.695, 0.04);
	glVertex3d(0, 0.695, 0.07);
	glVertex3d(0.01, 0.695, 0.04);
	glVertex3d(-0.01, 0.695, 0.04);
	glVertex3d(0, 0.695, 0.07);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glColor3f(0.5, 0.5, 0.7);           // Glasses
	glPushMatrix();
	glTranslated(0, 0.3, 0);

	glPushMatrix();   // Glass
	glTranslated(0.02, 0.7, 0.05);
	drawCircle(0.01);
	glPopMatrix();

	glPushMatrix();   // Glass 2
	glTranslated(-0.02, 0.7, 0.05);
	drawCircle(0.01);
	glPopMatrix();

	glBegin(GL_QUADS); // In Between
	glColor3f(0.5, 0.5, 0.5);
	glVertex3f(0.02, 0.698, 0.0499);
	glVertex3f(0.02, 0.702, 0.0499);
	glVertex3f(0.007, 0.702, 0.0499);
	glVertex3f(0.007, 0.698, 0.0499);
	glVertex3f(-0.02, 0.698, 0.0499);
	glVertex3f(-0.02, 0.702, 0.0499);
	glVertex3f(-0.007, 0.702, 0.0499);
	glVertex3f(-0.007, 0.698, 0.0499);
	glVertex3f(0.007, 0.702, 0.0499);
	glVertex3f(0.007, 0.698, 0.0499);
	glVertex3d(0, 0.709, 0.0499);
	glVertex3d(0, 0.713, 0.0499);
	glVertex3f(-0.007, 0.702, 0.0499);
	glVertex3f(-0.007, 0.698, 0.0499);
	glVertex3d(0, 0.709, 0.0499);
	glVertex3d(0, 0.713, 0.0499);
	glEnd();

	glPopMatrix();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, brownHair.texture[0]);
	glPushMatrix();                         // Moustache
	glColor3f(0.5, 0.3, 0.2);
	glTranslated(0, 0.435, 0);
	glScaled(1, 0.8, 1);

	glBegin(GL_POLYGON);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(0.09, 0.72, 0.046);
	glVertex3f(0.04, 0.68, 0.051);
	glVertex3f(0.08, 0.66, 0.047);
	glVertex3f(0.04, 0.64, 0.051);
	glVertex3f(0.05, 0.6, 0.05);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(-0.09, 0.72, 0.046);
	glVertex3f(-0.04, 0.68, 0.051);
	glVertex3f(-0.08, 0.66, 0.047);
	glVertex3f(-0.04, 0.64, 0.051);
	glVertex3f(-0.05, 0.6, 0.05);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(0.09, 0.72, 0.064);
	glVertex3f(0.04, 0.68, 0.059);
	glVertex3f(0.08, 0.66, 0.063);
	glVertex3f(0.04, 0.64, 0.059);
	glVertex3f(0.05, 0.6, 0.06);
	glEnd();
	glBegin(GL_POLYGON);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(-0.09, 0.72, 0.064);
	glVertex3f(-0.04, 0.68, 0.059);
	glVertex3f(-0.08, 0.66, 0.063);
	glVertex3f(-0.04, 0.64, 0.059);
	glVertex3f(-0.05, 0.6, 0.06);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(0.09, 0.72, 0.046);
	glVertex3f(0.09, 0.72, 0.064);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(-0.09, 0.72, 0.046);
	glVertex3f(-0.09, 0.72, 0.064);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(0.05, 0.6, 0.05);
	glVertex3f(0.05, 0.6, 0.06);
	glVertex3d(0, 0.685, 0.055);
	glVertex3f(-0.05, 0.6, 0.05);
	glVertex3f(-0.05, 0.6, 0.06);
	glEnd();
	glBegin(GL_QUADS);
	glVertex3f(0.09, 0.72, 0.046);
	glVertex3f(0.09, 0.72, 0.064);
	glVertex3f(0.04, 0.68, 0.059);
	glVertex3f(0.04, 0.68, 0.051);
	glVertex3f(0.04, 0.68, 0.059);
	glVertex3f(0.04, 0.68, 0.051);
	glVertex3f(0.08, 0.66, 0.047);
	glVertex3f(0.08, 0.66, 0.063);
	glVertex3f(0.08, 0.66, 0.047);
	glVertex3f(0.08, 0.66, 0.063);
	glVertex3f(0.04, 0.64, 0.059);
	glVertex3f(0.04, 0.64, 0.051);
	glVertex3f(0.04, 0.64, 0.059);
	glVertex3f(0.04, 0.64, 0.051);
	glVertex3f(0.05, 0.6, 0.05);
	glVertex3f(0.05, 0.6, 0.06);
	glVertex3f(-0.09, 0.72, 0.046);
	glVertex3f(-0.09, 0.72, 0.064);
	glVertex3f(-0.04, 0.68, 0.059);
	glVertex3f(-0.04, 0.68, 0.051);
	glVertex3f(-0.04, 0.68, 0.059);
	glVertex3f(-0.04, 0.68, 0.051);
	glVertex3f(-0.08, 0.66, 0.047);
	glVertex3f(-0.08, 0.66, 0.063);
	glVertex3f(-0.08, 0.66, 0.047);
	glVertex3f(-0.08, 0.66, 0.063);
	glVertex3f(-0.04, 0.64, 0.059);
	glVertex3f(-0.04, 0.64, 0.051);
	glVertex3f(-0.04, 0.64, 0.059);
	glVertex3f(-0.04, 0.64, 0.051);
	glVertex3f(-0.05, 0.6, 0.05);
	glVertex3f(-0.05, 0.6, 0.06);
	glEnd();

	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();                          // body

	glColor3f(1, 0, 0);        // Upper Body
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();
	glScaled(1.5, 2.2, 1);
	glTranslated(0, 0.337, 0);
	glutSolidSphere(0.1, 50, 50);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glColor3f(0, 0, 0);        // Lower Body
	glPushMatrix();
	glScaled(1.5, 2.2, 1);
	glTranslated(0, 0.32, 0);
	glutSolidSphere(0.1, 50, 50);
	glPopMatrix();

	glPopMatrix();
	glPushMatrix();                         // Arms
	glColor3f(1, 0, 0);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();      // Shoulder
	glTranslated(0.17, 0.86, 0);
	glRotated(-25, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	drawCylin(0.02, 0.02, 0.106, 200, 200);
	glPopMatrix();
	glPushMatrix();      // Shoulder 2
	glTranslated(-0.17 + 0.09606863, 0.86 + 0.0447975, 0);
	glRotated(25, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	drawCylin(0.02, 0.02, 0.106, 200, 200);
	glPopMatrix();
	glPushMatrix();      // Ulnar
	glTranslated(0.19, 0.628, 0);
	glRotated(5, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.0143, 0.0143, 0.25, 300, 300);
	glPopMatrix();
	glPushMatrix();      // Ulnar 2
	glTranslated(-0.19, 0.628, 0);
	glRotated(-5, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.0143, 0.0143, 0.25, 300, 300);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, gold.texture[0]);
	glColor3f(1, 1, 0);  // Bracelet
	glPushMatrix();
	glTranslated(0.19, 0.641, 0);
	glRotated(5, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.0143, 0.016, 25, 25);
	glPopMatrix();
	glPushMatrix();      // Bracelet 2
	glTranslated(-0.19, 0.641, 0);
	glRotated(-5, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.0143, 0.016, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glColor3f(1, 1, 1);  // Hand
	glPushMatrix();
	glTranslated(0.19, 0.6, 0);
	glScaled(1, 2, 1);
	glutSolidSphere(0.02, 50, 50);
	glPopMatrix();
	glPushMatrix();      // Hand 2
	glTranslated(-0.19, 0.6, 0);
	glScaled(1, 2, 1);
	glutSolidSphere(0.02, 50, 50);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();

	glPushMatrix();                   // Legs
	glColor3f(0, 0, 0);

	glPushMatrix();   // Leg
	glTranslatef(0.04, 0.05, 0);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.47, 200, 200);
	glPopMatrix();
	glPushMatrix();   // Leg 2
	glTranslatef(-0.04, 0.05, 0);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.47, 200, 200);
	glPopMatrix();
	glPushMatrix();      // Shoe
	glTranslated(0.04, 0.05, 0.02);
	glScaled(1, 1, 1.5);
	glutSolidSphere(0.03, 25, 25);
	glPopMatrix();
	glPushMatrix();      // Shoe 2
	glTranslated(-0.04, 0.05, 0.02);
	glScaled(1, 1, 1.5);
	glutSolidSphere(0.03, 25, 25);
	glPopMatrix();
	glPushMatrix();      // Shoe
	glTranslated(0.04, 0.05, 0.05);
	glutSolidCone(0.023, 0.04, 25, 25);
	glPopMatrix();
	glPushMatrix();      // Shoe 2
	glTranslated(-0.04, 0.05, 0.05);
	glutSolidCone(0.023, 0.04, 25, 25);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}
void bossdrone() {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, metal.texture[0]);
	glPushMatrix();                            // Eggman Drone

	glColor3f(0.5, 0.5, 0.5);   // Ground
	glPushMatrix();
	glScaled(1, 0.1, 1);
	glutSolidCube(0.5);
	glPopMatrix();

	glPushMatrix();            // Front
	glTranslated(0, 0.225, 0.23);
	glScaled(1, 1, 0.1);
	glutSolidCube(0.5);
	glPopMatrix();
	glPushMatrix();            // Control Panel

	glBegin(GL_QUADS);
	glVertex3d(0.25, 0.3, 0.205);
	glVertex3d(-0.25, 0.3, 0.205);
	glVertex3d(-0.25, 0.3, 0.1);
	glVertex3d(0.25, 0.3, 0.1);
	glVertex3d(0.25, 0.475, 0.205);
	glVertex3d(-0.25, 0.475, 0.205);
	glVertex3d(-0.25, 0.3, 0.1);
	glVertex3d(0.25, 0.3, 0.1);
	glEnd();
	glBegin(GL_TRIANGLES);
	glVertex3d(0.25, 0.3, 0.205);
	glVertex3d(0.25, 0.475, 0.205);
	glVertex3d(0.25, 0.3, 0.1);
	glVertex3d(-0.25, 0.475, 0.205);
	glVertex3d(-0.25, 0.3, 0.205);
	glVertex3d(-0.25, 0.3, 0.1);
	glEnd();
	glColor3f(0, 0, 0);
	glPushMatrix();   // Handle
	glTranslated(-0.15, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.05);
	drawCylin(0.01, 0.01, 0.05, 70, 70);
	glPopMatrix();
	glColor3f(1, 0, 0); // Handle Ball
	glPushMatrix();
	glTranslated(-0.15, 0.4257519037, 0.117141635);
	glutSolidSphere(0.02, 10, 10);
	glPopMatrix();
	glPushMatrix();   // Red Button
	glTranslated(0, 0.4, 0.16);
	glutSolidSphere(0.03, 10, 10);
	glPopMatrix();
	glColor3f(0, 0, 1);   // Button
	glPushMatrix();
	glTranslated(0.09, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCylin(0.01, 0.01, 0.005, 70, 70);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.09, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCircle(0.01);
	glPopMatrix();
	glColor3f(0, 1, 0);   // Button 2
	glPushMatrix();
	glTranslated(0.15, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCylin(0.01, 0.01, 0.005, 70, 70);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.15, 0.4, 0.16);
	glRotated(31, 1, 0, 0);
	glTranslated(0, 0, -0.005);
	drawCircle(0.01);
	glPopMatrix();

	glPopMatrix();
	glColor3f(0, 0, 0.5);
	glPushMatrix();             // El 7adid
	glTranslated(0.228, 0.47, -0.228);
	glRotated(90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 2
	glTranslated(-0.228, 0.47, -0.228);
	glRotated(90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 3
	glTranslated(0.225, 0.45, -0.225);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 4
	glTranslated(-0.225, 0.45, -0.225);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glPushMatrix();             // El 7adid 5
	glTranslated(-0.225, 0.45, -0.225);
	glRotated(90, 0, 1, 0);
	drawCylin(0.02, 0.02, 0.45, 200, 200);
	glPopMatrix();
	glColor3f(0.5, 0.5, 0.5);     // Corner
	glPushMatrix();
	glTranslated(0.225, 0.45, -0.225);
	glutSolidCube(0.05);
	glPopMatrix();
	glPushMatrix();               // Corner 2
	glTranslated(-0.225, 0.45, -0.225);
	glutSolidCube(0.05);
	glPopMatrix();
	glColor3f(1, 1, 0);          // Bazookaaa
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, gold.texture[0]);
	glPushMatrix();
	glTranslated(0, 0.2, 0.24);
	drawCylin(0.05, 0.05, 0.15, 200, 200);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}
void ugandanKnuckles() {
	glPushMatrix();                               // Ugandan Knuckles

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glColor3f(1, 0, 0);                // Body
	glPushMatrix();
	glScaled(1.5, 1.8, 1);
	glTranslated(0, 0.337, 0);
	glutSolidSphere(0.1, 300, 300);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glColor3f(1, 1, 1);               // Eclipse
	glPushMatrix();
	glTranslated(-0.02, 0.6, 0.035);
	glRotated(-45, 0, 0, 1);
	glScaled(1.5, 1, 1);
	glutSolidSphere(0.07, 1000, 1000);
	glPopMatrix();
	glPushMatrix();
	glTranslated(0.02, 0.6, 0.035);
	glRotated(45, 0, 0, 1);
	glScaled(1.5, 1, 1);
	glutSolidSphere(0.07, 1000, 1000);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();                   // Legs
	glColor3f(1, 0, 0);

	glPushMatrix();   // Leg
	glTranslatef(0.04, 0.3, 0);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.2, 200, 200);
	glPopMatrix();
	glPushMatrix();   // Leg 2
	glTranslatef(-0.04, 0.3, 0);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.02, 0.02, 0.2, 200, 200);
	glPopMatrix();

	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPushMatrix();                   // Shoes

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glColor3f(1, 0, 0);  // Heel
	glPushMatrix();
	glTranslatef(0.04, 0.3, -0.02);
	glScaled(1.1, 1.5, 0.8);
	glutSolidCube(0.04);
	glPopMatrix();
	glPushMatrix();      // Heel 2
	glTranslatef(-0.04, 0.3, -0.02);
	glScaled(1.1, 1.5, 0.8);
	glutSolidCube(0.04);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, gold.texture[0]);
	glColor3f(1, 1, 0); // Metatarsals
	glPushMatrix();
	glTranslatef(0.04, 0.3, 0.012);
	glScaled(1.1, 1.5, 0.8);
	glutSolidCube(0.04);
	glPopMatrix();
	glPushMatrix();    // Metatarsals 2
	glTranslatef(-0.04, 0.3, 0.012);
	glScaled(1.1, 1.5, 0.8);
	glutSolidCube(0.04);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glColor3f(1, 0, 0); // Phalanges
	glPushMatrix();
	glTranslatef(0.04, 0.3, 0.044);
	glScaled(1.1, 1.5, 0.8);
	glutSolidCube(0.04);
	glPopMatrix();
	glPushMatrix();     // Phalanges 2
	glTranslatef(-0.04, 0.3, 0.044);
	glScaled(1.1, 1.5, 0.8);
	glutSolidCube(0.04);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();
	glPushMatrix();                         // Arms
	glColor3f(1, 0, 0);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();      // Shoulder
	glTranslated(0.17, 0.7, 0);
	glRotated(-25, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	drawCylin(0.02, 0.02, 0.17, 200, 200);
	glPopMatrix();
	glPushMatrix();      // Shoulder 2
	glTranslated(-0.17 + 0.1540723238, 0.7 + 0.0718451045, 0);
	glRotated(25, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	drawCylin(0.02, 0.02, 0.17, 200, 200);
	glPopMatrix();
	glPushMatrix();      // Ulnar
	glTranslated(0.182, 0.616, 0);
	glRotated(10, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.0143, 0.0143, 0.1, 300, 300);
	glPopMatrix();
	glPushMatrix();      // Ulnar 2
	glTranslated(-0.182, 0.616, 0);
	glRotated(-10, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(-90, 1, 0, 0);
	drawCylin(0.0143, 0.0143, 0.1, 300, 300);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glColor3f(1, 1, 1); // Bracelet
	glPushMatrix();
	glTranslated(0.182, 0.616, 0);
	glRotated(10, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.0143, 0.016, 25, 25);
	glPopMatrix();
	glPushMatrix();      // Bracelet 2
	glTranslated(-0.182, 0.616, 0);
	glRotated(-10, 0, 0, 1);
	glScaled(1, 1, 1.398601399);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(0.0143, 0.016, 25, 25);
	glPopMatrix();
	glPushMatrix();      // Hand
	glTranslated(0.19, 0.575, 0);
	glScaled(1, 2, 1);
	glRotated(10, 0, 0, 1);
	glutSolidSphere(0.02, 25, 25);
	glPopMatrix();
	glPushMatrix();      // Hand 2
	glTranslated(-0.19, 0.575, 0);
	glScaled(1, 2, 1);
	glRotated(-10, 0, 0, 1);
	glutSolidSphere(0.02, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPopMatrix();

	glPushMatrix();                   // Head
	glTranslated(0, 0.84, 0);
	glScaled(0.04, 0.04, 0.04);

	glColor3f(0, 0, 0);  // Mouth
	glPushMatrix();
	glTranslated(-0.7, -0.9, 1.5);
	glRotated(90, 0, 1, 0);
	drawCylin(0.03, 0.03, 1.5, 50, 50);
	glPopMatrix();
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glPushMatrix();  // Temporalis
	glColor3f(1, 0, 0);
	glutSolidSphere(1.5, 200, 200);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_belly.texture[0]);
	glPushMatrix();  // Cheeks
	glColor3f(1.0, 0.8, 0.5);
	glTranslated(0, -0.7, 0.7);
	glScaled(1.8, 1, 1);
	glutSolidSphere(0.8, 400, 400);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	glPushMatrix();       // Nose
	glColor3f(0, 0, 0);
	glTranslated(0, -0.3, 1.6);
	glutSolidSphere(0.2, 500, 500);
	glPopMatrix();
	glPushMatrix();       // Eye
	glColor3f(1, 1, 1);
	glTranslated(0.6, 0.2, 1.1);
	glutSolidSphere(0.4, 500, 500);
	glPopMatrix();
	glPushMatrix();       // Eye 2
	glTranslated(-0.6, 0.2, 1.1);
	glutSolidSphere(0.4, 500, 500);
	glPopMatrix();
	glColor3f(0, 0, 0);   // Iris
	glPushMatrix();
	glTranslated(0.65, 0.2, 1.4);
	glutSolidSphere(0.2, 500, 500);
	glPopMatrix();
	glPushMatrix();      // Iris 2
	glTranslated(-0.65, 0.2, 1.4);
	glutSolidSphere(0.2, 500, 500);
	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}
void knuckleswithdrone() {

	glPushMatrix();

	glPushMatrix();
	glScaled(1, 0.5, 1);
	glRotated(180, 0, 1, 0);
	bossdrone();
	glPopMatrix();

	glPushMatrix();
	glRotated(180, 0, 1, 0);
	ugandanKnuckles();
	glPopMatrix();

	glPopMatrix();


}
void sonic(float scx, float scy, float scz, float x, float z) {

	glPushMatrix();
	glTranslatef(x, 125, z);
	glRotated(180, 0, 1, 0);
	glScalef(scx, scy, scz);




	glPushMatrix();
	glScaled(1.2, 1.2, 1.2);              // building sonic head              
	sonichead();
	glPopMatrix();


	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_fur.texture[0]);
	glPushMatrix();
	glScaled(1.5, 2.2, 1);
	glTranslated(0, -1.3, 0);
	glColor3f(0.2f, 0.3f, 0.9f);           // blue part of the body
	glutSolidSphere(0.7, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_belly.texture[0]);
	glPushMatrix();
	glScaled(1.5, 2.2, 1);
	glTranslated(0, -1.3, 0.6);
	glColor3f(1.0f, 0.8f, 0.5f);            // brown part of the body
	glutSolidSphere(0.5, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();  // right arm
	glTranslatef(-0.8, 1.4, -0.3);

	glPushMatrix();
	glTranslatef(2.8, -4, 0.5);
	glRotated(-25, 0, 0, 1);                            // upper part of right arm
	glRotated(-90, 0, 1, 0);
	cylinder(0.2, 0.2, 1.5, 1.0f, 0.8f, 0.5f);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(2.65, -5.8, 0.5);
	glRotated(-90, 1, 0, 0);                             // lower part of right arm
	cylinder(0.2, 0.2, 2, 1.0f, 0.8f, 0.5f);
	glPopMatrix();

	glPopMatrix(); // right arm end





	glPushMatrix(); // left arm

	glTranslatef(0.8, 1.5, -0.3);

	glPushMatrix();
	glTranslatef(-1.5, -3.5, 0.5);
	glRotated(25, 0, 0, 1);
	glRotated(-90, 0, 1, 0);                                    // upper part of left arm
	cylinder(0.2, 0.2, 1.5, 1.0f, 0.8f, 0.5f);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2.7, -5.9, 0.5);
	glRotated(-90, 1, 0, 0);                                   // lower part of left arm
	cylinder(0.2, 0.2, 2, 1.0f, 0.8f, 0.5f);
	glPopMatrix();

	glPopMatrix(); // end of left arm





	glPushMatrix();  // lower part
	glTranslated(0, 1.8, 0);




	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_fur.texture[0]);
	glPushMatrix();
	glTranslatef(-0.6, -9.5, 0.3);
	glRotated(-90, 1, 0, 0);
	cylinder(0.2, 0.2, 4, 0.2f, 0.3f, 0.9f);      // left leg
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.6, -9.5, 0.3);
	glRotated(-90, 1, 0, 0);
	cylinder(0.2, 0.2, 4, 0.2f, 0.3f, 0.9f);     // right leg
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	// right shoe
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();
	glTranslatef(0.6, -10.2, 0.4);
	glColor3f(1.0f, 0.0f, 0.0f);
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);                     //first part of right shoe
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, whiteLeather.texture[0]);
	glPushMatrix();
	glTranslatef(0.6, -10.2, 1.1);
	glColor3f(1.0f, 1.0f, 1.0f);             //second part of right shoe
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();
	glTranslatef(0.6, -10.2, 1.7);
	glColor3f(1.0f, 0.0f, 0.0f);              //third part of right shoe
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	// end of right shoe


	// left shoe

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();
	glTranslatef(-0.6, -10.2, 0.4);
	glColor3f(1.0f, 0.0f, 0.0f);
	glScaled(1, 1.5, 0.8);                      // first part of left shoe
	glutSolidCube(0.8);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, whiteLeather.texture[0]);
	glPushMatrix();
	glTranslatef(-0.6, -10.2, 1.1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScaled(1, 1.5, 0.8);                        // second part of left shoe
	glutSolidCube(0.8);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redLeather.texture[0]);
	glPushMatrix();
	glTranslatef(-0.6, -10.2, 1.7);
	glColor3f(1.0f, 0.0f, 0.0f);                 // third part of left shoe
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
	//end of left shoe

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glPushMatrix();
	glTranslatef(0.6, -9.3, 0.3);
	glColor3f(1.0, 1.0, 1.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);                // right shoe bracelet
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -9.3, 0.3);
	glColor3f(1.0, 1.0, 1.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);              // left shoe bracelet
	glPopMatrix();


	glPopMatrix();  // end of lower part

	glPushMatrix();
	glTranslatef(1.85, -4.7, 0.2);
	glColor3f(1.0, 1.0, 1.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);                  // right bracelet
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.9, -4.7, 0.2);
	glColor3f(1.0, 1.0, 1.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);             // left bracelet
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, white.texture[0]);
	glPushMatrix();
	glTranslated(-1.9, -5.8, 0.2);
	glScaled(1, 2, 1);
	glColor3f(1.0, 1.0, 1.0);
	glutSolidSphere(0.5, 25, 25);                     // left hand
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.85, -5.8, 0.2);
	glScaled(1, 2, 1);
	glColor3f(1.0, 1.0, 1.0);                     // right hand
	glutSolidSphere(0.5, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);






	glPopMatrix();

}
void drone2() {

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, metal.texture[0]);
	glPushMatrix();
	//glColor3f(0, 0, 0);
	glRotated(90, 1, 0, 0);
	glutSolidTorus(5, 50, 25, 25);
	glPopMatrix();

	glPushMatrix();
	//glColor3f(1, 0, 0);
	glutSolidSphere(10, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -50);
	//glColor3b(0, 0, 0);
	cylinder(5, 5, 100, 0, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-50, 0, 0);
	glRotated(90, 0, 1, 0);
	cylinder(5, 5, 100, 0, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	cylinder(5, 5, 50, 0, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -45, 0);
	glRotated(180, 0, 1, 0);
	glRotated(135, 1, 0, 0);
	cylinder(5, 5, 30, 0, 1, 1);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}
void shadowhead() {

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, redFur.texture[0]);
	glPushMatrix();
	glTranslated(0, 1.2, 0.3);
	glRotated(-90, 1, 0, 0);
	glScaled(0.5, 1.2, 0.5);
	glColor3f(1.0f, 0.0f, 0.0f);
	glutSolidSphere(0.8, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glTranslated(0, 0.7, -0.6);
	glRotated(-70, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -1.3);
	glRotated(-90, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, -0.7, -0.8);
	glRotated(-110, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, -1.3);
	glRotated(-90, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();


	glPushMatrix();
	glTranslated(0.5, 0, -0.6);
	glRotated(-10, 0, 1, 0);
	glRotated(-80, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0.5, -0.5, -0.6);
	glRotated(-40, 0, 1, 0);
	glRotated(-100, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, -0.5, -0.6);
	glRotated(40, 0, 1, 0);
	glRotated(-100, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.5, 0, -0.6);
	glRotated(10, 0, 1, 0);
	glRotated(-80, 1, 0, 0);
	glRotated(-90, 1, 0, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidCone(0.8, 2.5, 25, 25);
	glPopMatrix();





	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_belly.texture[0]);
	glPushMatrix();
	glTranslated(1.0, 1, 0);
	glRotated(45, 0, 0, 1);
	glRotated(90, 0, 1, 0);
	glColor3f(1.0f, 0.8f, 0.5f);
	glutSolidCone(0.5, 1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-1.0, 1, 0);
	glRotated(-45, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	glColor3f(1.0f, 0.8f, 0.5f);
	glutSolidCone(0.5, 1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-0.7, -0.9, 1.5);
	glRotated(90, 0, 1, 0);
	cylinder(0.03, 0.03, 1.5, 0, 0, 0);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);


	glPushMatrix();
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(1.5, 25, 25);
	glPopMatrix();

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, sonic_belly.texture[0]);
	glPushMatrix();
	glTranslated(0, -0.7, 0.7);
	glScaled(1.8, 1, 1);
	glColor3f(1.0f, 0.8f, 0.5f);
	glutSolidSphere(0.8, 25, 25);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glPushMatrix();
	glTranslated(0, -0.3, 1.6);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();


	glPushMatrix();
	glScaled(2, 2, 2);
	glTranslated(-0.3, 0.1, 0.6);
	glRotated(45, 0, 0, 1);
	glScaled(1, 2, 1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glScaled(2, 2, 1);
	glTranslated(0.3, -0.5, 0.2);

	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(-0.6, 0.3, 1.4);
	glColor3f(0.8f, 0.3f, 0.2f);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(-0.6, 0.3, 1.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.05, 25, 25);
	glPopMatrix();

	glPopMatrix();

	glPushMatrix();
	glScaled(2, 2, 2);
	glTranslated(0.3, 0.1, 0.6);
	glRotated(-45, 0, 0, 1);
	glScaled(1, 2, 1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.2, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glScaled(2, 2, 1);
	glTranslated(-0.3, -0.5, 0.2);


	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(0.6, 0.3, 1.4);
	glColor3f(0.8f, 0.3f, 0.2f);
	glutSolidSphere(0.1, 25, 25);
	glPopMatrix();


	glPushMatrix();
	glScaled(1, 2, 1);
	glTranslated(0.6, 0.3, 1.5);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.05, 25, 25);
	glPopMatrix();

	glPopMatrix();
}
void shadow(float scx, float scy, float scz, float x, float z) {

	glPushMatrix();
	glTranslatef(x, 125, z);
	glScalef(scx, scy, scz);





	glPushMatrix();
	glScaled(1.2, 1.2, 1.2);
	shadowhead();
	glPopMatrix();




	glPushMatrix();
	glScaled(1.5, 2.2, 1);
	glTranslated(0, -1.3, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glutSolidSphere(0.7, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glScaled(1, 1.2, 1);
	glTranslated(0, -1.9, 0.6);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.5, 25, 25);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-0.8, 1.4, -0.3);

	glPushMatrix();
	glTranslatef(2.8, -4, 0.5);
	glRotated(-25, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	cylinder(0.2, 0.2, 1.5, 0.0f, 0.0f, 0.0f);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(2.65, -5.8, 0.5);
	glRotated(-90, 1, 0, 0);
	cylinder(0.2, 0.2, 2, 0.0f, 0.0f, 0.0f);
	glPopMatrix();

	glPopMatrix();





	glPushMatrix();

	glTranslatef(0.8, 1.5, -0.3);

	glPushMatrix();
	glTranslatef(-1.5, -3.5, 0.5);
	glRotated(25, 0, 0, 1);
	glRotated(-90, 0, 1, 0);
	cylinder(0.2, 0.2, 1.5, 0.0f, 0.0f, 0.0f);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(-2.7, -5.9, 0.5);
	glRotated(-90, 1, 0, 0);
	cylinder(0.2, 0.2, 2, 0.0f, 0.0f, 0.0f);
	glPopMatrix();

	glPopMatrix();





	glPushMatrix();
	glTranslated(0, 1.8, 0);





	glPushMatrix();
	glTranslatef(-0.6, -9.5, 0.3);
	glRotated(-90, 1, 0, 0);
	cylinder(0.2, 0.2, 4, 0.0f, 0.0f, 0.0f);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(0.6, -9.5, 0.3);

	glRotated(-90, 1, 0, 0);
	cylinder(0.2, 0.2, 4, 0.0f, 0.0f, 0.0f);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.6, -10.2, 0.4);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -10.2, 1.1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.6, -10.2, 1.7);
	glColor3f(1.0f, 0.0f, 0.0f);
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();



	glPushMatrix();
	glTranslatef(-0.6, -10.2, 0.4);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -10.2, 1.1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -10.2, 1.7);
	glColor3f(1.0f, 0.0f, 0.0f);
	glScaled(1, 1.5, 0.8);
	glutSolidCube(0.8);
	glPopMatrix();


	glPushMatrix();
	glTranslatef(0.6, -9.3, 0.3);
	glColor3f(1.0, 0.0, 0.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.6, -9.3, 0.3);
	glColor3f(1.0, 0.0, 0.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);
	glPopMatrix();


	glPopMatrix();

	glPushMatrix();
	glTranslatef(1.85, -4.7, 0.2);
	glColor3f(1.0, 0.0, 0.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-1.9, -4.7, 0.2);
	glColor3f(1.0, 0.0, 0.0);
	glRotated(90, 1, 0, 0);
	glScaled(0.5, 0.5, 1);
	glutSolidTorus(0.3, 0.5, 25, 25);
	glPopMatrix();



	glPushMatrix();
	glTranslated(-1.9, -5.8, 0.2);
	glScaled(1, 2, 1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.5, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(1.85, -5.8, 0.2);
	glScaled(1, 2, 1);
	glColor3f(1.0f, 1.0f, 1.0f);
	glutSolidSphere(0.5, 25, 25);
	glPopMatrix();






	glPopMatrix();

}
void drone() {

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, metal.texture[0]);
	glPushMatrix();
	glRotated(-90, 1, 0, 0);
	cylinder(50, 48, 10, 1, 0, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(90, 1, 0, 0);
	circle(0, 0, 50, 0, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 10, 0);
	glRotated(90, 1, 0, 0);
	circle(0, 0, 48, 0, 0, 1);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 0, 0);
	glColor3b(0, 0, 0);
	glutSolidSphere(10, 25, 25);
	glPopMatrix();

	glPushMatrix();
	glTranslated(0, 5, -95);
	glColor3b(0, 0, 0);
	cylinder(5, 5, 190, 0, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glTranslated(-95, 5, 0);
	glRotated(90, 0, 1, 0);
	glColor3b(0, 0, 0);
	cylinder(5, 5, 190, 0, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(45, 0, 1, 0);
	glTranslated(-95, 5, 0);
	glRotated(90, 0, 1, 0);
	glColor3b(0, 0, 0);
	cylinder(5, 5, 190, 0, 1, 0);
	glPopMatrix();

	glPushMatrix();
	glRotated(-45, 0, 1, 0);
	glTranslated(-95, 5, 0);
	glRotated(90, 0, 1, 0);
	glColor3b(0, 0, 0);
	cylinder(5, 5, 190, 0, 1, 0);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}

void check(int value) {
	
	glutTimerFunc(5000, check, 0);
}

void droneattack(int value) {
	cannonx = 0;
	cannonz = 0;
	if (level == 1) {
		targetacquiredx = sonicposx;
		targetacquiredz = sonicposz;
	}
	else if (level == 2) {
		targetacquiredx = shadowposx;
		targetacquiredz = shadowposz;
	}
	droneattacking = true;
	if (targetacquiredx != droneposx && !bossdes) {
		m = (targetacquiredz - droneposz) / (targetacquiredx - droneposx);
	}

	if (targetacquiredx != bossposx && bossdes) {
		m = (targetacquiredz - bossposz) / (targetacquiredx - bossposx);
	}
	glutPostRedisplay();
	PlaySound(TEXT("sounds/cannon.wav"), NULL, SND_FILENAME | SND_ASYNC);
	glutTimerFunc(5000, droneattack, 0);
}
void camRotLeft() {
	camCenterX = camera.center.x;
	camCenterZ = camera.center.z;
	if (level == 1) {
		camEyeX = camera.eye.x;
		camEyeZ = camera.eye.z;
		camera.center.x = camCenterZ + sonicposx - sonicposz;
		camera.center.z = -camCenterX + sonicposx + sonicposz;
		camera.eye.x = camEyeZ + sonicposx - sonicposz;
		camera.eye.z = -camEyeX + sonicposx + sonicposz;
	}
	else {
		camEyeX = camera.eye.x;
		camEyeZ = camera.eye.z;
		camera.center.x = camCenterZ + shadowposx - shadowposz;
		camera.center.z = -camCenterX + shadowposx + shadowposz;
		camera.eye.x = camEyeZ + shadowposx - shadowposz;
		camera.eye.z = -camEyeX + shadowposx + shadowposz;
	}
}
void viewToggle() {
	if (level == 1) {
		if (view) {
			view = false;

			camera.eye.x = sonicposx;
			camera.eye.y = 5;
			camera.eye.z = sonicposz;
		}
		else {
			view = true;
			if (look == "i") {
				camera.eye.x = sonicposx + 10;
				camera.eye.y = 10;
				camera.eye.z = sonicposz ;
			}
			else if (look == "k") {
				camera.eye.x = sonicposx - 10;
				camera.eye.y = 10;
				camera.eye.z = sonicposz ;
			}
			else if (look == "j") {
				camera.eye.x = sonicposx ;
				camera.eye.y = 10;
				camera.eye.z = sonicposz - 10;
			}
			else if (look == "l") {
				camera.eye.x = sonicposx ;
				camera.eye.y = 10;
				camera.eye.z = sonicposz + 10;
			}


		}
	}
	else {
		if (view) {
			view = false;

			camera.eye.x = shadowposx;
			camera.eye.y = 5;
			camera.eye.z = shadowposz;
		}
		else {
			view = true;
			if (look == "i") {
				camera.eye.x = shadowposx;
				camera.eye.y = 10;
				camera.eye.z = shadowposz - 10;
			}
			else if (look == "k") {
				camera.eye.x = shadowposx;
				camera.eye.y = 10;
				camera.eye.z = shadowposz + 10;
			}
			else if (look == "j") {
				camera.eye.x = shadowposx - 10;
				camera.eye.y = 10;
				camera.eye.z = shadowposz;
			}
			else if (look == "l") {
				camera.eye.x = shadowposx + 10;
				camera.eye.y = 10;
				camera.eye.z = shadowposz;
			}


		}

	}
	
}

void Anim() {
	x += 0.1;
	if (light < 1.1 && light >0.9) dayNight = true;
	else if (light < 0.1 && light >-0.9) dayNight = false;
	if(dayNight) light -= 0.05;
	else light += 0.05;
	
	if (droneattacking == true) {
		if (targetacquiredx < bossposx && !(targetacquiredx < bossposx + 2 && targetacquiredx > bossposx - 2))
			cannonx = cannonx - 1;
		else if (targetacquiredx < bossposx + 2 && targetacquiredx > bossposx - 2 && bossposz < targetacquiredz) {
			cannonz = cannonz + 1;
			cannonx = 0;
		}

		else if (targetacquiredx < bossposx + 2 && targetacquiredx > bossposx - 2 && bossposz > targetacquiredz) {
			cannonz = cannonz - 1;
			cannonx = 0;
		}
		else if (targetacquiredx > bossposx && !(targetacquiredx < bossposx + 2 && targetacquiredx > bossposx - 2))
			cannonx = cannonx + 1;


		if (!(targetacquiredx < bossposx + 2 && targetacquiredx > bossposx - 2)) {
			cannonz = (m * cannonx);
		}


	}


	if (level == 1 && bossposx != sonicposx) {
		dronerotation += 0.1;
		bossrotation = atan((bossposz - sonicposz) / (bossposx - sonicposx)) * 180 / 3.1415;
	}
	else if (shadowposx != droneposx && level == 2) {
		dronerotation = atan((droneposz - shadowposz) / (droneposx - shadowposx)) * 180 / 3.1415;
		bossrotation = atan((bossposz - shadowposz) / (bossposx - shadowposx)) * 180 / 3.1415;
	}

	glutPostRedisplay();
}

void newdrone(int value) {
	if (dronenum < 3) {
		//cout << " " << dronenum << " ";
		dronedes = false;
		dronenum += 1;
		dronehp = 1;
		glutPostRedisplay();
	}
	else {
		bossdes = false;
	}
}

void resethit(int value) {
	mchit = false;
}


void keyboardup(unsigned char key, int x, int y) {

	mcmoving = false;
	glutPostRedisplay();
}
void prepareattack(int value) {
	readytoattack = true;
}

void RenderStreet1()
{
	//west
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.3, 0.3, 0.3);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_asphalt.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(20, 0, -5);
	glTexCoord2f(1, 0);
	glVertex3f(20, 0, 5);
	glTexCoord2f(1, 1);
	glVertex3f(-20, 0, 5);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, -5);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//east

	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.3, 0.3, 0.3);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_asphalt.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(5, 0, -25);
	glTexCoord2f(1, 0);
	glVertex3f(-5, 0, -25);
	glTexCoord2f(1, 1);
	glVertex3f(-5, 0, -5);
	glTexCoord2f(0, 1);
	glVertex3f(5, 0, -5);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//south
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_street.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(20, 0, 35);
	glTexCoord2f(1, 0);
	glVertex3f(30, 0, 35);
	glTexCoord2f(1, 1);
	glVertex3f(30, 0, -35);
	glTexCoord2f(0, 1);
	glVertex3f(20, 0, -35);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//north
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_street.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-20, 0, 35);
	glTexCoord2f(1, 0);
	glVertex3f(-30, 0, 35);
	glTexCoord2f(1, 1);
	glVertex3f(-30, 0, -35);
	glTexCoord2f(0, 1);
	glVertex3f(-20, 0, -35);
	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void drawTrees1() {
	// Draw Tree Model
	glPushMatrix();
	glTranslated(-29.5, 0, -3.3333);
	glScalef(0.5, 0.5, 0.5);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(29.5, 0, -3.3333);
	glScalef(0.5, 0.5, 0.5);
	model_tree.Draw();
	glPopMatrix();
}
void drawLamps1() {
	glPushMatrix();
	glTranslated(-29.3, 5, 0);
	glRotated(180, 0, 1, 0);
	glScalef(0.0015, 0.0015, 0.0015);
	lamp.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-6, 5, -4.5);
	glRotated(180, 0, 1, 0);
	glScalef(0.0015, 0.0015, 0.0015);
	lamp.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(6, 5, -4.5);
	glScalef(0.0015, 0.0015, 0.0015);
	lamp.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(29.3, 5, 0);
	glScalef(0.0015, 0.0015, 0.0015);
	lamp.Draw();
	glPopMatrix();
}
void drawBaskets1() {
	glPushMatrix();
	glTranslated(0, 0, 4.7);
	glScalef(0.001, 0.003, 0.001);
	basket.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(-5.7, 0, -24.6);
	glScalef(0.001, 0.003, 0.001);
	basket.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(3.7, 0, -24.6);
	glScalef(0.001, 0.003, 0.001);
	basket.Draw();
	glPopMatrix();
}
void drawAtms1() {
	glPushMatrix();
	glTranslated(-29.5, 0, 3.3333);
	glScalef(0.004, 0.004, 0.004);
	atm.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(29.5, 0, 3.3333);
	glScalef(0.004, 0.004, 0.004);
	atm.Draw();
	glPopMatrix();
}
void drawBuildings1() {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(-30, 0, 35);
	glTexCoord2f(0, 0);
	glVertex3f(-30, 0, -35);
	glTexCoord2f(1, 0);
	glVertex3f(-30, 20, -35);
	glTexCoord2f(1, 1);
	glVertex3f(-30, 20, 35);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, 5);
	glTexCoord2f(0, 0);
	glVertex3f(20, 0, 5);
	glTexCoord2f(5, 0);
	glVertex3f(20, 20, 5);
	glTexCoord2f(5, 5);
	glVertex3f(-20, 20, 5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 1);
	glVertex3f(30, 0, 35);
	glTexCoord2f(0, 0);
	glVertex3f(30, 0, -35);
	glTexCoord2f(1, 0);
	glVertex3f(30, 20, -35);
	glTexCoord2f(1, 1);
	glVertex3f(30, 20, 35);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 5);
	glVertex3f(-5, 0, -25);
	glTexCoord2f(0, 0);
	glVertex3f(5, 0, -25);
	glTexCoord2f(5, 0);
	glVertex3f(5, 20, -25);
	glTexCoord2f(5, 5);
	glVertex3f(-5, 20, -25);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 5);
	glVertex3f(-5, 0, -25);
	glTexCoord2f(0, 0);
	glVertex3f(-5, 0, -5);
	glTexCoord2f(5, 0);
	glVertex3f(-5, 20, -5);
	glTexCoord2f(5, 5);
	glVertex3f(-5, 20, -25);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(1, 0, 0);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 5);
	glVertex3f(5, 0, -25);
	glTexCoord2f(0, 0);
	glVertex3f(5, 0, -5);
	glTexCoord2f(5, 0);
	glVertex3f(5, 20, -5);
	glTexCoord2f(5, 5);
	glVertex3f(5, 20, -25);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 5);
	glVertex3f(-20, 0, -5);
	glTexCoord2f(0, 0);
	glVertex3f(-5, 0, -5);
	glTexCoord2f(5, 0);
	glVertex3f(-5, 20, -5);
	glTexCoord2f(5, 5);
	glVertex3f(-20, 20, -5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glNormal3f(0, 0, 1);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 5);
	glVertex3f(20, 0, -5);
	glTexCoord2f(0, 0);
	glVertex3f(5, 0, -5);
	glTexCoord2f(5, 0);
	glVertex3f(5, 20, -5);
	glTexCoord2f(5, 5);
	glVertex3f(20, 20, -5);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}
void drawFence1() {
	glPushMatrix();
	glTranslated(-20, 2, 0);
	glScalef(0.0005, 0.0005, 0.00054);
	fence.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslated(20, 2, 0);
	glScalef(0.0005, 0.0005, 0.00054);
	fence.Draw();
	glPopMatrix();
}

void drawTrees2() {
	// Draw Tree Model
	glPushMatrix();
	glTranslatef(-35, 0, 20);
	glScalef(0.5, 0.5, 0.5);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, 8);
	glScalef(0.5, 0.5, 0.5);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -4);
	glScalef(0.5, 0.5, 0.5);
	model_tree.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -16);
	glScalef(0.5, 0.5, 0.5);
	model_tree.Draw();
	glPopMatrix();
}
void drawLamps2() {
	glPushMatrix();
	glTranslatef(-35, 4, 23);
	glRotated(180, 0, 1, 0);
	glScalef(0.001, 0.001, 0.001);
	lamp.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 4, 11);
	glRotated(180, 0, 1, 0);
	glScalef(0.001, 0.001, 0.001);
	lamp.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 4, -1);
	glRotated(180, 0, 1, 0);
	glScalef(0.001, 0.001, 0.001);
	lamp.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 4, -13);
	glRotated(180, 0, 1, 0);
	glScalef(0.001, 0.001, 0.001);
	lamp.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 4, -25);
	glRotated(180, 0, 1, 0);
	glScalef(0.001, 0.001, 0.001);
	lamp.Draw();
	glPopMatrix();
}
void drawBaskets2() {
	glPushMatrix();
	glTranslatef(-35, 0, 23);
	glScalef(0.001, 0.001, 0.001);
	basket.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, 11);
	glScalef(0.001, 0.001, 0.001);
	basket.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -1);
	glScalef(0.001, 0.001, 0.001);
	basket.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -13);
	glScalef(0.001, 0.001, 0.001);
	basket.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -25);
	glScalef(0.001, 0.001, 0.001);
	basket.Draw();
	glPopMatrix();
}
void drawAtms2() {
	glPushMatrix();
	glTranslatef(-35, 0, 21);
	glScalef(0.002, 0.002, 0.002);
	atm.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, 9);
	glScalef(0.002, 0.002, 0.002);
	atm.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -3);
	glScalef(0.002, 0.002, 0.002);
	atm.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -15);
	glScalef(0.002, 0.002, 0.002);
	atm.Draw();
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-35, 0, -27);
	glScalef(0.002, 0.002, 0.002);
	atm.Draw();
	glPopMatrix();
}
void drawBuildings2() {
	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glScaled(1, 2, 1);
	glNormal3f(1, 0, 0);
	glTranslated(-35, 2.5, 27);
	glutSolidCube(5);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glScaled(1, 2, 1);
	glNormal3f(1, 0, 0);
	glTranslated(-35, 2.5, 15);
	glutSolidCube(5);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glScaled(1, 2, 1);
	glNormal3f(1, 0, 0);
	glTranslated(-35, 2.5, 3);
	glutSolidCube(5);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glScaled(1, 2, 1);
	glNormal3f(1, 0, 0);
	glTranslated(-35, 2.5, -9);
	glutSolidCube(5);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);

	glEnable(GL_TEXTURE_GEN_S);
	glEnable(GL_TEXTURE_GEN_T);
	glBindTexture(GL_TEXTURE_2D, tex_building.texture[0]);
	glPushMatrix();
	glScaled(1, 2, 1);
	glNormal3f(1, 0, 0);
	glTranslated(-35, 2.5, -21);
	glutSolidCube(5);
	glPopMatrix();
	glDisable(GL_TEXTURE_GEN_S);
	glDisable(GL_TEXTURE_GEN_T);
}
//=======================================================================
// Lighting Configuration Function
//=======================================================================
void InitLightSource()
{
	// Enable Lighting for this OpenGL Program
	glEnable(GL_LIGHTING);

	// Enable Light Source number 0
	// OpengL has 8 light sources
	glEnable(GL_LIGHT0);

	// Define Light source 0 ambient light
	GLfloat ambient[] = { 0.1f, 0.1f, 0.1, 1.0f };
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);

	// Define Light source 0 diffuse light
	GLfloat diffuse[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);

	// Define Light source 0 Specular light
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);

	// Finally, define light source 0 position in World Space
	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
}

// Material Configuration Function
//======================================================================
void InitMaterial()
{
	// Enable Material Tracking
	glEnable(GL_COLOR_MATERIAL);

	// Sich will be assigneet Material Properties whd by glColor
	glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

	// Set Material's Specular Color
	// Will be applied to all objects
	GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glMaterialfv(GL_FRONT, GL_SPECULAR, specular);

	// Set Material's Shine value (0->128)
	GLfloat shininess[] = { 96.0f };
	glMaterialfv(GL_FRONT, GL_SHININESS, shininess);
}

//=======================================================================
// OpengGL Configuration Function
//=======================================================================
void myInit(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);

	glMatrixMode(GL_PROJECTION);

	glLoadIdentity();

	gluPerspective(fovy, aspectRatio, zNear, zFar);
	//*******************************************************************************************//
	// fovy:			Angle between the bottom and top of the projectors, in degrees.			 //
	// aspectRatio:		Ratio of width to height of the clipping plane.							 //
	// zNear and zFar:	Specify the front and back clipping planes distances from camera.		 //
	//*******************************************************************************************//

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
	//*******************************************************************************************//
	// EYE (ex, ey, ez): defines the location of the camera.									 //
	// AT (ax, ay, az):	 denotes the direction where the camera is aiming at.					 //
	// UP (ux, uy, uz):  denotes the upward orientation of the camera.							 //
	//*******************************************************************************************//

	InitLightSource();

	InitMaterial();

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_NORMALIZE);
}

//=======================================================================
// Render Ground Function
//=======================================================================
void RenderGround()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_ground.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-30, 0, -30);
	glTexCoord2f(5, 0);
	glVertex3f(30, 0, -30);
	glTexCoord2f(5, 5);
	glVertex3f(30, 0, 30);
	glTexCoord2f(0, 5);
	glVertex3f(-30, 0, 30);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void RenderPath()
{
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_path.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(-30, 0, -30);
	glTexCoord2f(5, 0);
	glVertex3f(30, 0, -30);
	glTexCoord2f(5, 5);
	glVertex3f(30, 0, 30);
	glTexCoord2f(0, 5);
	glVertex3f(-30, 0, 30);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}
void RenderStreet2()
{
	//west
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_street.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(30, 0, 35);
	glTexCoord2f(5, 0);
	glVertex3f(30, 0, 30);
	glTexCoord2f(5, 5);
	glVertex3f(-30, 0, 30);
	glTexCoord2f(0, 5);
	glVertex3f(-30, 0, 35);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//east

	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_street.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(30, 0, -35);
	glTexCoord2f(5, 0);
	glVertex3f(30, 0, -30);
	glTexCoord2f(5, 5);
	glVertex3f(-30, 0, -30);
	glTexCoord2f(0, 5);
	glVertex3f(-30, 0, -35);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//south
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_street.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(35, 0, 35);
	glTexCoord2f(5, 0);
	glVertex3f(30, 0, 35);
	glTexCoord2f(5, 5);
	glVertex3f(30, 0, -35);
	glTexCoord2f(0, 5);
	glVertex3f(35, 0, -35);
	glEnd();
	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.

	//north
	glDisable(GL_LIGHTING);	// Disable lighting 

	glColor3f(0.6, 0.6, 0.6);	// Dim the ground texture a bit

	glEnable(GL_TEXTURE_2D);	// Enable 2D texturing

	glBindTexture(GL_TEXTURE_2D, tex_street.texture[0]);	// Bind the ground texture

	glPushMatrix();
	glRotated(180, 0, 1, 0);
	glBegin(GL_QUADS);
	glNormal3f(0, 1, 0);	// Set quad normal direction.
	glTexCoord2f(0, 0);		// Set tex coordinates ( Using (0,0) -> (5,5) with texture wrapping set to GL_REPEAT to simulate the ground repeated grass texture).
	glVertex3f(35, 0, 35);
	glTexCoord2f(5, 0);
	glVertex3f(30, 0, 35);
	glTexCoord2f(5, 5);
	glVertex3f(30, 0, -35);
	glTexCoord2f(0, 5);
	glVertex3f(35, 0, -35);
	glEnd();

	glPopMatrix();

	glEnable(GL_LIGHTING);	// Enable lighting again for other entites coming throung the pipeline.

	glColor3f(1, 1, 1);	// Set material back to white instead of grey used for the ground texture.
}

void setupCamera() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60, 640 / 480, 0.001, 200);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	camera.look(level);
}



//=======================================================================
// Display Function
//=======================================================================
void myDisplay(void)
{
	setupCamera();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	// use x for speedy demo because it takes 10 minuts for full cycle
	if (!HPlow) {
		glPushMatrix();
		glRotated(x, 0, 0, 1);
		GLfloat lightIntensity[] = { 0.4, 0.4, 0.4, 0.0f };
		GLfloat lightPosition[] = { 0.0f, 45.0f, 0.0f, 0.0f };
		GLfloat lighColour[] = { 1, 1, 1, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lighColour);
		glPopMatrix();
	}
	else {
		GLfloat lightIntensity[] = { light, 0.0, 0.0, 0.0f };
		GLfloat lightPosition[] = { 0.0f, 100.0f, 0.0f, 0.0f };
		GLfloat lighColour[] = { light, 0.0, 0.0, 0.0f };
		glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
		glLightfv(GL_LIGHT0, GL_AMBIENT, lightIntensity);
		glLightfv(GL_LIGHT0, GL_DIFFUSE, lighColour);
	}

	//sun
	glPushMatrix();

	GLUquadricObj* qobjS;
	qobjS = gluNewQuadric();
	glRotated(x, 0, 0, 1);
	glTranslated(0, 45, 0);
	glBindTexture(GL_TEXTURE_2D, tex_sun);
	gluQuadricTexture(qobjS, true);
	gluQuadricNormals(qobjS, GL_SMOOTH);
	gluSphere(qobjS, 2.5, 100, 100);
	gluDeleteQuadric(qobjS);
	glPopMatrix();

	//moon
	glPushMatrix();

	GLUquadricObj* qobjM;
	qobjM = gluNewQuadric();
	glRotated(x - 180, 0, 0, 1);
	glTranslated(0, 45, 0);
	glBindTexture(GL_TEXTURE_2D, tex_moon);
	gluQuadricTexture(qobjM, true);
	gluQuadricNormals(qobjM, GL_SMOOTH);
	gluSphere(qobjM, 1.5, 100, 100);
	gluDeleteQuadric(qobjM);
	glPopMatrix();

	if (level == 1) {
		RenderStreet1();

		drawAtms1();

		drawTrees1();

		drawLamps1();

		drawBaskets1();

		drawFence1();

		//sky box
		glPushMatrix();
		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();

	    drawBuildings1();

		sonicposx = 10;
		sonicposz = 0;
		droneposx = 0.5;
		droneposz = 0.5;
		bossposx = droneposx;
		bossposz = droneposz;

		if (mcdied == false) {
			if (mcmoving) {
				glPushMatrix();
				glTranslated(sonicx, 0.3, sonicz);
				glTranslated(sonicposx, 0.3, sonicposz);
				glColor3f(0.2f, 0.3f, 0.9f);
				glutSolidSphere(0.4, 25, 25);
				glPopMatrix();
			}
			else if (!mcmoving) {
				glPushMatrix();
				glTranslated(sonicx, -122, sonicz);
				glTranslated(sonicposx, 0, sonicposz);
				glRotated(sonicrotation2, 0, 1, 0);
				glTranslated(-sonicposx, 0, -sonicposz);
				sonic(0.3, 0.3, 0.3, sonicposx, sonicposz);
				glPopMatrix();

			}
			sonicposx = sonicposx + sonicx;
			sonicposz = sonicposz + sonicz;
		}

		if (dronehp <= 0 && dronedes == false) {
			dronedes = true;
			glutTimerFunc(2000, newdrone, 0);
			glutPostRedisplay();

		}

		if (mchp <= 0) {
			mcdied = true;

		}

		if (dronedes == false) {
			glPushMatrix();
			glTranslated(droneposx, 3, droneposz);
			glRotated(dronerotation, 0, 1, 0);
			glScaled(0.02, 0.02, 0.02);
			drone();
			glPopMatrix();


			if (droneattacking && !mcdied && !mchit) {
				glPushMatrix();
				glTranslated(cannonx, 0, cannonz);
				glTranslated(droneposx, 3, droneposz);
				glColor3f(0, 0, 0);
				glutSolidSphere(0.25, 25, 25);
				glPopMatrix();
			}
			if (sonicposx > droneposx - 2 && sonicposx < droneposx + 2 && sonicposz > droneposz - 2 && sonicposz < droneposz + 2) {
				dronehp -= 1;
				PlaySound(TEXT("sounds/boom1.wav"), NULL, SND_FILENAME | SND_ASYNC);

			}

			if (droneposx + cannonx < sonicposx + 1 && droneposx + cannonx > sonicposx - 1 && droneposz + cannonz < sonicposz + 1 && droneposz + cannonz > sonicposz - 1 && !mchit) {
				mchp -= 1;
				if (mchp <= 3) HPlow = true;
				PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
				mchit = true;
				glutTimerFunc(1000, resethit, 0);
			}
			if (sonicposx > droneposx + 2 && sonicposx < droneposx + 7 || sonicposx < droneposx - 2 && sonicposx > droneposx - 7) {
				if (droneposx + cannonx < sonicposx + 5 && droneposx + cannonx > sonicposx - 5 && droneposz + cannonz < sonicposz + 5 && droneposz + cannonz > sonicposz - 5 && !mchit) {
					mchp -= 1;
					if (mchp <= 3) HPlow = true;
					PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
					mchit = true;
					glutTimerFunc(1000, resethit, 0);
				}
			}
		}
		if (bossdes == false) {
			glPushMatrix();
			glTranslated(bossposx, 2, bossposz);
			if (sonicposx < bossposx)
				glRotated(-90 - bossrotation, 0, 1, 0);
			else if (sonicposx == bossposx && sonicposz < bossposz) {
				glRotated(180, 0, 1, 0);
			}
			else if (sonicposx == bossposx && sonicposz > bossposz) {
				glRotated(0, 0, 1, 0);
			}
			else if (sonicposx > bossposx) {
				glRotated(90 - bossrotation, 0, 1, 0);
			}
			glScaled(5, 5, 5);
			Eggman();
			glPopMatrix();

			if (droneattacking && !mcdied && !mchit) {
				glPushMatrix();
				glTranslated(cannonx, 0, cannonz);
				glTranslated(bossposx, 3, bossposz);
				glColor3f(0, 0, 0);
				glutSolidSphere(0.25, 25, 25);
				glPopMatrix();
			}
			
			if (sonicposx > bossposx - 2 && sonicposx < bossposx + 2 && sonicposz > bossposz - 2 && sonicposz < bossposz + 2 && readytoattack) {
				bosshp -= 1;
				PlaySound(TEXT("sounds/ow2.wav"), NULL, SND_FILENAME | SND_ASYNC);
				readytoattack = false;
				glutTimerFunc(2000, prepareattack, 0);

			}



			if (bossposx + cannonx < sonicposx + 1 && bossposx + cannonx > sonicposx - 1 && bossposz + cannonz < sonicposz + 1 && bossposz + cannonz > sonicposz - 1 && !mchit) {
				mchp -= 1;
				if (mchp <= 3) HPlow = true;
				PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
				mchit = true;
				glutTimerFunc(1000, resethit, 0);
			}

			if (sonicposx > bossposx + 2 && sonicposx < bossposx + 7 || sonicposx < bossposx - 2 && sonicposx > bossposx - 7) {
				if (bossposx + cannonx < sonicposx + 3 && bossposx + cannonx > sonicposx - 3 && bossposz + cannonz < sonicposz + 3 && bossposz + cannonz > sonicposz - 3 && !mchit) {
					mchp -= 1;
					if (mchp <= 3) HPlow = true;
					PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
					mchit = true;
					glutTimerFunc(1000, resethit, 0);
				}
			}
		}

		if (bosshp <= 0 && bossdes == false) {
			bossdes = true;
			level = 2;
			mcmoving = false;
			droneattacking = false;
			droneposx = 0;
			droneposz = 0;
			bossposx = 0;
			bossposz = 0;
			dronerotation = 0;
			bossrotation = 0;
			set = false;
			dronehp = 1;
			dronedes = false;
			bossdes = true;
			readytoattack = true;
			dronenum = 1;
			mchp = 10;
			mcdied;
			mchit = false;
			bosshp = 5;
			look = "i";

			glutPostRedisplay();


		}

		if (mchp <= 0) {
			mcdied = true;

		}

		if (mcdied) {
			start1 = true;
			look = "i";
			HPlow = false ;
			mcdied = false;
			sonicrotation = 0;
			sonicrotation2 = 90;
			sonicx = 0;
			sonicz = 0;
			sonicposx = 0;
			sonicposz = 0;
			level = 1;
			mcmoving = false;
			droneattacking = false;
			droneposx = 0;
			droneposz = 0;
			bossposx = 0;
			bossposz = 0;
			dronerotation = 0;
			bossrotation = 0;
			set = false;
			dronehp = 1;
			dronedes = false;
			bossdes = true;
			readytoattack = true;
			dronenum = 1;
			mchp = 10;
			mcdied;
			mchit = false;
			bosshp = 5;

			glutPostRedisplay();
		}


	}
	else if (level == 2) {
		
		// Draw Ground
		RenderGround();

		// Draw Street
		RenderStreet2();

		// Draw path
		glPushMatrix();
		glScaled(0.2, 1, 1);
		glTranslatef(0, 0.02, 0);
		RenderPath();
		glPopMatrix();

		glPushMatrix();
		glScaled(1, 1, 0.1);
		glTranslatef(0, 0.03, 0);
		RenderPath();
		glPopMatrix();

		// Draw Tree Model

		//north
		drawTrees2();

		//east
		glPushMatrix();
		glRotated(90, 0, 1, 0);
		drawTrees2();
		glPopMatrix();

		//south
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		drawTrees2();
		glPopMatrix();

		// Draw lamp Model

		//north
		drawLamps2();

		//east
		glPushMatrix();
		glRotated(90, 0, 1, 0);
		drawLamps2();
		glPopMatrix();

		//south
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		drawLamps2();
		glPopMatrix();

		// Draw basket Model

		//north
		drawBaskets2();

		//east
		glPushMatrix();
		glRotated(90, 0, 1, 0);
		drawBaskets2();
		glPopMatrix();

		//south
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		drawBaskets2();
		glPopMatrix();

		// Draw atm Model

		//north
		drawAtms2();

		//east
		glPushMatrix();
		glRotated(90, 0, 1, 0);
		drawAtms2();
		glPopMatrix();

		//south
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		drawAtms2();
		glPopMatrix();

		//sky box
		glPushMatrix();

		GLUquadricObj* qobj;
		qobj = gluNewQuadric();
		glTranslated(50, 0, 0);
		glRotated(90, 1, 0, 1);
		glBindTexture(GL_TEXTURE_2D, tex);
		gluQuadricTexture(qobj, true);
		gluQuadricNormals(qobj, GL_SMOOTH);
		gluSphere(qobj, 100, 100, 100);
		gluDeleteQuadric(qobj);
		glPopMatrix();

		//building

		//north
		drawBuildings2();

		//east
		glPushMatrix();
		glRotated(90, 0, 1, 0);
		drawBuildings2();
		glPopMatrix();

		//south
		glPushMatrix();
		glRotated(180, 0, 1, 0);
		drawBuildings2();
		glPopMatrix();

		//fountain
		glPushMatrix();
		glScaled(0.001, 0.001, 0.001);
		fountain.Draw();
		glPopMatrix();

		//bench
		glPushMatrix();
		glTranslated(0, 0.4, 2.5);
		glScaled(0.0001, 0.0001, 0.0001);
		bench.Draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(90, 0, 1, 0);
		glTranslated(0, 0.4, 2.5);
		glScaled(0.0001, 0.0001, 0.0001);
		bench.Draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(180, 0, 1, 0);
		glTranslated(0, 0.4, 2.5);
		glScaled(0.0001, 0.0001, 0.0001);
		bench.Draw();
		glPopMatrix();

		glPushMatrix();
		glRotated(270, 0, 1, 0);
		glTranslated(0, 0.4, 2.5);
		glScaled(0.0001, 0.0001, 0.0001);
		bench.Draw();
		glPopMatrix();

		shadowposx = 0;
		shadowposz = -20;
		droneposx = 10.5;
		droneposz = 10.5;
		bossposx = droneposx;
		bossposz = droneposz;

		if (mcdied == false) {
			if (mcmoving) {
				glPushMatrix();
				glTranslated(shadowx, 0.3, shadowz);
				glTranslated(shadowposx, 0.3, shadowposz);
				glColor3f(0.0f, 0.0f, 0.0f);
				glutSolidSphere(0.4, 25, 25);
				glPopMatrix();
			}
			else if (!mcmoving) {
				glPushMatrix();
				glTranslated(shadowx, -122, shadowz);
				glTranslated(shadowposx, 0, shadowposz);
				glRotated(shadowrotation2, 0, 1, 0);
				glTranslated(-shadowposx, 0, -shadowposz);
				shadow(0.3, 0.3, 0.3, shadowposx, shadowposz);
				glPopMatrix();

			}
			shadowposx = shadowposx + shadowx;
			shadowposz = shadowposz + shadowz;
		}

		if (dronehp <= 0 && dronedes == false) {
			dronedes = true;
			glutTimerFunc(2000, newdrone, 0);
			glutPostRedisplay();

		}

		if (mchp <= 0) {
			mcdied = true;

		}

		if (dronedes == false) {
			glPushMatrix();
			glTranslated(droneposx, 2, droneposz);
			if (shadowposx < droneposx)
				glRotated(-90 - dronerotation, 0, 1, 0);
			else if (shadowposx == droneposx && shadowposz < droneposz) {
				glRotated(180, 0, 1, 0);
			}
			else if (shadowposx == droneposx && shadowposz > droneposz) {
				glRotated(0, 0, 1, 0);
			}
			else if (shadowposx > droneposx) {
				glRotated(90 - dronerotation, 0, 1, 0);
			}
			glScaled(0.02, 0.02, 0.02);
			drone2();
			glPopMatrix();


			if (droneattacking && !mcdied && !mchit) {
				glPushMatrix();
				glTranslated(cannonx, 0, cannonz);
				glTranslated(droneposx, 2, droneposz);
				glColor3f(1, 1, 1);
				glutSolidSphere(0.25, 25, 25);
				glPopMatrix();
			}

			if (shadowposx > droneposx - 2 && shadowposx < droneposx + 2 && shadowposz > droneposz - 2 && shadowposz < droneposz + 2 && readytoattack) {
				dronehp -= 1;
				PlaySound(TEXT("sounds/boom2.wav"), NULL, SND_FILENAME | SND_ASYNC);

			}

			if (droneposx + cannonx < shadowposx + 2 && droneposx + cannonx > shadowposx - 2 && droneposz + cannonz < shadowposz + 2 && droneposz + cannonz > shadowposz - 2 && !mchit) {
				mchp -= 1;
				if (mchp <= 3) HPlow = true;
				PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
				mchit = true;
				glutTimerFunc(1000, resethit, 0);
			}

			if (shadowposx > droneposx + 2 && shadowposx < droneposx + 7 || shadowposx < droneposx - 2 && shadowposx > droneposx - 7) {
				if (droneposx + cannonx < shadowposx + 3 && droneposx + cannonx > shadowposx - 3 && droneposz + cannonz < shadowposz + 3 && droneposz + cannonz > shadowposz - 3 && !mchit) {
					mchp -= 1;
					if (mchp <= 3) HPlow = true;
					PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
					mchit = true;
					glutTimerFunc(1000, resethit, 0);
				}
			}
		}
		if (bossdes == false) {
			glPushMatrix();
			glTranslated(bossposx, 2, bossposz);
			if (shadowposx < bossposx)
				glRotated(90 - bossrotation, 0, 1, 0);
			else if (shadowposx == bossposx && shadowposz < bossposz) {
				glRotated(180, 0, 1, 0);
			}
			else if (shadowposx == bossposx && shadowposz > bossposz) {
				glRotated(0, 0, 1, 0);
			}
			else if (shadowposx > bossposx) {
				glRotated(-90 - bossrotation, 0, 1, 0);
			}
			glScaled(8, 8, 8);
			knuckleswithdrone();
			glPopMatrix();

			if (droneattacking && !mcdied && !mchit) {
				glPushMatrix();
				glTranslated(cannonx, 0, cannonz);
				glTranslated(bossposx, 3, bossposz);
				glColor3f(1, 1, 1);
				glutSolidSphere(0.25, 25, 25);
				glPopMatrix();
			}

			if (shadowposx > bossposx - 5 && shadowposx < bossposx + 5 && shadowposz > bossposz - 5 && shadowposz < bossposz + 5 && readytoattack) {
				bosshp -= 1;
				PlaySound(TEXT("sounds/ow3.wav"), NULL, SND_FILENAME | SND_ASYNC);
				readytoattack = false;
				glutTimerFunc(2000, prepareattack, 0);

			}


			if (bossposx + cannonx < shadowposx + 2 && bossposx + cannonx > shadowposx - 2 && bossposz + cannonz < shadowposz + 2 && bossposz + cannonz > shadowposz - 2 && !mchit) {
				mchp -= 1;
				if (mchp <= 3) HPlow = true;
				PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
				mchit = true;
				glutTimerFunc(1000, resethit, 0);
			}

			if (shadowposx > bossposx + 2 && shadowposx < bossposx + 7 || shadowposx < bossposx - 2 && shadowposx > bossposx - 7) {
				if (bossposx + cannonx < shadowposx + 3 && bossposx + cannonx > shadowposx - 3 && bossposz + cannonz < shadowposz + 3 && bossposz + cannonz > shadowposz - 3 && !mchit) {
					mchp -= 1;
					if (mchp <= 3) HPlow = true;
					PlaySound(TEXT("sounds/ow1.wav"), NULL, SND_FILENAME | SND_ASYNC);
					mchit = true;
					glutTimerFunc(1000, resethit, 0);
				}
			}
		}

		if (bosshp <= 0 && bossdes == false) {
			bossdes = true;
			exit(0);


		}

		if (mchp <= 0) {
			mcdied = true;

		}

		if(mcdied){

			start2 = true;
			look = "i";
			HPlow = false;
			mcdied = false;
			shadowrotation = 0;
			shadowrotation2 = 90;
			shadowx = 0;
			shadowz = 0;
			shadowposx = 0;
			shadowposz = 0;
			level = 2;
			mcmoving = false;
			droneattacking = false;
			droneposx = 0;
			droneposz = 0;
			bossposx = 0;
			bossposz = 0;
			dronerotation = 0;
			bossrotation = 0;
			set = false;
			dronehp = 1;
			dronedes = false;
			bossdes = true;
			readytoattack = true;
			dronenum = 1;
			mchp = 10;
			mcdied;
			mchit = false;
			bosshp = 5;

			glutPostRedisplay();
		}





	}
	if (bosshp <= 0) {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	}

	glutSwapBuffers();
}

//=======================================================================
// Keyboard Function
//=======================================================================
// lesa level 1
void Keyboard(unsigned char key, int x, int y) {
	float d = 0.05;

	switch (key) {
	case 'w':
		camera.moveY(d);
		break;
	case 'a':
		camera.moveX(d);
		break;
	case 's':
		camera.moveY(-d);
		break;
	case 'd':
		camera.moveX(-d);
		break;
	case 'q':
		camera.moveZ(d);
		break;
	case 'e':
		camera.moveZ(-d);
		break;

	case 'i':
		if (level == 1) {
			if (look == "i") {
				if (sonicposx - 1 > -18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx - 1 > -4 || (sonicposz == 4 && !(sonicposx - 1 >= 1 && sonicposx - 1 <= 3) && sonicposx - 1 > -18) || (sonicposz == -4 && !(sonicposx - 1 >= 6 && sonicposx - 1 <= 8 || sonicposx - 1 >= -7 && sonicposx - 1 <= -5) && sonicposx - 1 > -18)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					sonicx -= 1;
					sonicrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if (sonicposx + 1 < 18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx + 1 < 5 || (sonicposz == 4 && !(sonicposx + 1 >= 1 && sonicposx + 1 <= 3) && sonicposx + 1 < 18) || (sonicposz == -4 && !(sonicposx + 1 >= 6 && sonicposx + 1 <= 8 || sonicposx + 1 >= -7 && sonicposx + 1 <= -5) && sonicposx + 1 < 18)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					sonicx += 1;
					sonicrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((sonicposz + 1 < 5 && !(sonicposx >= 1 && sonicposx <= 3)) || (sonicposz + 1 < 4 && sonicposx >= 1 && sonicposx <= 3)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					sonicz += 1;
					sonicrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((sonicposz - 1 > -5 && sonicposx <= -7) || (sonicposz - 1 > -5 && sonicposx >= 8) || (sonicposz - 1 > -24 && !(sonicposx <= -5 || sonicposx >= 6)) || (sonicposz - 1 > -4 && sonicposx > -7 && sonicposx <= -5) || (sonicposz - 1 > -4 && sonicposx < 8 && sonicposx >= 6) || (sonicposz - 1 > -23 && (sonicposx == -4 || sonicposx == 5))) {
					camera.center.z -= 1;
					camera.eye.z -= 1;
					mcmoving = true;
					sonicz -= 1;
					sonicrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		else if (level == 2) {
			if (look == "i") {
				if ((shadowposz + 1) < 27 && !(shadowposz + 1 > -5 && shadowposz + 1 < 5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					shadowz += 1;
					shadowrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if ((shadowposz - 1 > -27) && !(shadowposz - 1 < 4 && shadowposz - 1 > -5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z -= 1;
					camera.eye.z -= 1;
					mcmoving = true;
					shadowz -= 1;
					shadowrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((shadowposx + 1 < 29) && !(shadowposx + 1 > -4 && shadowposx + 1 < 4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					shadowx += 1;
					shadowrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((shadowposx - 1 > -29) && !(shadowposx - 1 < 4 && shadowposx - 1 > -4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					shadowx -= 1;
					shadowrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		break;

	case 'k':
		if (level == 1) {
			camRotLeft();
			camRotLeft();
			if (look == "i") {
				look = "k";
			}
			else if (look == "k") {
				look = "i";
			}
			else if (look == "j") {
				look = "l";
			}
			else if (look == "l") {
				look = "j";
			}
			if (look == "i") {
				if (sonicposx - 1 > -18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx - 1 > -4 || (sonicposz == 4 && !(sonicposx - 1 >= 1 && sonicposx - 1 <= 3) && sonicposx - 1 > -18) || (sonicposz == -4 && !(sonicposx - 1 >= 6 && sonicposx - 1 <= 8 || sonicposx - 1 >= -7 && sonicposx - 1 <= -5) && sonicposx - 1 > -18)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					sonicx -= 1;
					sonicrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if (sonicposx + 1 < 18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx + 1 < 5 || (sonicposz == 4 && !(sonicposx + 1 >= 1 && sonicposx + 1 <= 3) && sonicposx + 1 < 18) || (sonicposz == -4 && !(sonicposx + 1 >= 6 && sonicposx + 1 <= 8 || sonicposx + 1 >= -7 && sonicposx + 1 <= -5) && sonicposx + 1 < 18)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					sonicx += 1;
					sonicrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((sonicposz + 1 < 5 && !(sonicposx >= 1 && sonicposx <= 3)) || (sonicposz + 1 < 4 && sonicposx >= 1 && sonicposx <= 3)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					sonicz += 1;
					sonicrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((sonicposz - 1 > -5 && sonicposx <= -7) || (sonicposz - 1 > -5 && sonicposx >= 8) || (sonicposz - 1 > -24 && !(sonicposx <= -5 || sonicposx >= 6)) || (sonicposz - 1 > -4 && sonicposx > -7 && sonicposx <= -5) || (sonicposz - 1 > -4 && sonicposx < 8 && sonicposx >= 6) || (sonicposz - 1 > -23 && (sonicposx == -4 || sonicposx == 5))) {
					camera.center.z -= 1;
					camera.eye.z-= 1;
					mcmoving = true;
					sonicz -= 1;
					sonicrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		else if (level == 2) {
			camRotLeft();
			camRotLeft();
			if (look == "i") {
				look = "k";
			}
			else if (look == "k") {
				look = "i";
			}
			else if (look == "j") {
				look = "l";
			}
			else if (look == "l") {
				look = "j";
			}
			if (look == "i") {
				if ((shadowposz + 1) < 27 && !(shadowposz + 1 > -5 && shadowposz + 1 < 5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					shadowz += 1;
					shadowrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if ((shadowposz - 1 > -27) && !(shadowposz - 1 < 4 && shadowposz - 1 > -5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z -= 1;
					camera.eye.z -= 1;
					mcmoving = true;
					shadowz -= 1;
					shadowrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((shadowposx + 1 < 29) && !(shadowposx + 1 > -4 && shadowposx + 1 < 4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					shadowx += 1;
					shadowrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((shadowposx - 1 > -29) && !(shadowposx - 1 < 4 && shadowposx - 1 > -4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					shadowx -= 1;
					shadowrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		break;

	case 'j':
		if (level == 1) {
			camRotLeft();
			if (look == "i") {
				look = "j";
			}
			else if (look == "k") {
				look = "l";
			}
			else if (look == "j") {
				look = "k";
			}
			else if (look == "l") {
				look = "i";
			}
			if (look == "i") {
				if (sonicposx - 1 > -18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx - 1 > -4 || (sonicposz == 4 && !(sonicposx - 1 >= 1 && sonicposx - 1 <= 3) && sonicposx - 1 > -18) || (sonicposz == -4 && !(sonicposx - 1 >= 6 && sonicposx - 1 <= 8 || sonicposx - 1 >= -7 && sonicposx - 1 <= -5) && sonicposx - 1 > -18)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					sonicx -= 1;
					sonicrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if (sonicposx + 1 < 18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx + 1 < 5 || (sonicposz == 4 && !(sonicposx + 1 >= 1 && sonicposx + 1 <= 3) && sonicposx + 1 < 18) || (sonicposz == -4 && !(sonicposx + 1 >= 6 && sonicposx + 1 <= 8 || sonicposx + 1 >= -7 && sonicposx + 1 <= -5) && sonicposx + 1 < 18)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					sonicx += 1;
					sonicrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((sonicposz + 1 < 5 && !(sonicposx >= 1 && sonicposx <= 3)) || (sonicposz + 1 < 4 && sonicposx >= 1 && sonicposx <= 3)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					sonicz += 1;
					sonicrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((sonicposz - 1 > -5 && sonicposx <= -7) || (sonicposz - 1 > -5 && sonicposx >= 8) || (sonicposz - 1 > -24 && !(sonicposx <= -5 || sonicposx >= 6)) || (sonicposz - 1 > -4 && sonicposx > -7 && sonicposx <= -5) || (sonicposz - 1 > -4 && sonicposx < 8 && sonicposx >= 6) || (sonicposz - 1 > -23 && (sonicposx == -4 || sonicposx == 5))) {
					camera.center.z -= 1;
					camera.eye.z -= 1;
					mcmoving = true;
					sonicz -= 1;
					sonicrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		else if (level == 2) {
			camRotLeft();
			if (look == "i") {
				look = "j";
			}
			else if (look == "k") {
				look = "l";
			}
			else if (look == "j") {
				look = "k";
			}
			else if (look == "l") {
				look = "i";
			}
			if (look == "i") {
				if ((shadowposz + 1) < 27 && !(shadowposz + 1 > -5 && shadowposz + 1 < 5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					shadowz += 1;
					shadowrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if ((shadowposz - 1 > -27) && !(shadowposz - 1 < 4 && shadowposz - 1 > -5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z -= 1;
					camera.eye.z -= 1;
					mcmoving = true;
					shadowz -= 1;
					shadowrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((shadowposx + 1 < 29) && !(shadowposx + 1 > -4 && shadowposx + 1 < 4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					shadowx += 1;
					shadowrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((shadowposx - 1 > -29) && !(shadowposx - 1 < 4 && shadowposx - 1 > -4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					shadowx -= 1;
					shadowrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		break;

	case 'l':
		if (level == 1) {
			camRotLeft();
			camRotLeft();
			camRotLeft();
			if (look == "i") {
				look = "l";
			}
			else if (look == "k") {
				look = "j";
			}
			else if (look == "j") {
				look = "i";
			}
			else if (look == "l") {
				look = "k";
			}
			if (look == "i") {
				if (sonicposx - 1 > -18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx - 1 > -4 || (sonicposz == 4 && !(sonicposx - 1 >= 1 && sonicposx - 1 <= 3) && sonicposx - 1 > -18) || (sonicposz == -4 && !(sonicposx - 1 >= 6 && sonicposx - 1 <= 8 || sonicposx - 1 >= -7 && sonicposx - 1 <= -5) && sonicposx - 1 > -18)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					sonicx -= 1;
					sonicrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if (sonicposx + 1 < 18 && !(sonicposz <= -5) && sonicposz != 4 && sonicposz != -4 || sonicposz <= -5 && sonicposx + 1 < 5 || (sonicposz == 4 && !(sonicposx + 1 >= 1 && sonicposx + 1 <= 3) && sonicposx + 1 < 18) || (sonicposz == -4 && !(sonicposx + 1 >= 6 && sonicposx + 1 <= 8 || sonicposx + 1 >= -7 && sonicposx + 1 <= -5) && sonicposx + 1 < 18)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					sonicx += 1;
					sonicrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((sonicposz + 1 < 5 && !(sonicposx >= 1 && sonicposx <= 3)) || (sonicposz + 1 < 4 && sonicposx >= 1 && sonicposx <= 3)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					sonicz += 1;
					sonicrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((sonicposz - 1 > -5 && sonicposx <= -7) || (sonicposz - 1 > -5 && sonicposx >= 8) || (sonicposz - 1 > -24 && !(sonicposx <= -5 || sonicposx >= 6)) || (sonicposz - 1 > -4 && sonicposx > -7 && sonicposx <= -5) || (sonicposz - 1 > -4 && sonicposx < 8 && sonicposx >= 6) || (sonicposz - 1 > -23 && (sonicposx == -4 || sonicposx == 5))) {
					camera.center.z -= 1;
					camera.eye.z-= 1;
					mcmoving = true;
					sonicz -= 1;
					sonicrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		else if (level == 2)  {
			camRotLeft();
			camRotLeft();
			camRotLeft();
			if (look == "i") {
				look = "l";
			}
			else if (look == "k") {
				look = "j";
			}
			else if (look == "j") {
				look = "i";
			}
			else if (look == "l") {
				look = "k";
			}
			if (look == "i") {
				if ((shadowposz + 1) < 27 && !(shadowposz + 1 > -5 && shadowposz + 1 < 5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z += 1;
					camera.eye.z += 1;
					mcmoving = true;
					shadowz += 1;
					shadowrotation2 = 0;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "k") {
				if ((shadowposz - 1 > -27) && !(shadowposz - 1 < 4 && shadowposz - 1 > -5 && shadowposx > -4 && shadowposx < 4)) {
					camera.center.z -= 1;
					camera.eye.z -= 1;
					mcmoving = true;
					shadowz -= 1;
					shadowrotation2 = 180;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}

			}
			else if (look == "j") {
				if ((shadowposx + 1 < 29) && !(shadowposx + 1 > -4 && shadowposx + 1 < 4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x += 1;
					camera.eye.x += 1;
					mcmoving = true;
					shadowx += 1;
					shadowrotation2 = 90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
			else if (look == "l") {
				if ((shadowposx - 1 > -29) && !(shadowposx - 1 < 4 && shadowposx - 1 > -4 && shadowposz > -4 && shadowposz < 4)) {
					camera.center.x -= 1;
					camera.eye.x -= 1;
					mcmoving = true;
					shadowx -= 1;
					shadowrotation2 = -90;
				}
				else {
					PlaySound(TEXT("sounds/thud.wav"), NULL, SND_FILENAME | SND_ASYNC);
				}
			}
		}
		break;

	case 'v':
		viewToggle();
		break;

	case '1':
		camRotLeft();
		break;
	case '2':
		break;

	case GLUT_KEY_ESCAPE:
		exit(EXIT_SUCCESS);
	}

	glutPostRedisplay();
}

void Special(int key, int x, int y) {
	float d = 0.2;

	if (key == GLUT_KEY_UP) {
		camera.center.y += d;
	}
	if (key == GLUT_KEY_DOWN) {
		camera.center.y += -d;
	}
	if (key == GLUT_KEY_RIGHT) {
		camera.center.x += d;
		camera.center.z += -d;
	}
	if (key == GLUT_KEY_LEFT) {
		camera.center.x += -d;
		camera.center.z += d;
	}

	glutPostRedisplay();
}


//=======================================================================
// Motion Function
//=======================================================================
void myMotion(int x, int y)
{
	y = HEIGHT - y;

	if (cameraZoom - y > 0)
	{
		Eye.x += -0.1;
		Eye.z += -0.1;
	}
	else
	{
		Eye.x += 0.1;
		Eye.z += 0.1;
	}

	cameraZoom = y;

	glLoadIdentity();	//Clear Model_View Matrix

	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);	//Setup Camera with modified paramters

	GLfloat light_position[] = { 0.0f, 10.0f, 0.0f, 1.0f };
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);

	glutPostRedisplay();	//Re-draw scene 
}

//=======================================================================
// Mouse Function
////=======================================================================
void myMouse(int button, int state, int x, int y)
{

		if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {

			camCenterX = camera.center.x;
			camCenterZ = camera.center.z;
			if (level == 1) {
				camEyeX = camera.eye.x;
				camEyeZ = camera.eye.z;
				camera.center.x = camCenterX * cos(DEG2RAD(10)) + camCenterZ * sin(DEG2RAD(10)) + sonicposx - sonicposx * cos(DEG2RAD(10)) - sonicposz * sin(DEG2RAD(10));
				camera.center.z = -camCenterX * sin(DEG2RAD(10)) + camCenterZ * cos(DEG2RAD(10)) + sonicposz + sonicposx * sin(DEG2RAD(10)) - sonicposz * cos(DEG2RAD(10));
				camera.eye.x = camEyeX * cos(DEG2RAD(10)) + camEyeZ * sin(DEG2RAD(10)) + sonicposx - sonicposx * cos(DEG2RAD(10)) - sonicposz * sin(DEG2RAD(10));
				camera.eye.z = -camEyeX * sin(DEG2RAD(10)) + camEyeZ * cos(DEG2RAD(10)) + sonicposz + sonicposx * sin(DEG2RAD(10)) - sonicposz * cos(DEG2RAD(10));
			}
			else {
				camEyeX = camera.eye.x;
				camEyeZ = camera.eye.z;
				camEyeX = camera.eye.x;
				camEyeZ = camera.eye.z;
				camera.center.x = camCenterX * cos(DEG2RAD(10)) + camCenterZ * sin(DEG2RAD(10)) + shadowposx - shadowposx * cos(DEG2RAD(10)) - shadowposz * sin(DEG2RAD(10));
				camera.center.z = -camCenterX * sin(DEG2RAD(10)) + camCenterZ * cos(DEG2RAD(10)) + shadowposz + shadowposx * sin(DEG2RAD(10)) - shadowposz * cos(DEG2RAD(10));
				camera.eye.x = camEyeX * cos(DEG2RAD(10)) + camEyeZ * sin(DEG2RAD(10)) + shadowposx - shadowposx * cos(DEG2RAD(10)) - shadowposz * sin(DEG2RAD(10));
				camera.eye.z = -camEyeX * sin(DEG2RAD(10)) + camEyeZ * cos(DEG2RAD(10)) + shadowposz + shadowposx * sin(DEG2RAD(10)) - shadowposz * cos(DEG2RAD(10));
			}
		}
		if (button == GLUT_RIGHT_BUTTON && state == GLUT_DOWN) {
			camCenterX = camera.center.x;
			camCenterZ = camera.center.z;
			if (level == 1) {
				camEyeX = camera.eye.x;
				camEyeZ = camera.eye.z;
				camera.center.x = camCenterX * cos(DEG2RAD(-10)) + camCenterZ * sin(DEG2RAD(-10)) + sonicposx - sonicposx * cos(DEG2RAD(-10)) - sonicposz * sin(DEG2RAD(-10));
				camera.center.z = -camCenterX * sin(DEG2RAD(-10)) + camCenterZ * cos(DEG2RAD(-10)) + sonicposz + sonicposx * sin(DEG2RAD(-10)) - sonicposz * cos(DEG2RAD(-10));
				camera.eye.x = camEyeX * cos(DEG2RAD(-10)) + camEyeZ * sin(DEG2RAD(-10)) + sonicposx - sonicposx * cos(DEG2RAD(-10)) - sonicposz * sin(DEG2RAD(-10));
				camera.eye.z = -camEyeX * sin(DEG2RAD(-10)) + camEyeZ * cos(DEG2RAD(-10)) + sonicposz + sonicposx * sin(DEG2RAD(-10)) - sonicposz * cos(DEG2RAD(-10));
			}
			else {
				camEyeX = camera.eye.x;
				camEyeZ = camera.eye.z;
				camEyeX = camera.eye.x;
				camEyeZ = camera.eye.z;
				camera.center.x = camCenterX * cos(DEG2RAD(-10)) + camCenterZ * sin(DEG2RAD(-10)) + shadowposx - shadowposx * cos(DEG2RAD(-10)) - shadowposz * sin(DEG2RAD(-10));
				camera.center.z = -camCenterX * sin(DEG2RAD(-10)) + camCenterZ * cos(DEG2RAD(-10)) + shadowposz + shadowposx * sin(DEG2RAD(-10)) - shadowposz * cos(DEG2RAD(-10));
				camera.eye.x = camEyeX * cos(DEG2RAD(-10)) + camEyeZ * sin(DEG2RAD(-10)) + shadowposx - shadowposx * cos(DEG2RAD(-10)) - shadowposz * sin(DEG2RAD(-10));
				camera.eye.z = -camEyeX * sin(DEG2RAD(-10)) + camEyeZ * cos(DEG2RAD(-10)) + shadowposz + shadowposx * sin(DEG2RAD(-10)) - shadowposz * cos(DEG2RAD(-10));
			}
		}
	
}

//=======================================================================
// Reshape Function
//=======================================================================
void myReshape(int w, int h)
{
	if (h == 0) {
		h = 1;
	}

	WIDTH = w;
	HEIGHT = h;

	// set the drawable region of the window
	glViewport(0, 0, w, h);

	// set up the projection matrix 
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(fovy, (GLdouble)WIDTH / (GLdouble)HEIGHT, zNear, zFar);

	// go back to modelview matrix so we can move the objects about
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(Eye.x, Eye.y, Eye.z, At.x, At.y, At.z, Up.x, Up.y, Up.z);
}

//=======================================================================
// Assets Loading Function
//=======================================================================
void LoadAssets()
{
	// Loading Model files
	model_house.Load("Models/house/house.3DS");
	model_tree.Load("Models/tree/Tree1.3ds");
	fence.Load("Models/fence/fence.3ds");
	lamp.Load("Models/lamp/lamp-post.3Ds");
	basket.Load("Models/basket/basket.3DS ");
	atm.Load("Models/atm/atm.3dS ");
	fountain.Load("Models/fountain/fountain.3ds");
	bench.Load("Models/bench/bench.3ds");


	// Loading texture files
	tex_ground.Load("Textures/garden.bmp");
	tex_building.Load("Textures/building.bmp");
	tex_path.Load("Textures/path.bmp");
	tex_street.Load("Textures/street.bmp");
	sonic_fur.Load("Textures/sonic.bmp");
	sonic_belly.Load("Textures/sonicBelly.bmp");
	white.Load("Textures/white.bmp");
	sonic_iris.Load("Textures/sonicIris.bmp");
	redLeather.Load("Textures/redLeather.bmp");
	whiteLeather.Load("Textures/whiteLeather.bmp");
	metal.Load("Textures/metal.bmp");
	gold.Load("Textures/gold.bmp");
	redFur.Load("Textures/redFur.bmp");
	brownHair.Load("Textures/brownHair.bmp");
	loadBMP(&tex, "Textures/blu-sky-3.bmp", true);
	loadBMP(&tex_moon, "Textures/moon.bmp", true);
	loadBMP(&tex_sun, "Textures/sun.bmp", true);
}

//=======================================================================
// Main Function
//=======================================================================
void main(int argc, char** argv)
{
	
	glutInit(&argc, argv);

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);

	glutInitWindowSize(WIDTH, HEIGHT);

	glutInitWindowPosition(100, 150);

	glutCreateWindow(title);

	glutDisplayFunc(myDisplay);
	glutIdleFunc(Anim);

	glutKeyboardFunc(Keyboard);      // Sets the Keyboard handler function; called when a key is pressed
	glutSpecialFunc(Special);   // Sets the Keyboard handler function; called when a special key is pressed

	glutTimerFunc(10000, droneattack, 0);

	glutTimerFunc(5000, check, 0);

	glutKeyboardUpFunc(keyboardup);

	glutMotionFunc(myMotion);

	glutMouseFunc(myMouse);

	glutReshapeFunc(myReshape);
	cout << " " << sonicposx << " ";
	cout << " " << sonicposz << " ";

	myInit();

	LoadAssets();
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_NORMALIZE);
	glEnable(GL_COLOR_MATERIAL);

	glShadeModel(GL_SMOOTH);

	glutMainLoop();

}
