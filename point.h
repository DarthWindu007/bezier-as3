#ifndef POINT_H
#define POINT_H
#include "vector.h"
#include <iostream>

class Point {
public:
	float x,y,z;
	Point();
	Point(float,float,float);
	Point operator+(Point);
	Point operator+(Vector);
	Point operator-(Vector);
	Vector operator-(Point);
};

std::ostream& operator<<(std::ostream&, const Point&);

float operator*(Vector v2,Point p);
Point operator*(float v2,Point p);
Point operator*(Point p, float v2);
Vector operator+(Vector v2,Point p);

#endif
