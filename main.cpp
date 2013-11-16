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

#include "tri.h"



#ifdef OSX
#include <GLUT/glut.h>
#include <OpenGL/glu.h>
#else
#include <GL/glut.h>
#include <GL/glu.h>
#endif


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

// ---------------------Global Variables -----------------------
float step_size;
bool is_adaptive = false;
int num_patches;
bool uniform =true;
bool is_wireframe = false;
bool is_smooth = false;
bool is_hidden = false;
Viewport viewport;
GLfloat light_d[] = {0.8, 0.0, 1, 1.0};
GLfloat light_pos[] = {1.0, 1.0, 1.0, 0.0};
float rotx = 0;
float roty = 0;
float transx = 0;
float transy = 0;
float zoom = -5;
int limit = 5;

Patch newPatch; // used in parser and added to all_patches periodically
vector<Patch> all_patches;
vector<Patch> all_meshes;
vector<tri> all_triangles;

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


triPoint getMidPoint(triPoint p1, triPoint p2, Patch patch){
    float newU = (p1.u + p2.u) * 0.5;
    float newV = (p1.v + p2.v) * 0.5;

    Point newXY = (p1.p + p2.p) * 0.5;
    newXY.vn.x =0;
    newXY.vn.y =0;
    newXY.vn.z =0;
    triPoint toReturn = triPoint(newXY,newU,newV);
    return toReturn;
}

bool isGoodSubdiv(triPoint thisPt, Patch p){
    Point midP = thisPt.p;
    Point actualPt = p.bezpatchinterp(thisPt.u,thisPt.v);
    float diff = pow((midP.x-actualPt.x),2) + pow((midP.y-actualPt.y),2) + pow((midP.z-actualPt.z),2);
    diff = sqrt(diff);
    if (diff < step_size) { return true;}
    if (diff >= step_size) { return false;}
}

triPoint getAdaptivePointOnCurve(triPoint p1, triPoint p2, Patch p){
    float newU = (p1.u + p2.u) * 0.5;
    float newV = (p1.v + p2.v) * 0.5;
    Point thisPt = p.bezpatchinterp(newU,newV);
    triPoint toReturn = triPoint(thisPt,newU,newV);
    return toReturn;
}


void makeAdaptiveTriangles_recursive(tri t, Patch p){
    triPoint mp1 = getMidPoint(t.p1,t.p2,p);
    triPoint mp2 = getMidPoint(t.p2,t.p3,p);
    triPoint mp3 = getMidPoint(t.p3,t.p1,p);
    
    bool g1 = isGoodSubdiv(mp1,p);
    bool g2 = isGoodSubdiv(mp2,p);
    bool g3 = isGoodSubdiv(mp3,p);
   
    mp1 = getAdaptivePointOnCurve(t.p1,t.p2,p);
    mp2 = getAdaptivePointOnCurve(t.p2,t.p3,p);
    mp3 = getAdaptivePointOnCurve(t.p3,t.p1,p);
    
    

    
    if (!g1 && !g2 && !g3){ 
        makeAdaptiveTriangles_recursive(tri(t.p1,mp1,mp3),p);
        makeAdaptiveTriangles_recursive(tri(mp1,mp3,mp2),p);
        makeAdaptiveTriangles_recursive(tri(mp1,t.p2,mp2),p);
        makeAdaptiveTriangles_recursive(tri(mp3,mp2,t.p3),p);
        return;
    }
    else if (!g1 && !g2 && g3){
        makeAdaptiveTriangles_recursive(tri(mp1,t.p2,mp2),p);
        makeAdaptiveTriangles_recursive(tri(t.p1,mp1,mp2),p);
        makeAdaptiveTriangles_recursive(tri(t.p1,mp2,t.p3),p);
        return;
    }
    else if (!g1 && g2 && !g3){
        makeAdaptiveTriangles_recursive(tri(t.p1,mp1,mp3),p);
        makeAdaptiveTriangles_recursive(tri(mp1,t.p2,t.p3),p);
        makeAdaptiveTriangles_recursive(tri(mp1,t.p3,mp3),p);
        return;
    }
    else if (!g1 && g2 && g3){
        makeAdaptiveTriangles_recursive(tri(t.p1,mp1,t.p3),p);
        makeAdaptiveTriangles_recursive(tri(mp1,t.p2,t.p3),p);
        return;
    }
    else if (g1 && !g2 && !g3){
        makeAdaptiveTriangles_recursive(tri(t.p1,t.p2,mp3),p);
        makeAdaptiveTriangles_recursive(tri(t.p2,mp2,mp3),p);
        makeAdaptiveTriangles_recursive(tri(mp3,mp2,t.p3),p);
        return;
    }
    else if (g1 && !g2 && g3){
        makeAdaptiveTriangles_recursive(tri(t.p1,t.p2,mp2),p);
        makeAdaptiveTriangles_recursive(tri(t.p1,mp2,t.p3),p);
        return;
    }
    else if (g1 && g2 && !g3){
        makeAdaptiveTriangles_recursive(tri(t.p1,t.p2,mp3),p);
        makeAdaptiveTriangles_recursive(tri(mp3,t.p2,t.p3),p);
        return;
    }
    else if (g1 && g2 && g3){
        all_triangles.push_back(t);
        return;
    }
}

