// implementation of MESH class

#include "mesh.h"
#include "utility.h"

#define PI 3.14159265

void generateChordAndNormals(int size);
// input is NOT chord but all vertice on the Bezier curve
// generate normals for vertices in input
// size is the length of the control point vector

void rotate(int cut);
// the function that generate vertices by rotating
// the input points for 360 degrees around the y axis
// cut is the number of cut on the rotation

void generateSurfaces(int cut);
// generate surfaces, which includes the indices of vertices on a surface

void draw(void);

void drawPoly(void);

GLfloatPoint crossProduct(GLfloatPoint a, GLfloatPoint b);

vector<GLfloatPoint> getVertices(void);
// get the vertices of the meshes
vector<GLfloatPoint> getNormals(void);
// get the normals, one for each vertex
vector< vector<int> > getSurface(void);
// get the surface vertices' indice

//<<<<<<<<<<<<<<<<<<<<<< implementations >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
GLfloatPoint Mesh:: crossProduct(GLfloatPoint a, GLfloatPoint b)
{
	GLfloatPoint result;
	result.x = a.y*b.z - a.z*b.y;
	result.y = a.z*b.x - a.x*b.z;
	result.z = a.x*b.y - a.y*b.x;

	return result;
}

void Mesh:: generateChordAndNormals(vector<GLfloatPoint> input, int size)
{
	// input is not the chord points
	// need to find chord points and
	// generate normals for each chord vertex

	int n = size - 1;

	vector<GLfloatPoint> chordPoints;
	vector<GLfloatPoint> chordNormals;

	// mark the type of the point of a chord: 1 is head, 2 is tail
	int type = 1; 

	GLfloatPoint Z;
	Z.x = 0;
	Z.y = 0;
	Z.z = 1;

	for(unsigned i = 0; i < input.size(); i++)
	{
		if(i%n == 0)
		{
			// then it is a cord point
			chordPoints.push_back(input[i]);

			if(type == 1)
			{
				// normal vector of a head is input[i+1] - input[i]
				GLfloatPoint newNormal;
				newNormal.x = input[i+1].x - input[i].x;
				newNormal.y = input[i+1].y - input[i].y;
				newNormal.z = input[i+1].z - input[i].z;

				// calculate new normal by crossproduct
				newNormal = crossProduct(Z, newNormal);

				// normalize vector
				float norm = sqrt((newNormal.x)*(newNormal.x) 
					+ (newNormal.y)*(newNormal.y) 
					+ (newNormal.z)*(newNormal.z));

				newNormal.x = newNormal.x/norm;
				newNormal.y = newNormal.y/norm;
				newNormal.z = newNormal.z/norm;

				// add new normal
				chordNormals.push_back(newNormal);

				type = 2;
				// set to be tail
			}
			else
			{
				// normal vector of a tail is input[i] - input[i-1]
				GLfloatPoint newNormal;
				newNormal.x = input[i].x - input[i-1].x;
				newNormal.y = input[i].y - input[i-1].y;
				newNormal.z = input[i].z - input[i-1].z;

				// calculate new normal by crossproduct
				newNormal = crossProduct(Z, newNormal);

				// normalize vector
				float norm = sqrt((newNormal.x)*(newNormal.x) 
					+ (newNormal.y)*(newNormal.y) 
					+ (newNormal.z)*(newNormal.z));

				newNormal.x = newNormal.x/norm;
				newNormal.y = newNormal.y/norm;
				newNormal.z = newNormal.z/norm;

				// add new normal
				chordNormals.push_back(newNormal);

				type = 1;
				// set to be head
			}
		}
	}

	this -> chordVertices = chordPoints;
	this -> chordNormals = chordNormals;

}	

