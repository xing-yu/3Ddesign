// 3D design
// The main implementation of the 3D design software

#include <stdio.h>
#include "utility.h"
#include "bezier.h"
#include "mesh.h"
#include "gfp.h"
#include "camera.h"
#include "filer.h"

#define ADD_MODE 0
#define EDIT_MODE 1
#define CLEARSCREEN 3
#define CUT 100

int mode = 0;			// default mode is add
int selected = -1;		// index of selected control point
int bezierDegree = 7; 	// the degree of Bezier curve
int rolling = -1;		// switch the rolling mode -1: off 1: on
int TDmode = -1; 		// -1: 3D 1: wireframe
int light0switch = 1;	// -1: on 1: off
int light1switch = 1;	// -1: on 1: off
int texturing = -1;   	// -1: off 1: on
int showControl = 1;	// show control polyline-- -1: off, 1: on
// texture name
int ployTexture = 1;

// camera value
float eyex = 0; 
float eyey = 1.2; 
float eyez = -3.0; 
float lookx = 0; 
float looky = 0.5; 
float lookz = 0;
float upx = 0; 
float upy = 1.0; 
float upz = 0;

// camera move
float sensitivity = 0.5;

float yawAngle_pre = 0;
float rollAngle_pre = 0;
float pitchAngle_pre = 0;

float yawAngle = 0;
float rollAngle = 0;
float pitchAngle = 0;

float delU = 0;
float delV = 0;
float delN = 0;

int xstart = -1;	// mark the starting point
int ystart = -1;	// mark the ending point


float width = 1280.0;
float height = 640.0;
float aspectRatiro = width/height;

vector<GLfloatPoint> controlPoints;
//<<<<<<<<<<<<<<<< initialize displays >>>>>>>>>>>>
void initDisplay()
{
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(1.0f, 0.0f, 0.0f);
}

