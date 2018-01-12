#ifndef CAMERA_H
#define CAMERA_H

#include "gfp.h"
#include "utility.h"
#include <vector>

using namespace std;
// header file CAMERA.H for class camera

class Camera
{
public:
	void setCamera(
		float eyex, float eyey, float eyez, 
		float lookx, float looky, float lookz, 
		float upx, float upy, float upz);
	void roll(float angle);
	void pitch(float angle);
	void yaw(float angle);
	void slide(float delU, float delV, float delN);
	GLfloatPoint getEye(void);
	
private:
	vector<GLfloatPoint> eyeLookUp;
	vector<GLfloatPoint> nuv;

	double viewAngle, aspect, near, far;
	void setModelViewMatrix(void);

	GLfloatPoint crossProduct(GLfloatPoint a, GLfloatPoint b);
	GLfloatPoint normalize(GLfloatPoint a);
	GLfloat dotProduct(GLfloatPoint a, GLfloatPoint b);

};

#endif //BEZIER_H