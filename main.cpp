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
#include <list>
#include <unistd.h>



#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif
#define PI 3.14159265 

using namespace std;

struct pointNormal
{
	Point point;
	Normal normal;
	pointNormal(Point pp, Normal nn): point(pp), normal(nn) {}
};

class Viewport;
class Viewport {
  public:
    int w, h; // width and height
};
class ParametricPoint {
public:
	ParametricPoint(float u, float v);
	float u,v;
};
ParametricPoint::ParametricPoint(float u, float v) {
	this->u = u;
	this->v = v;
}
class Triangle {
public:
	Triangle(pointNormal p1, ParametricPoint uv1, pointNormal p2, ParametricPoint uv2, pointNormal p3, ParametricPoint uv3);
	vector<pointNormal> points;
	vector<ParametricPoint> parametrics;
};

// ---------------------Global Variables -----------------------
float step_size;
bool is_adaptive = false;
int num_patches;
bool uniform =true;
Viewport viewport;
GLfloat light_d[] = {0.2, 0.1, 1, 1.0};
GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
float rotx = 0;
float roty = 0;
float transx = 0;
float transy = 0;
float zoom = 1;

Patch newPatch; // used in parser and added to all_patches periodically
vector<Patch> all_patches;
vector<Patch> all_meshes;

vector <vector< vector<pointNormal> > > allOutputPoints;	



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
  		while(inpfile.good()){ // if the file is good
  			vector<string> splitline;
  			string buf; 
  			getline(inpfile,line); // gets the line
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
					vector<Point> curve1;
					curve1.push_back(Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str())));
					curve1.push_back(Point(atof(splitline[3].c_str()),atof(splitline[4].c_str()), atof(splitline[5].c_str())));
					curve1.push_back(Point(atof(splitline[6].c_str()),atof(splitline[7].c_str()), atof(splitline[8].c_str())));
					curve1.push_back(Point(atof(splitline[9].c_str()),atof(splitline[10].c_str()), atof(splitline[11].c_str())));
					newPatch.patch.push_back(curve1);
					count_curve++;
				} else if(count_curve==1){
					vector<Point> curve2;
					curve2.push_back(Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str())));
					curve2.push_back(Point(atof(splitline[3].c_str()),atof(splitline[4].c_str()), atof(splitline[5].c_str())));
					curve2.push_back(Point(atof(splitline[6].c_str()),atof(splitline[7].c_str()), atof(splitline[8].c_str())));
					curve2.push_back(Point(atof(splitline[9].c_str()),atof(splitline[10].c_str()), atof(splitline[11].c_str())));
					newPatch.patch.push_back(curve2);
					count_curve++;
				} else if(count_curve==2){
					vector<Point> curve3;
					curve3.push_back(Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str())));
					curve3.push_back(Point(atof(splitline[3].c_str()),atof(splitline[4].c_str()), atof(splitline[5].c_str())));
					curve3.push_back(Point(atof(splitline[6].c_str()),atof(splitline[7].c_str()), atof(splitline[8].c_str())));
					curve3.push_back(Point(atof(splitline[9].c_str()),atof(splitline[10].c_str()), atof(splitline[11].c_str())));
					newPatch.patch.push_back(curve3);
					count_curve++;
				} else if(count_curve==3){
					vector<Point> curve4;
					curve4.push_back(Point(atof(splitline[0].c_str()),atof(splitline[1].c_str()), atof(splitline[2].c_str())));
					curve4.push_back(Point(atof(splitline[3].c_str()),atof(splitline[4].c_str()), atof(splitline[5].c_str())));
					curve4.push_back(Point(atof(splitline[6].c_str()),atof(splitline[7].c_str()), atof(splitline[8].c_str())));
					curve4.push_back(Point(atof(splitline[9].c_str()),atof(splitline[10].c_str()), atof(splitline[11].c_str())));
					newPatch.patch.push_back(curve4);
					count_curve=0;

					all_patches.push_back(newPatch);
					newPatch = Patch();
					//newPatch.toString();
					//cout << endl;
				}
			}
  		}
  	}
}