//<<<<<<<<<<<<<<<<< display function >>>>>>>>>>>>>>>
void display(void)
{
	//glClearDepth(1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glClear(GL_COLOR_BUFFER_BIT);
	glColor3f(1.0f, 0.0f, 0.0f);

    // viewport #1: for drawing curves
    glViewport(width/2, 0, width/2, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, 1.0, 0.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // draw a line to separate the drawing and displaying areas
    glLineWidth(4.0);
    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINE_LOOP);
    glVertex3f(0.0,0.0,0.0);
    glVertex3f(0.0,1.0,0.0);
    glVertex3f(1.0,1.0,0.0);
    glVertex3f(1.0,0.0,0.0);
    glEnd();


    glDisable(GL_LIGHTING);
    // draw control points (testing)
    glPointSize(8.0);
    glLineWidth(4.0);
    glColor3f(0.0f, 1.0f, 0.0f);
    if(showControl == 1)
    {
	    glBegin(GL_LINE_STRIP);
	    	for(unsigned int i = 0; i < controlPoints.size(); i++)
	    	{
	    		glVertex3f(controlPoints[i].x, controlPoints[i].y, controlPoints[i].z);
	    	}
	    glEnd();
	}

    glLineWidth(4.0);
    glColor3f(1.0f, 0.0f, 0.0f);
    // generate and draw Bezier curve
    vector<GLfloatPoint> curvePoints;
    if(controlPoints.size() > 2)
    {
	    Bezier bcurve;

	    bcurve.generateVertices(controlPoints, bezierDegree);

	    curvePoints = bcurve.getVertices();

	    bcurve.draw();
	}	

	// viewport #2: for drawing 3D object
	glViewport(0, 0, width/2, height);

	// projection and modelview matrices
	glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(40, 1, 1, 100);
    glMatrixMode(GL_MODELVIEW);

    glLoadIdentity();
 
    // set light source 2
    if(light1switch == 1 && TDmode == -1)
	{
		// enable lighting
		glEnable(GL_LIGHTING);
		glEnable(GL_LIGHT1);
		GLfloatPoint currentEye;
		//currentEye = cam.getEye();
		GLfloat light1pos[] = {eyex, eyey, eyez, 0.0};
		glLightfv(GL_LIGHT1, GL_POSITION, light1pos);
	}

    // set camera
    Camera cam;
    cam.setCamera(eyex, eyey, eyez, lookx, looky, lookz, upx, upy, upz);
    cam.slide(delU, delV, delN);
    cam.yaw(yawAngle + yawAngle_pre);
    cam.pitch(pitchAngle + pitchAngle_pre);
    cam.roll(rollAngle + rollAngle_pre);

	

    // draw 3D object
    if(controlPoints.size() > 2)
    {
    	int size = controlPoints.size();
	    // draw 3D object
	    Mesh threeD;
	    threeD.generateChordAndNormals(curvePoints, size);
	    threeD.rotate(CUT);
	    threeD.generateSurfaces(CUT);

		if(TDmode == -1)
		{	
			if(light0switch == 1)
			{
				// enable lighting
				glEnable(GL_LIGHTING);
				glEnable(GL_LIGHT0);
				GLfloat light0pos[] = {1.0, 1.0, -2., 1.0};
		    	glLightfv(GL_LIGHT0, GL_POSITION, light0pos);
			}

			if(texturing == -1)
			{
				// set lighting color
				GLfloat cyan[] = {0.f, .8f, .8f, 1.f};
				GLfloat white[] = {0.5f, 0.5f, 0.5f, 1.0f};
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, cyan);
				glMaterialfv(GL_FRONT, GL_SPECULAR, white);
				GLfloat shininess[] = {20};
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
			}

			if(light0switch == -1 && light1switch == -1)
			{
				if(texturing == -1)
				{
					glColor3f(0.f, .8f, .8f);
				}
				else
				{
					glColor3f(0.8f, .8f, .8f);
				}
			}

			if(texturing == 1)
			{
				// set color
				GLfloat white[] = {0.8f, 0.8f, 0.8f, 0.0f};
				glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, white);
				glMaterialfv(GL_FRONT, GL_SPECULAR, white);
				GLfloat shininess[] = {20};
				glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, shininess);

				// texture mode
				Filer picfile;
				GLubyte* texture;
				int width;
				int height;
				glBindTexture(GL_TEXTURE_2D, ployTexture);
				texture = picfile.ppmRead("tp.ppm", &width, &height);
				gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
		                 GL_RGB, GL_UNSIGNED_BYTE, texture);

				glEnable(GL_TEXTURE_2D);
				glBindTexture(GL_TEXTURE_2D, ployTexture);
			}
			threeD.drawPoly();

			glDisable(GL_TEXTURE_2D);
		}
		else
		{
			glDisable(GL_LIGHTING);
			glLineWidth(1.0);
    		glColor3f(1.0f, 0.0f, 0.0f);
	    	threeD.draw();
	    	//glEnable(GL_LIGHTING);
	    }
    
	}
	
	

    glFlush();

    glutSwapBuffers();
}

//<<<<<<<<<<<<<<<<<< reshape functio>>>>>>>>>>>>>>>>
void reshape(int w, int h)
{
	width = h*aspectRatiro;
	height = h;
	display();
}

