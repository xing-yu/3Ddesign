// implementation of BEZIER class

#include "bezier.h"
#include "utility.h"

void generateVertices(vector<GLfloatPoint> control, int degree);
void draw(void);
vector<GLfloatPoint> getVertices(void);

//<<<<<<<<<<<<<<<<<<<<<< generate vertices >>>>>>>>>>>>>>>>>>>>>>>
void Bezier:: generateVertices(vector<GLfloatPoint> control, int degree)
{
  // store all the vertices as the final output
  vector<GLfloatPoint> result;

  result = control;
  // generate vertices by degree
	for(int i = 0; i < degree; i++)
	{
    vector<GLfloatPoint> tempResult;
    vector<GLfloatPoint> firstHalf;
    vector<GLfloatPoint> secondHalf;


    firstHalf.push_back(result[0]);
    secondHalf.insert(secondHalf.begin(), result.back());

    // start the bezier method with the control points
    vector<GLfloatPoint> pre;
    // copy the control points
    pre = result;
		for(unsigned int j = 1; j < result.size(); j++)
		{
      // store vertices for each new level
      vector<GLfloatPoint> current;   

      // generate new vertices based on previous level
      for(unsigned int k = 0; k < result.size() - j; k++)
      {
        GLfloatPoint newVertex;
        newVertex.x = (pre[k].x + pre[k+1].x)*0.5;
        newVertex.y = (pre[k].y + pre[k+1].y)*0.5;
        newVertex.z = 0.0;

        //cout <<"x: "<< newVertex.x <<endl;
        //cout <<"y: "<< newVertex.y <<endl;
		    current.push_back(newVertex);
      }

      // add the first element to the first half
      firstHalf.push_back(current[0]);
      // add the last element to the seconde half
      secondHalf.insert(secondHalf.begin(), current.back());
      // update the previous points
      pre = current;
		}

    // remove the duplicated middle point
    firstHalf.pop_back();

    // combine the first and second half
    tempResult.reserve(firstHalf.size() + secondHalf.size());
    tempResult.insert(tempResult.end(), firstHalf.begin(), firstHalf.end());
    tempResult.insert(tempResult.end(), secondHalf.begin(), secondHalf.end());
	
    // copy temp result to result
    result = tempResult;
  }

  // update the vector in the object
  this -> curveVertices = result;
}

//<<<<<<<<<<<<<<<<<<<<<<<< draw curve >>>>>>>>>>>>>>>>>>>>>>>
void Bezier:: draw(void)
{
  // if the vertices vector is not empty
  if(!curveVertices.empty())
  {
    glBegin(GL_LINE_STRIP);
    for(unsigned int i = 0; i < this -> curveVertices.size(); i++)
    {
      glVertex3f(this -> curveVertices[i].x, this -> curveVertices[i].y, this -> curveVertices[i].z);
    }
    glEnd();
    glFlush();
  }
  else
  {
    cout << "The bezier curve vector is empty!\n";
  }
}

//<<<<<<<<<<<<<<<<<<<<<<< get vertices >>>>>>>>>>>>>>>>>>>>>
vector<GLfloatPoint> Bezier:: getVertices(void)
{
  return this -> curveVertices;
}
