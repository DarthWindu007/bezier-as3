#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

class Patch
{
public:
	Point patch[4][4];
	Patch();
	void toString();
};

Patch::Patch(){
	
}

void Patch::toString(){
	for(int i = 0; i < 4; i++){
		for(int j = 0; j < 4; j++){
			cout << patch[i][j] << ", ";
			//cout << "fuk this shit" << endl;
		}
		cout << endl;
	}

}