//<<<<<<<<<<<<<<<<<<< mouse function >>>>>>>>>>>>>>>
void myMouse(int button, int state, int x, int y)
{

	if(mode == ADD_MODE)	// add point
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			// check if x, y are in the drawing area
			if(x > 0.5*width && x < width)
			{
				if(y > 0 && y < height)
				{
					GLfloatPoint newPoint;
					newPoint.x = (x - 0.5*width)/(0.5*width);
					newPoint.y = (height - y)/height;
					newPoint.z = 0.0;
					controlPoints.push_back(newPoint);

					display();

				}
			}
	
			pitchAngle_pre += pitchAngle;
			rollAngle_pre += rollAngle;
			yawAngle_pre += yawAngle; 

			pitchAngle = 0;
			rollAngle = 0;
			yawAngle = 0;
						
		}
		// mark the starting point
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			// check if x, y are in the 3D panel
			if(x > 0.0 && x < 0.5*width)
			{
				if(y > 0 && y < height)
				{
					xstart = x;
					ystart = y;
				}
			}
			else
			{
				xstart = -1;
				ystart = -1;
			}
		}
	}
	else				// select point
	{
		if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
		{
			// check if x, y are in the drawing area
			if(x > 0.5*width && x < width)
			{
				if(y > 0 && y < height)
				{
					// reset selected point
					selected = -1;

					// convert to wcs
					GLfloat cx = (x - 0.5*width)/(0.5*width);
					GLfloat cy = (height - y)/height;

					float minDistance = -1.0;

					for(unsigned i = 0; i < controlPoints.size(); i++)
					{

						float distance;
						distance = 
						(controlPoints[i].x - cx)*(controlPoints[i].x - cx)
						+ 
						(controlPoints[i].y - cy)*(controlPoints[i].y - cy);

						if(minDistance == -1.0)
						{
							minDistance = distance;
							selected = i;
						}
						else
						{
							if(minDistance > distance)
							{
								minDistance = distance;
								selected = i;
							}
						}
					}

				}
			}

			// mark the starting point

			if(x > 0.0 && x < 0.5*width)
			{
				if(y > 0 && y < height)
				{
					xstart = x;
					ystart = y;
				}
			}
			else	// prevent accidental moving
			{
				xstart = -1;
				ystart = -1;
			}
	
		}

		// save and clearn yaw pitch and roll angle
		if(button == GLUT_LEFT_BUTTON && state == GLUT_UP)
		{
			pitchAngle_pre += pitchAngle;
			rollAngle_pre += rollAngle;
			yawAngle_pre += yawAngle; 

			pitchAngle = 0;
			rollAngle = 0;
			yawAngle = 0;
		}
	}
}

//<<<<<<<<<<<<<<<<<< motion function >>>>>>>>>>>>>>>
void myMotion(int x, int y)
{
	// add mode: add new vertex
	if(mode == ADD_MODE)
	{
		if(x > 0.5*width && x < width)	// move the new point
		{

			if(y > 0 && y < height && xstart == -1 && ystart == -1)
			{
				GLfloatPoint newPoint;
				newPoint.x = (x - 0.5*width)/(0.5*width);
				newPoint.y = (height - y)/height;
				newPoint.z = 0.0;
				controlPoints.push_back(newPoint);

				display();

				controlPoints.pop_back();

			}
		}

		if(x > 0.0 && x < 0.5*width)	// move the camera
		{
			if(y > 0 && y < height)
			{
				if(xstart != -1 && ystart != -1)
				{
					if(rolling == -1)
					{
						yawAngle = (float)((xstart - x)*180/width)*sensitivity;
						pitchAngle = (float)((y - ystart)*180/height)*sensitivity;
					}
					else
					{
						rollAngle = (float)((y - ystart)*180/height)*sensitivity;
					}
					display();
				}

			}
		}

	}
	else
	{
		// if a point is selected
		if(selected != -1)
		{
			if(x > 0.5*width && x < width)
			{

				if(y > 0 && y < height && xstart == -1 && ystart == -1)
				{
					controlPoints[selected].x = (x - 0.5*width)/(0.5*width);
					controlPoints[selected].y = (height - y)/height;
					controlPoints[selected].z = 0.0;
					display();
				}
			}
		}

		if(x > 0.0 && x < 0.5*width)	// move the camera
		{
			if(y > 0 && y < height)
			{
				if(xstart != -1 && ystart != -1)
				{
					if(rolling == -1)
					{
						yawAngle = (float)((xstart - x)*180/width)*sensitivity;
						pitchAngle = (float)((y - ystart)*180/height)*sensitivity;
					}
					else
					{
						rollAngle = (float)((y - ystart)*180/height)*sensitivity;
					}
					display();
				}
			}
		}
	}
}

