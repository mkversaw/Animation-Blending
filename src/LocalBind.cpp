#include "LocalBind.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "QOL.h"

using glm::vec3;
using glm::vec4;

using namespace std;

LocalBind::LocalBind(std::string DATA_DIR, std::string localBindFile) {
	Parse(DATA_DIR, localBindFile);
}

void LocalBind::Parse(std::string DATA_DIR, std::string localBindFile) {
	string value;

	ifstream ifs(DATA_DIR + localBindFile);
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
		vec3 currAngle;

		ss >> rootPos.x >> rootPos.y >> rootPos.z; // first 3 on the line is the root joint position
		//cout << "RPos: " << rootPos << "\n";

		while (ss >> currAngle.x) {
			ss >> currAngle.y >> currAngle.z; // euler angles
			eulerAngles.push_back(currAngle); // push back the new angle
			//cout << currAngle << "\n";
		}

	}

	ifs.close(); // CLOSE THE STREAM !!!
}