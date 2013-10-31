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
	glViewport (0,0,viewport.w,viewport.h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)viewport.w/(GLfloat)viewport.h,0.1f,100.0f);
}
void myKybdHndlr(int key, int x, int y){

    if (key == GLUT_KEY_UP){}
    else if (key == GLUT_KEY_DOWN){}	
    else if (key == GLUT_KEY_LEFT){}	    
    else if (key == GLUT_KEY_RIGHT){}
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

}

void myDisplay(){
	glClear(GL_COLOR_BUFFER_BIT);		    // clear the color buffer
    glClear(GL_DEPTH_BUFFER_BIT);           // clear the depth buffer

    glMatrixMode(GL_PROJECTION);			       
    glLoadIdentity();				        // make sure transformation is "zero'd"
    gluPerspective(60.0f,(GLfloat)viewport.w/(GLfloat)viewport.h,0.1f,100.0f);
    
    glMatrixMode(GL_MODELVIEW);			    // indicate we are specifying camera transformations
    glLoadIdentity();


    glFlush();
    glutSwapBuffers();	
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

    initScene();
    glutDisplayFunc(myDisplay);	    // function to run when its time to draw something
    glutReshapeFunc(myReshape);	    // function to run when the window gets resized
    glutKeyboardFunc(myKybdHndlr);
    glutSpecialFunc(myKybdHndlr);
    glutMainLoop();				    // infinite loop that will keep drawing and resizing
    return 0;  
}