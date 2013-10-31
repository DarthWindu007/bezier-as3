#include "vector.h"
#include <iostream>
#include <typeinfo>
#include "math.h"

using namespace std;

Vector::Vector(){
	this->x=0;
	this->y=0;
	this->z=0;
}
Vector::Vector(vec3 v){
	this->x=v[0];
	this->y=v[1];
	this->z=v[2];
}
Vector::Vector(float nx, float ny, float nz){
	this->x=nx;
	this->y=ny;
	this->z=nz;  
}

Vector Vector::operator^(Vector v){
	vec3 b = vec3(v.x,v.y,v.z);
	vec3 a = vec3(this->x,this->y,this->z);
	return Vector(a^b);
}

Vector Vector::operator+(Vector v2){
	return Vector(this->x+v2.x,this->y+v2.y,this->z+v2.z);
}
Vector Vector::operator-(Vector v2){
	return (*this)+(v2*-1.0f); // is the same as this.operator+(v2*-1.0f);
}
float Vector::operator*(Vector v2){
	return (v2.x*this->x + v2.y*this->y + v2.z*this->z);
}
Vector Vector::operator*(float v2){
	return Vector(v2*this->x,v2*this->y,v2*this->z);
}
Vector Vector::operator/(float v2){
	return (*this)*(1.0f/v2);
}
Vector Vector::norm(){
	float d = sqrt(pow(this->x,2)+pow(this->y,2)+pow(this->z,2));
	if(d==0)
		return Vector(0,0,0);
	else
		return (*this)/d;
}

ostream& operator<<(ostream &strm, const Vector &a){
	return strm << "Vector(" << a.x << ", " << a.y << ", " << a.z << ")";
}

Vector operator+(Vector v2, float f2){
	return v2+Vector(f2,f2,f2);
}
Vector operator-(Vector v2, float f2){
	return v2+(Vector(f2,f2,f2)*-1.0f);
}
Vector operator+(float f2, Vector v2){
	return v2+Vector(f2,f2,f2);
}
Vector operator-(float f2, Vector v2){
	return Vector(f2,f2,f2)-v2;
}
Vector operator*(float f2, Vector v2){
	return v2*f2;
}

////////