void Mesh:: rotate(int cut)
{
	vector<GLfloatPoint> verticeResult;
	vector<GLfloatPoint> normalResult;

	for(int i = 0; i < cut; i++)
	{
		// calculate sin and cosine theta
		float degree = i*360/cut;
		float rad = degree*PI/180;
		float s;
		float c;
		s = sin(rad);
		c = cos(rad);

		// generate and add vertices
		for(unsigned int j = 0; j < this -> chordVertices.size(); j++)
		{
			GLfloatPoint newVertex;
			newVertex.x = (GLfloat)(this -> chordVertices[j].x*c + this -> chordVertices[j].z*s);
			newVertex.y = (GLfloat)(this -> chordVertices[j].y);
			newVertex.z = (GLfloat)(this -> chordVertices[j].x*(-s) + this -> chordVertices[j].z*c);

			verticeResult.push_back(newVertex);
		}

		// generate and add normals
		for(unsigned int j = 0; j < this -> chordNormals.size(); j++)
		{
			GLfloatPoint newNormal;
			newNormal.x = (GLfloat)(this -> chordNormals[j].x*c + this -> chordNormals[j].z*s);
			newNormal.y = (GLfloat)(this -> chordNormals[j].y);
			newNormal.z = (GLfloat)(this -> chordNormals[j].x*(-s) + this -> chordNormals[j].z*c);

			normalResult.push_back(newNormal);
		}
	}

	// save the result: all vertices of the 3D object
	this -> meshVertices = verticeResult;
	this -> vertexNormals = normalResult;
}

void Mesh:: generateSurfaces(int cut)
{
	vector< vector<int> > surfaceResult;

	int n = this -> chordVertices.size();
	int total = n*cut;

	for(unsigned j = 0; j < cut; j++)
	{
		for(unsigned i = 1; i < n; i++)
		{
			vector<int> newSurface;
			newSurface.push_back((i+j*n)%total);
			newSurface.push_back((i+n+j*n)%total);
			newSurface.push_back((i+n-1+j*n)%total);
			newSurface.push_back((i-1+j*n)%total);

			surfaceResult.push_back(newSurface);
		}
	}

	this -> surfaceIndex = surfaceResult;
}

void Mesh:: draw(void)
{
	for(unsigned i = 0; i < this -> surfaceIndex.size(); i++)
	{
		glBegin(GL_LINE_LOOP);
		for(unsigned j = 0; j < this->surfaceIndex[i].size(); j++)
		{
			int k = this->surfaceIndex[i][j];
			
			glVertex3f(
				this -> meshVertices[k].x, 
				this -> meshVertices[k].y, 
				this -> meshVertices[k].z);
			
		}
		glEnd();
	}
	glFlush();
}

void Mesh:: drawPoly(void)
{
	float r = 0.0;
	float g = 0.0;
	float b = 0.5;
	for(unsigned i = 0; i < this -> surfaceIndex.size(); i++)
	{
		glBegin(GL_POLYGON);
			for(unsigned j = 0; j < this->surfaceIndex[i].size(); j++)
			{
				//r += 0.0001;
				//g += 0.0002;
				//b += 0.0000005;
				int k = this->surfaceIndex[i][j];
				
				//glColor3f(r, g, b);
				glNormal3f(
					this->vertexNormals[k].x, 
					this->vertexNormals[k].y, 
					this->vertexNormals[k].z);
				glTexCoord3f(
					this->meshVertices[k].x, 
					this->meshVertices[k].y, 
					this->meshVertices[k].z);
				glVertex3f(
					this->meshVertices[k].x, 
					this->meshVertices[k].y, 
					this->meshVertices[k].z);
				
				
			}

		glEnd();
	}
	glFlush();
}

vector<GLfloatPoint> Mesh:: getVertices(void)
{
	return this -> meshVertices;
}
// get the vertices of the meshes
vector<GLfloatPoint> Mesh:: getNormals(void)
{
	return this -> vertexNormals;
}
// get the normals, one for each vertex
vector< vector<int> > Mesh:: getSurface(void)
{
	return this -> surfaceIndex;
}
// get the surface vertices' indice