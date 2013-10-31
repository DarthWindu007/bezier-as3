#include "vector.h"
#include "point.h"
#include <iostream>
#include <typeinfo>
#include "math.h"
using namespace std;

Point::Point(){
	this->x=0;
	this->y=0;
	this->z=0;
}
Point::Point(float nx, float ny, float nz){
	this->x=nx;
	this->y=ny;
	this->z=nz;  
}

Point Point::operator+(Point n2){
    return Point(this->x + n2.x,this->y + n2.y,this->z + n2.z);
}
Point Point::operator+(Vector n2){
    return Point(this->x + n2.x,this->y + n2.y,this->z + n2.z);
}
Point Point::operator-(Vector n2){
    return Point(this->x - n2.x,this->y - n2.y,this->z - n2.z);
}


/*Point operator+(Vector n2, Point p){
    return p+n2;
}
*/
Point operator-(Vector n2, Point p){
    return Point(-p.x,-p.y,-p.z)+n2;
}

Vector Point::operator-(Point n2){
    return Vector(this->x - n2.x,this->y - n2.y,this->z - n2.z);
}

ostream& operator<<(ostream &strm, const Point &a){
	return strm << "Point(" << a.x << ", " << a.y << ", " << a.z << ")";
}


//---------------


float operator*(Vector v2,Point p){
	return (v2.x*p.x + v2.y*p.y + v2.z*p.z);
}
Point operator*(float v2,Point p){
	return Point(v2*p.x , v2*p.y , v2*p.z);
}
Point operator*(Point p, float v2){
	return Point(v2*p.x , v2*p.y , v2*p.z);
}
	Vector operator+(Vector v2,Point p){
		return Vector(v2.x+p.x , v2.y+p.y , v2.z+p.z);
	}