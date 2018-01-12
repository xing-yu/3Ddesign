#ifndef MESH_H
#define MESH_H

#include "gfp.h"
#include <vector>

using namespace std;
// header file MESH.H for class mesh
// a class object can create mesh based on intput vertices

class Mesh
{
public:
	void generateChordAndNormals(vector<GLfloatPoint> input, int size);
	// generate normals for vertices in input
	// size is the length of the control point vector

  	void rotate(int cut);
  	// the function that generate vertices by rotating
  	// the input points for 360 degrees around the y axis
  	// cut is the number of cut on the rotation
  	// size is the length of the control point vector

  	void generateSurfaces(int cut);

  	void draw(void);

    void drawPoly(void);

  	vector<GLfloatPoint> getVertices(void);
  	// get the vertices of the meshes
  	vector<GLfloatPoint> getNormals(void);
  	// get the normals, one for each vertex
  	vector< vector<int> > getSurface(void);
  	// get the surface vertices' indice

private:
    GLfloatPoint crossProduct(GLfloatPoint a, GLfloatPoint b);

  	vector<GLfloatPoint> meshVertices;
  	// a vector that stores the vertices generated 
  	// from input points and cut

  	vector<GLfloatPoint> vertexNormals;
  	// store the normals for each vertex

  	vector< vector<int> > surfaceIndex;
  	// store the index of vertices on a surface

  	vector<GLfloatPoint> chordVertices;
  	// store the points on the chord

  	vector<GLfloatPoint> chordNormals;
  	// store the normals of chord points
};

#endif //MESH_H