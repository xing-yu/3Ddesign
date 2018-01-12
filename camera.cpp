// implementation of CAMERA class

#include "camera.h"
#include "gfp.h"
#include "utility.h"
#include <vector>

#define PI 3.14159265

void setCamera(
		float eyex, float eyey, float eyez, 
		float lookx, float looky, float lookz, 
		float upx, float upy, float upz);
GLfloatPoint crossProduct(GLfloatPoint a, GLfloatPoint b);
GLfloat dotProduct(GLfloatPoint a, GLfloatPoint b);
GLfloatPoint normalize(GLfloatPoint a);

GLfloatPoint getEye(void);

void roll(float angle);
void pitch(float angle);
void yaw(float angle);
void slide(float delU, float delV, float delN);
void setModelViewMatrix(void);

//<<<<<<<<<<<<<<<<<<<<<<<< implementation >>>>>>>>>>>>>>>>>>>>>>
GLfloatPoint Camera:: crossProduct(GLfloatPoint a, GLfloatPoint b)
{
	GLfloatPoint result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;

	return result;
}

GLfloatPoint Camera:: normalize(GLfloatPoint a)
{
	float norm;
	GLfloatPoint result;
	norm = a.x*a.x + a.y*a.y + a.z*a.z;
	norm = sqrt(norm);

	result.x = a.x/norm;
	result.y = a.y/norm;
	result.z = a.z/norm;

	return result;
}

GLfloat Camera:: dotProduct(GLfloatPoint a, GLfloatPoint b)
{
	GLfloat result;
	result = a.x*b.x + a.y*b.y + a.z*b.z;

	return result;
}

void Camera:: setCamera(
		float eyex, float eyey, float eyez, 
		float lookx, float looky, float lookz, 
		float upx, float upy, float upz)
{
	GLfloatPoint eye, look, up;
	eye.x = eyex;
	eye.y = eyey;
	eye.z = eyez;
	look.x = lookx;
	look.y = looky;
	look.z = lookz;
	up.x = upx;
	up.y = upy;
	up.z = upz;

	// set eye look and up
	this -> eyeLookUp.push_back(eye);
	this -> eyeLookUp.push_back(look);
	this -> eyeLookUp.push_back(up);

	/*
	cout << eyeLookUp[2].x << endl;
	cout << eyeLookUp[2].y << endl;
	cout << eyeLookUp[2].z << endl;
	*/

	// calculate n
	GLfloatPoint n;
	n.x = eyeLookUp[0].x - eyeLookUp[1].x;
	n.y = eyeLookUp[0].y - eyeLookUp[1].y;
	n.z = eyeLookUp[0].z - eyeLookUp[1].z;

	// calculate u, u = up crossproduct n
	GLfloatPoint u;
	u = crossProduct(eyeLookUp[2], n);

	// calculate v, v = n crossproduct u
	GLfloatPoint v;
	v = crossProduct(n, u);

	// normalize n u v
	n = normalize(n);
	u = normalize(u);
	v = normalize(v);

	// set nuv
	this -> nuv.push_back(n);
	this -> nuv.push_back(u);
	this -> nuv.push_back(v);

	// set modelview matrix
	setModelViewMatrix();
}

void Camera:: setModelViewMatrix(void)
{
	float m[16];
	GLfloatPoint u, v, n, eye;
	eye = this -> eyeLookUp[0];
	u = this -> nuv[1];
	v = this -> nuv[2];
	n = this -> nuv[0];

	m[0] = u.x; m[4] = u.y; m[8] = u.z; m[12] = -(eye.x*u.x + eye.y*u.y + eye.z*u.z);
	m[1] = v.x; m[5] = v.y; m[9] = v.z; m[13] = -(eye.x*v.x + eye.y*v.y + eye.z*v.z);
	m[2] = n.x; m[6] = n.y; m[10] = n.z; m[14] = -(eye.x*n.x + eye.y*n.y + eye.z*n.z);
	m[3] = 0; m[7] = 0; m[11] = 0; m[15] = 1.0;


	glMatrixMode(GL_MODELVIEW);

	glLoadMatrixf(m);

}

void Camera:: slide(float delU, float delV, float delN)
{
	GLfloatPoint u, v, n;

	u = this -> nuv[1];
	v = this -> nuv[2];
	n = this -> nuv[0];

	this -> eyeLookUp[0].x += delU*u.x + delV*v.x + delN*n.x;
	this -> eyeLookUp[0].y += delU*u.y + delV*v.y + delN*n.y;
	this -> eyeLookUp[0].z += delU*u.z + delV*v.z + delN*n.z;

	this -> eyeLookUp[1].x += delU*u.x + delV*v.x + delN*n.x;
	this -> eyeLookUp[1].y += delU*u.y + delV*v.y + delN*n.y;
	this -> eyeLookUp[1].z += delU*u.z + delV*v.z + delN*n.z;

	setModelViewMatrix();
}

void Camera:: roll(float angle)
{
	float s, c;
	c = cos(angle*(PI/180.0));
	s = sin(angle*(PI/180.0));

	// keep a copy of u
	GLfloatPoint tu;

	tu.x = this -> nuv[1].x;
	tu.y = this -> nuv[1].y;
	tu.z = this -> nuv[1].z;

	this -> nuv[1].x = c*tu.x - s*(this -> nuv[2].x);
	this -> nuv[1].y = c*tu.y - s*(this -> nuv[2].y);
	this -> nuv[1].z = c*tu.z - s*(this -> nuv[2].z);

	this -> nuv[2].x = s*tu.x + c*(this -> nuv[2].x);
	this -> nuv[2].y = s*tu.y + c*(this -> nuv[2].y);
	this -> nuv[2].z = s*tu.z + c*(this -> nuv[2].z);

	setModelViewMatrix();
}
void Camera:: pitch(float angle)
{
	float s, c;
	c = cos(angle*(PI/180.0));
	s = sin(angle*(PI/180.0));

	// keep a copy of n
	GLfloatPoint tn;

	tn.x = this -> nuv[0].x;
	tn.y = this -> nuv[0].y;
	tn.z = this -> nuv[0].z;

	this -> nuv[0].x = c*tn.x - s*(this -> nuv[2].x);
	this -> nuv[0].y = c*tn.y - s*(this -> nuv[2].y);
	this -> nuv[0].z = c*tn.z - s*(this -> nuv[2].z);

	this -> nuv[2].x = c*(this -> nuv[2].x) - s*tn.x;
	this -> nuv[2].y = c*(this -> nuv[2].y) - s*tn.y;
	this -> nuv[2].z = c*(this -> nuv[2].z) - s*tn.z;

	setModelViewMatrix();
}

void Camera:: yaw(float angle)
{
	float s, c;
	c = cos(angle*(PI/180.0));
	s = sin(angle*(PI/180.0));

	// keep a copy of u
	GLfloatPoint tn;

	tn.x = this -> nuv[0].x;
	tn.y = this -> nuv[0].y;
	tn.z = this -> nuv[0].z;

	this -> nuv[0].x = c*tn.x + s*(this -> nuv[1].x);
	this -> nuv[0].y = c*tn.y + s*(this -> nuv[1].y);
	this -> nuv[0].z = c*tn.z + s*(this -> nuv[1].z);

	this -> nuv[1].x = c*(this -> nuv[1].x) - s*tn.x;
	this -> nuv[1].y = c*(this -> nuv[1].y) - s*tn.y;
	this -> nuv[1].z = c*(this -> nuv[1].z) - s*tn.z;

	setModelViewMatrix();
}

GLfloatPoint Camera:: getEye(void)
{
	return this -> eyeLookUp[0];
}
