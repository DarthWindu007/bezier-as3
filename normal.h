#ifndef NORMAL_H
#define NORMAL_H
#include <iostream>
#include "vector.h"
#include "point.h"
#include "algebra3.h"
class Normal {
private:
	void setStuff(float,float,float);
public:
	float x,y,z;
	Normal();
	Normal(float,float,float);
	Normal(Vector);
	Normal(vec3);
	Normal(Point);
	Normal operator+(Normal);
	Normal operator-(Normal);
	Normal operator*(Normal);

	Normal norm();
};

std::ostream& operator<<(std::ostream&, const Normal&);


float operator*(Vector v2,Normal p);
Point operator*(Normal p, float v2);


#endif