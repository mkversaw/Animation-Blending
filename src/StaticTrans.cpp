#include "StaticTrans.h"
#include <iostream>
#include <fstream>
#include <sstream>

using glm::vec3;
using glm::vec4;

using namespace std;

StaticTrans::StaticTrans(std::string DATA_DIR, std::string staticFile) {
	Parse(DATA_DIR, staticFile);
}

void StaticTrans::Parse(std::string DATA_DIR, std::string staticFile) {
	string value;

	ifstream ifs(DATA_DIR + staticFile);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);

	boneCount = stoi(value);

	while (getline(ifs, value)) { // each subsequent line
		stringstream ss(value);
		vec4 currQuat;
		vec3 currPos;

		vector<Bone> currMatrices; // holds the current 8 "bones" (matrices)

		for (int i = 0; i < 8; i++) { // 8 matrices per line
			ss >> currQuat.x >> currQuat.y >> currQuat.z >> currQuat.w; // rotations
			ss >> currPos.x >> currPos.y >> currPos.z; // positions
			//cout << currPos.x << " " << currPos.y << " " << currPos.z << "\n";
			currMatrices.push_back(Bone(currQuat, currPos)); // push back the matrice
		}
		attributes.push_back(currMatrices); // push back the 8 mats
	}

	ifs.close(); // CLOSE THE STREAM !!!
}