//---------------------------------------------------

Point bezcurveinterp(vector<Point> curve, float u){
	vector<Point> iter1;
	vector<Point> iter2;
	vector<Point> iter3;

	for(int i = 0; i < curve.size()-1; i++){
		//cout << curve[i]*(1.0f-u)+curve[i+1]*u << endl;
		iter1.push_back(curve[i]*(1.0f-u)+curve[i+1]*u);
	}
	//exit(1);
	for(int i = 0; i < iter1.size()-1; i++){
		iter2.push_back(iter1[i]*(1.0f-u)+iter1[i+1]*u);
		//cout << iter1[i]*(1.0f-u)+iter1[i+1]*u << endl;;
	}
	//exit(1);
	for(int i = 0; i < iter2.size()-1; i++){
		Point p = iter2[i]*(1.0f-u)+iter2[i+1]*u;
		Vector n = 3*(iter2[i+1]-iter2[i]);
		p.vn = n;
		iter3.push_back(p);
		//cout << iter2[i]*(1.0f-u)+iter2[i+1]*u << endl;
	}
	//exit(1);
    //cout << "bez curve interp return value:     " << iter3.front().x << iter3.front().y << iter3.front().z << "---------" << endl;
    //cout << endl;
    //exit(1);
	return iter3.front();
}

Point bezpatchinterp(Patch p, float u, float v){
/*	p.toString();
	exit(1);*/
	vector<Point> vcurve;
	vcurve.push_back(bezcurveinterp(p.patch[0],u));
	vcurve.push_back(bezcurveinterp(p.patch[1],u));
	vcurve.push_back(bezcurveinterp(p.patch[2],u));
	vcurve.push_back(bezcurveinterp(p.patch[3],u));
	//exit(1);
	vector<Point> ucurve;
	vector<Point> ucurve1;
	vector<Point> ucurve2;
	vector<Point> ucurve3;
	vector<Point> ucurve4;

    ucurve1.push_back(p.patch[0][0]);
    ucurve1.push_back(p.patch[1][0]);
    ucurve1.push_back(p.patch[2][0]);
    ucurve1.push_back(p.patch[3][0]);
    ucurve.push_back(bezcurveinterp(ucurve1,v));
    ucurve2.push_back(p.patch[0][1]);
    ucurve2.push_back(p.patch[1][1]);
    ucurve2.push_back(p.patch[2][1]);
    ucurve2.push_back(p.patch[3][1]);
    ucurve.push_back(bezcurveinterp(ucurve2,v));
    ucurve3.push_back(p.patch[0][2]);
    ucurve3.push_back(p.patch[1][2]);
    ucurve3.push_back(p.patch[2][2]);
    ucurve3.push_back(p.patch[3][2]);
    ucurve.push_back(bezcurveinterp(ucurve3,v));
    ucurve4.push_back(p.patch[0][3]);
    ucurve4.push_back(p.patch[1][3]);
    ucurve4.push_back(p.patch[2][3]);
    ucurve4.push_back(p.patch[3][3]);
    ucurve.push_back(bezcurveinterp(ucurve4,v));

    //cout << ucurve[0] << endl;
	Point point = bezcurveinterp(vcurve,v);
	//cout << point << endl;
	Point normal = bezcurveinterp(ucurve,u);
	Vector v1 = Vector(normal.x,normal.y,normal.z);
	Vector v2 = Vector(point.x,point.y,point.z);
	Vector n = (normal.vn^point.vn).norm();
	point.vn = n;
	//exit(1);
	return point;
}

void adaptive(Patch p){

}

