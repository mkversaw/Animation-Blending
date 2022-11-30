#include "PCHierarchy.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "QOL.h"

using glm::vec3;
using glm::vec4;

using namespace std;

PCHierarchy::PCHierarchy(std::string DATA_DIR, std::string PCHierarchyFile) {
	Parse(DATA_DIR, PCHierarchyFile);
}

void PCHierarchy::Parse(std::string DATA_DIR, std::string PCHierarchyFile) {
	string value;

	ifstream ifs(DATA_DIR + PCHierarchyFile);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);

	jointCount = stoi(value);

	//cout << "JC: " << jointCount << "\n";

	while (getline(ifs, value)) { // each subsequent line
		stringstream ss(value);

		Joint currJoint;
		ss >> currJoint.jointIdx >> currJoint.parentIdx >> currJoint.rotOrder >> currJoint.name;
		joints.push_back(currJoint);
		//cout << currJoint.jointIdx << " " << currJoint.parentIdx << " " << currJoint.name << "\n";
	}

	ifs.close(); // CLOSE THE STREAM !!!
}