#include "vector.h"
#include "point.h"
#include "normal.h"
#include "algebra3.h"
#include "patch.h"
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <stdlib.h>
#include <cstdlib> 


#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif
#define PI 3.14159265 

using namespace std;

class Viewport;
class Viewport {
  public:
    int w, h; // width and height
};

// ---------------------Global Variables -----------------------
int step_size;
bool is_adaptive = false;
int num_patches;

Viewport viewport;
GLfloat light[] = {1, 1, 1, 1.0};
float rotx = 0;
float roty = 0;
float transx = 0;
float transy = 0;

Patch newPatch; // used in parser and added to all_patches periodically
vector<Patch> all_patches;
vector<Patch> all_meshes;

//--------------------------------------------------------------


//----------------------Parser-----------------------
void parse_file(string name){
	ifstream inpfile(name.c_str());
	int count_patch = 0;
	int count_curve = 0;
	if(!inpfile.is_open()) {
    	cout << "Unable to open file" << endl;
  	} else {
  		string line;
  		while(inpfile.good()){
  			vector<string> splitline;
  			string buf;
  			getline(inpfile,line);
  			stringstream ss(line);
  			while(ss >> buf){
  				splitline.push_back(buf);
  			}
			if(splitline.size() == 0) {
				continue;
			}
			if(splitline[0][0] == '#') {
				continue;
			} else if(splitline.size() == 1) {
				num_patches = atoi(splitline[0].c_str());
				//cout << "num_patches parser" << endl;
				//cout << splitline[0].c_str() << endl;
			} else if(splitline.size() == 12){
				if(count_curve==0){
					newPatch.patch[0][0] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[0][1] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[0][2] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[0][3] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					count_curve++;
				} else if(count_curve==1){
					newPatch.patch[1][0] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[1][1] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[1][2] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[1][3] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					count_curve++;
				} else if(count_curve==2){
					newPatch.patch[2][0] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[2][1] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[2][2] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[2][3] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					count_curve++;
				} else if(count_curve==3){
					newPatch.patch[3][0] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[3][1] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[3][2] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					newPatch.patch[3][3] = Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str()));
					count_curve=0;

					all_patches.push_back(newPatch);
					//newPatch.toString();
					//cout << endl;
				}
			}
  		}
  	}
}

//---------------------------------------------------

void myReshape(int w, int h){
	viewport.w = w;
	viewport.h = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)viewport.w/(GLfloat)viewport.h,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,viewport.w,viewport.h);
	
}
void myKybdHndlr_2(int key, int x, int y){

	int modifier = glutGetModifiers();
    if (key == GLUT_KEY_UP){
    	if(modifier == 1)
    		transy += 1;
    	else
    		rotx += 2.5;
    }
    else if (key == GLUT_KEY_DOWN){
    	if(modifier == 1)
    		transy -= 1;
    	else
    		rotx -= 2.5;
    }	
    else if (key == GLUT_KEY_LEFT){
    	if(modifier == 1)
    		transx -= 1;
    	else
    		roty += 2.5;
    }	    
    else if (key == GLUT_KEY_RIGHT){
    	if(modifier == 1)
    		transx += 1;
    	else
    		roty -= 2.5;
    }
    else 
        return;
    
    glutPostRedisplay ();
}
void myKybdHndlr(unsigned char key, int x, int y){
	if (key == ' ')
        exit(0);
    else if (key == '+'){}

    else if (key == '-'){}
	
	else if (key == 's'){}
	
	else if (key == 'w'){}
    
    else if (key == 'h'){}

    else if (key == 'q'){}
    
    else 
        return;
    
    glutPostRedisplay();
}

void initScene(){
	glViewport(0,0,viewport.w,viewport.h);

    glMatrixMode(GL_PROJECTION);			       
    glLoadIdentity();
    gluPerspective(60.0f,(GLfloat)viewport.w/(GLfloat)viewport.h,0.1f,100.0f);

    glClear(GL_COLOR_BUFFER_BIT);		    // clear the color buffer
    glClear(GL_DEPTH_BUFFER_BIT);           // clear the depth buffer

/*   glClearColor(0.0f, 0.0f, 0.0f, 1.0f); // Set background color to black and opaque
   glClearDepth(1.0f);                   // Set background depth to farthest
   glEnable(GL_DEPTH_TEST);   // Enable depth testing for z-culling
   glDepthFunc(GL_LEQUAL);    // Set the type of depth-test
   glShadeModel(GL_SMOOTH);   // Enable smooth shading
   glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);  // Nice perspective corrections*/

	glLineWidth(0.5);
	glColor3f(1,1,1);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light);

	glEnable(GL_LIGHT0);
    glEnable(GL_LIGHTING);
    glEnable(GL_DEPTH_TEST);
    glutPostRedisplay();
}

void myDisplay(){

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
   glMatrixMode(GL_MODELVIEW);     // To operate on model-view matrix
 

   glLoadIdentity();

    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glRotatef(roty,0,1,0);
    glRotatef(rotx,1,0,0);
    glTranslatef(transx,0,0);
    glTranslatef(0,transy,0);

    glBegin(GL_QUADS);
    glColor3f(1,0,1);
    glNormal3f(1,1,0);
    glVertex3f( 0.5, 0.5,-10.0);
    glVertex3f( 1.5, 0.5,-10.0);
    glVertex3f( 1.5, 1.5,-10.0);
    glVertex3f( 0.5, 1.5,-10.0);
    
    glEnd();





    glFlush();
    glutSwapBuffers();	

    cout << "wat?" <<endl;
}

int main(int argc, char** argv){
	string file_name;
	if(argc > 2){
		file_name = argv[1];
		step_size = atof(argv[2]);
		if(argc >3){
			is_adaptive = string(argv[3])=="-a";
		}
	}else{
		cout << "Error: Invalid Arguments" << endl;
		exit(0);
	}

	parse_file(file_name);

	//cout << num_patches << endl;
	for(int i =0; i < num_patches; i++){
		all_patches[i].toString();
		cout << endl;
		//cout << "going here?" <<endl;
	}

	//cout << "wat?" << endl;

    viewport.w = 600;
    viewport.h = 600;
	glutInit(&argc, argv);                        // This initializes glut  
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );  // Use a double-buffered window with red, green, and blue channels 
    glutInitWindowSize(viewport.w, viewport.h);   //The size and position of the window
    glutInitWindowPosition(0,0);      // x-,y- coords of the topLeft of new window.
    glutCreateWindow(argv[0]);        //} name of window.

    
    glutDisplayFunc(myDisplay);	    // function to run when its time to draw something
    glutReshapeFunc(myReshape);	    // function to run when the window gets resized
    glutKeyboardFunc(myKybdHndlr);
    glutSpecialFunc(myKybdHndlr);
    initScene();
    //glutIdleFunc(myDisplay);
    glutMainLoop();				    // infinite loop that will keep drawing and resizing
    return 0;  
}