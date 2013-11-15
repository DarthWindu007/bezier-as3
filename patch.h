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
	void toString();
};

Patch::Patch(){
	
}

void Patch::toString(){
	for(int i = 0; i < patch.size(); i++){
		for(int j = 0; j < patch[i].size(); j++){
			cout << patch[i][j] << " ";
		}
		cout << endl;
	}
}