// given a patch, perform uniform subdivision 
void subdividepatch(Patch p){
	float u,v;
	Patch temp;
	int polygons = floor(1.0f/step_size)+1;

	for(int i = 0; i < polygons; i++){
		u = min(1.0f,i*step_size);
		vector<Point> curve;
		if(u == 1.0f) continue;
		for(int j = 0; j < polygons; j++){
			v = min(1.0f, j*step_size);
			if(v==1.0f)
				continue;
			Point interp = bezpatchinterp(p,u,v);
			curve.push_back(interp);
		}
		Point interp = bezpatchinterp(p,u,1.0f);
		curve.push_back(interp);
		temp.patch.push_back(curve);		
	}
	vector<Point> curve2;
	for(int k = 0; k < polygons; k++){
		v = k*step_size;
		if(v==1.0f) continue;
		Point interp = bezpatchinterp(p,1.0f,v);
		curve2.push_back(interp);
	}
	Point interp = bezpatchinterp(p,1.0f,1.0f);
	curve2.push_back(interp);
	temp.patch.push_back(curve2);
	all_meshes.push_back(temp);
	return;
}

void myReshape(int w, int h){
	viewport.w = w;
	viewport.h = h;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)viewport.w/(GLfloat)viewport.h,0.1f,100.0f);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0,0,viewport.w,viewport.h);
	
}
void myKybdHndlr(int key, int x, int y){

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
    else if (key == '+'){
    	zoom += 0.05;
    }

    else if (key == '-'){
    	zoom -= 0.05;
    }
	
	else if (key == 's'){
		is_smooth == false;
	}
	
	else if (key == 'w'){
		is_wireframe == false;
	}
    
    else if (key == 'h'){}

    else if (key == 'q'){}
    
    else 
        return;
    
    glutPostRedisplay();
}

void drawPolygons(){
	for(int i = 0; i < all_meshes.size(); i++){
		Patch p = all_meshes[i];
		for(int j = 0; j < p.patch.size()-1; j++){
			for(int c = 0; c < p.patch.size()-1; c++){
				glBegin(GL_QUADS);
				Point p1 = p.patch[j][c];
				Point p2 = p.patch[j+1][c];
				Point p3 = p.patch[j+1][c+1];
				Point p4 = p.patch[j][c+1];

				glNormal3f(p1.vn.x,p1.vn.y,p1.vn.z);
				glVertex3f(p1.x,p1.y,p1.z);
				glNormal3f(p2.vn.x,p2.vn.y,p2.vn.z);
				glVertex3f(p2.x,p2.y,p2.z);
				glNormal3f(p3.vn.x,p3.vn.y,p3.vn.z);
				glVertex3f(p3.x,p3.y,p3.z);
				glNormal3f(p4.vn.x,p4.vn.y,p4.vn.z);
				glVertex3f(p4.x,p4.y,p4.z);

				glEnd();
			}
		}
	}
}

void initScene(){

	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_d);
    glLightfv(GL_LIGHT0, GL_AMBIENT, light_d);
    glLightfv(GL_LIGHT0, GL_POSITION, light_pos);
    glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	//glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	glLineWidth(0.5);
    glColor3f(1,1,1);

    for(int i = 0; i < all_patches.size(); i++){
    	Patch p;
    	p.patch = (all_patches[i]).patch;
    	if(is_adaptive){
    		adaptive(p);
    		continue;
    	}
    	subdividepatch(p);
    }

    drawPolygons();

	myReshape(viewport.w,viewport.h);



}

void myDisplay(){

    glClear(GL_COLOR_BUFFER_BIT);               
	glClear(GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);			       
    glLoadIdentity();
	gluPerspective(60.0f,(GLfloat)viewport.w/(GLfloat)viewport.h,0.1f,100.0f);
	
	glMatrixMode(GL_MODELVIEW);                  
	glLoadIdentity(); 

    glEnable(GL_LIGHTING);
    glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    glShadeModel(GL_FLAT);
    
    glRotatef(roty,0,1,0);
    glRotatef(rotx,1,0,0);
    glTranslatef(transx,transy,zoom);
    //glTranslatef(0,transy,0);
    //glTranslatef(0,0,zoom);
    //glScalef(1, 1, zoom);

	drawPolygons();

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
	for (int k = 0; k < all_patches.size(); k++){
		all_patches[k].toString();
		cout << endl;
	}
	if(uniform){
		step_size = 1.0f/ceil(1.0f/step_size);
	}


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