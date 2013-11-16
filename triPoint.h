#include "point.h"

class triPoint
{
public:
	Point p;
	float u, v;
	triPoint(Point, float, float);
	triPoint();
};
triPoint::triPoint(){
	p = Point();
	u=0;
	v=0;
}
triPoint::triPoint(Point x, float i, float j){
	p=x;
	u=i;
	v=j;
}