void adaptive(Patch p){
    Point p1 = p.bezpatchinterp( 0, 0);
    Point p2 = p.bezpatchinterp( 1, 0);
    Point p3 = p.bezpatchinterp( 1, 1);
    Point p4 = p.bezpatchinterp( 0, 1);
    
    triPoint pt1,pt2,pt3,pt4;
    
    pt1 = triPoint(p1,0,0);
    pt2 = triPoint(p2,1,0);
    pt3 = triPoint(p3,1,1);
    pt4 = triPoint(p4,0,1);
    
    tri t1 = tri(pt1,pt2,pt3);
    
    tri t2 = tri(pt1,pt4,pt3);
    
    makeAdaptiveTriangles_recursive(t1, p);
    makeAdaptiveTriangles_recursive(t2, p);
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
			Point interp = p.bezpatchinterp(u,v);
			curve.push_back(interp);
		}
		Point interp = p.bezpatchinterp(u,1.0f);
		curve.push_back(interp);
		temp.patch.push_back(curve);		
	}
	vector<Point> curve2;
	for(int k = 0; k < polygons; k++){
		v = k*step_size;
		if(v==1.0f) continue;
		Point interp = p.bezpatchinterp(1.0f,v);
		curve2.push_back(interp);
	}
	Point interp = p.bezpatchinterp(1.0f,1.0f);
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
    	zoom += 0.5;
    }

    else if (key == '-'){
    	zoom -= 0.5;
    }
	
	else if (key == 's'){
		is_smooth = is_smooth==false;
	}
	
	else if (key == 'w'){
		is_wireframe = is_wireframe==false;
	}
    
    else if (key == 'h'){
    	is_hidden = is_hidden == false;
    }

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
void drawNormalVertex(triPoint tp){
	glNormal3f(tp.p.vn.x,tp.p.vn.y,tp.p.vn.z);
	glVertex3f(tp.p.x,tp.p.y,tp.p.z);
}

void draw(tri t){
	glBegin(GL_TRIANGLES);

	drawNormalVertex(t.p1);
	drawNormalVertex(t.p2);
	drawNormalVertex(t.p3);

	glEnd();

}

void drawTriangles(){
	for(int i = 0; i < all_triangles.size(); i++){
		tri shape = all_triangles[i];
		draw(shape);
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

	glLineWidth(1.0);
    glColor3f(0.8f,0.1f,0.1f);

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

	//myReshape(viewport.w,viewport.h);
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

        glTranslatef(transx,transy,zoom);
    glRotatef(roty,0,1,0);
    glRotatef(rotx,1,0,0);
    if(!is_wireframe){
    	glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
    	glEnable(GL_LIGHTING);
	    if(!is_smooth)
	    	glShadeModel(GL_FLAT);
	    else
	    	glShadeModel(GL_SMOOTH);
    	if(is_adaptive)
			drawTriangles();
		else
			drawPolygons();
	}
    else{
    	glColor3f(0.8f, 0.2f, 0.1f);
    	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    	glDisable(GL_LIGHTING);
    	    		        if(is_adaptive)
	        	drawTriangles();
	        else
	        	drawPolygons();
    	if (is_hidden){
	        glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);
	        glEnable(GL_POLYGON_OFFSET_FILL);
	        glPolygonOffset(1.f,1.f);
	        glColor3f(0.0f, 0.0f, 0.0f);
	        if(is_adaptive)
	        	drawTriangles();
	        else
	        	drawPolygons();
	        glColor3f(0.8f, 0.2f, 0.1f);
	        glDisable(GL_POLYGON_OFFSET_FILL);
    	}
    	else{
    		glColor3f(0.8f, 0.2f, 0.1f);

	    }
    }
/*    if(!is_smooth)
    	glShadeModel(GL_FLAT);
    else
    	glShadeModel(GL_SMOOTH);*/
    


    //glTranslatef(0,transy,0);
    //glTranslatef(0,0,zoom);
    //glScalef(1, 1, zoom);

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