#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include "point.h"
using namespace std;
class Patch
{
public:
	vector<vector<Point> > patch;
	Patch();
	Point bezpatchinterp(float, float);
	void toString();
};

Patch::Patch(){
	
}
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

Point Patch::bezpatchinterp(float u, float v){

	vector<Point> vcurve;
	vcurve.push_back(bezcurveinterp(this->patch[0],u));
	vcurve.push_back(bezcurveinterp(this->patch[1],u));
	vcurve.push_back(bezcurveinterp(this->patch[2],u));
	vcurve.push_back(bezcurveinterp(this->patch[3],u));
	//exit(1);
	vector<Point> ucurve;
	vector<Point> ucurve1;
	vector<Point> ucurve2;
	vector<Point> ucurve3;
	vector<Point> ucurve4;

    ucurve1.push_back(this->patch[0][0]);
    ucurve1.push_back(this->patch[1][0]);
    ucurve1.push_back(this->patch[2][0]);
    ucurve1.push_back(this->patch[3][0]);
    ucurve.push_back(bezcurveinterp(ucurve1,v));
    ucurve2.push_back(this->patch[0][1]);
    ucurve2.push_back(this->patch[1][1]);
    ucurve2.push_back(this->patch[2][1]);
    ucurve2.push_back(this->patch[3][1]);
    ucurve.push_back(bezcurveinterp(ucurve2,v));
    ucurve3.push_back(this->patch[0][2]);
    ucurve3.push_back(this->patch[1][2]);
    ucurve3.push_back(this->patch[2][2]);
    ucurve3.push_back(this->patch[3][2]);
    ucurve.push_back(bezcurveinterp(ucurve3,v));
    ucurve4.push_back(this->patch[0][3]);
    ucurve4.push_back(this->patch[1][3]);
    ucurve4.push_back(this->patch[2][3]);
    ucurve4.push_back(this->patch[3][3]);
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

void Patch::toString(){
	for(int i = 0; i < patch.size(); i++){
		for(int j = 0; j < patch[i].size(); j++){
			cout << patch[i][j] << " ";
		}
		cout << endl;
	}
}