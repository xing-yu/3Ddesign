#ifndef BEZIER_H
#define BEZIER_H

#include "gfp.h"
#include <vector>

using namespace std;
// header file BEZIER.H for class Bezier curve
// a class object can create a Bezier curve points
// based on given control points and degree
// and draw the curve

class Bezier
{
public:
  	void generateVertices(vector<GLfloatPoint> control, int degree);
  	// the function that generate vertices
  	void draw(void);
  	// draw the Bezier Curve
  	vector<GLfloatPoint> getVertices(void);
  	// get the generated vertices

private:
  	vector<GLfloatPoint> curveVertices;
  	// a vector that stores the vertices generated 
  	// from control points and degree
};

#endif //BEZIER_H
