#include "LocalSkele.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "QOL.h"

using glm::vec3;
using glm::vec4;

using namespace std;

LocalSkele::LocalSkele(std::string DATA_DIR, std::string localSkeleFile) {
	Parse(DATA_DIR, localSkeleFile);
}

void LocalSkele::Parse(std::string DATA_DIR, std::string localSkeleFile) {
	string value;

	ifstream ifs(DATA_DIR + localSkeleFile);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);


	stringstream temp(value);
	string tempStr;

	temp >> tempStr;
	frameCount = stoi(tempStr);
	temp >> tempStr;
	boneCount = stoi(tempStr);

	//cout << "FC: " << frameCount << "\n";
	//cout << "BC: " << boneCount << "\n";

	while (getline(ifs, value)) { // each subsequent line
		stringstream ss(value);
		vec3 rootPos;
		vec3 currAngle;

		ss >> rootPos.x >> rootPos.y >> rootPos.z; // first 3 on the line is the root joint position
		//cout << rootPos << "\n";
		rootPositions.push_back(rootPos); // pushback the root pos for this frame
		vector<vec3> currentAngles; // holds this frame's euler angles

		while (ss >> currAngle.x) {
			ss >> currAngle.y >> currAngle.z; // euler angles
			currentAngles.push_back(currAngle); // push back the new angle
		}
		
		eulerAngles.push_back(currentAngles); // push back this frame's euler angles
		
	}

	ifs.close(); // CLOSE THE STREAM !!!
}