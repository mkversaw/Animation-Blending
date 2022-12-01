#include "PCHierarchy.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include "QOL.h"

using glm::vec3;
using glm::vec4;

using namespace std;

void PCHierarchy::Parse(std::string& DATA_DIR, std::string& PCHierarchyFile, std::vector<shared_ptr<Joint>>& joints) {
	string value;
	
	ifstream ifs(DATA_DIR + PCHierarchyFile);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);

	jointCount = stoi(value);

	//cout << "JC: " << jointCount << "\n";

	for(int i = 0; i < jointCount; i++) {
		getline(ifs, value); // each subsequent line
		stringstream ss(value);
		
		shared_ptr<Joint> currJoint = make_shared<Joint>();
		

		ss >> currJoint->jointIdx >> currJoint->parentIdx >> currJoint->rotOrder >> currJoint->name;

		if (currJoint->parentIdx != -1) { // not the root
			joints[currJoint->parentIdx]->childrenIdx.push_back(currJoint->jointIdx); // add this joint as a child to its parent joint
		}

		joints.push_back(currJoint);
	}


	ifs.close(); // CLOSE THE STREAM !!!
}