//<<<<<<<<<<<<<<< keyboard function >>>>>>>>>>>>>>>>
void SpecialInput(int key, int x, int y)
{
  switch(key)
  {
    case GLUT_KEY_UP: delV += 0.2; display(); break;
    case GLUT_KEY_DOWN: delV -= 0.2; display();break;
    case GLUT_KEY_LEFT: delU -= 0.2; display();break;
    case GLUT_KEY_RIGHT: delU += 0.2; display();break;
  }
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	switch(theKey)
	{
		//case 'd': deleteDot(); break;
    	//case 'z': if(zroll == 0){zroll = 1;}else{zroll = 0;} break;
    	case 'z': delN += 0.2; display(); break;
    	case 'x': delN -= 0.2; display(); break;
		case ' ':
			yawAngle_pre = 0;
			rollAngle_pre = 0;
			pitchAngle_pre = 0;

			yawAngle = 0;
			rollAngle = 0;
			pitchAngle = 0;

			delU = 0;
			delV = 0;
			delN = 0;

			xstart = -1;	
			ystart = -1;
		display();
		break;
		case 'r': if(rolling == -1){rolling = 1;}else{rolling = -1;}break;
		case 't': if(TDmode == -1){TDmode = 1;}else{TDmode = -1;}display();break;
		case '1': if(light0switch == -1){light0switch = 1;}else{light0switch = -1;}display();break;
		case '2': if(light1switch == -1){light1switch = 1;}else{light1switch = -1;}display();break;
		case 'e': if(texturing == -1){texturing = 1;}else{texturing = -1;}display();break;
		case '+': if(bezierDegree < 50){bezierDegree += 1;}display();break;
		case '-': if(bezierDegree > 1){bezierDegree -= 1;}display();break;
		case 'c': if(showControl == -1){showControl = 1;}else{showControl = -1;}display();break;	// toggle control polyline
		case 'd': if(selected != -1 && mode == EDIT_MODE){controlPoints.erase(controlPoints.begin() + selected);}display();break;
		case 'q': exit(0); break;
	}

}
//<<<<<<<<<<<<<<<<<<< menu function >>>>>>>>>>>>>>>>
void changeMode(int option)
{
	switch(option)
	{
		case ADD_MODE: mode = ADD_MODE; break;
		case EDIT_MODE: mode = EDIT_MODE; break;
		case CLEARSCREEN: mode = ADD_MODE; controlPoints.clear(); display(); break;
	}
}
//<<<<<<<<<<<<<<<<<<< main function >>>>>>>>>>>>>>>>
int main(int argc, char** argv)
{
	//<<<<<<<<<<<< initialize window >>>>>>>>>>>>>
  	glutInit(&argc, argv);
  	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  	glutInitWindowPosition(100,100);
	glutInitWindowSize(width,height);
	glutCreateWindow("3D Interactive Design");

	// depth buffer
	glEnable (GL_DEPTH_TEST);
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//<<<<<<<< register display function >>>>>>>>>
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMotion);
	glutSpecialFunc(SpecialInput);
	glutKeyboardFunc(myKeyboard);

	//<<<<<<<<<<<<<<<< menus >>>>>>>>>>>>>>>>>>>>>
	int modeMenu = glutCreateMenu(changeMode);
	glutAddMenuEntry("add point", ADD_MODE);
  	glutAddMenuEntry("select point", EDIT_MODE);
  	glutAddMenuEntry("clear all points", CLEARSCREEN);

  	glutAttachMenu(GLUT_RIGHT_BUTTON);
	//<<<<<<<<<<<< initialize window >>>>>>>>>>>>>
	initDisplay();	

	//<<<<<<<<<<<<< enter loop >>>>>>>>>>>>>>>>>>>
	glutMainLoop();
}
