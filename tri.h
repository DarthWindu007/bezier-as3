#include "triPoint.h"
class tri
{
public:
	triPoint p1;
	triPoint p2;
	triPoint p3;
	tri(triPoint,triPoint,triPoint);
	tri();
};

tri::tri(){
	p1=triPoint();
	p2=triPoint();
	p3=triPoint();
}

tri::tri(triPoint v1, triPoint v2, triPoint v3){
	p1=v1;
	p2=v2;
	p3=v3;
}