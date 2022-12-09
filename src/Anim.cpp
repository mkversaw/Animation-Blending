#include "Anim.h"
#include <fstream>
#include <sstream>

using namespace std;
using glm::mat4;

void Anim::genHierarchy(std::string& DATA_DIR, std::string& PCHierarchyFile) {
	hierarchy = make_shared<PCHierarchy>();
	hierarchy->Parse(DATA_DIR, PCHierarchyFile, joints);
	boneCount = hierarchy->jointCount;
	//cout << boneCount << "askldjaklsdklaskldaslk\n";
	genParentSpace();

}

void Anim::genStaticTransforms(std::string& DATA_DIR, std::string& staticFile) {
	std::vector<std::vector<shared_ptr<Bone>> > MY_attributes;

	stat_trans = make_shared<StaticTrans>();
	stat_trans->Parse(DATA_DIR, staticFile);
}

void Anim::genParentSpace() {
	parentSpaceFrames.resize(frames.size());
	for (int i = 0; i < frames.size(); i++) {
		parentSpaceFrames[i] = make_shared<Frame>(frames[i]->bones); // scuffed  (deep) copy construct
		
		//cout << frames[i]->bones.size() << "\n";
		//cout << boneCount << "\n";
		assert(frames[i]->bones.size() == boneCount);
		for (int j = 0; j < boneCount; j++) {
			int parentIdx = joints[j]->parentIdx;
			if (parentIdx == -1) { // is root 
				parentSpaceFrames[i]->bones[j].mat = frames[i]->bones[j].mat; // so it stays the same
			}
			else {
				
				mat4 inverseParent = glm::inverse(frames[i]->bones[parentIdx].mat); // inverse of the parent world trans mat
				parentSpaceFrames[i]->bones[j].mat = inverseParent * frames[i]->bones[j].mat; // parent space mat for the jth joint
				
			}
			
		}
	}
}


void Anim::genBoneFrames(std::string& DATA_DIR, std::string& boneFile) {
	string value;
	int bones;

	ifstream ifs(DATA_DIR + boneFile);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	getline(ifs, value);
	cout << value << "\n";
	stringstream temp(value);
	std::string tempStr;
	temp >> tempStr;
	frameCount = stoi(tempStr);
	temp >> tempStr;
	bones = stoi(tempStr);

	while (getline(ifs, value)) {
		stringstream ss(value);
		glm::vec4 quatr;
		glm::vec3 pos;
		shared_ptr<Frame> frame = make_shared<Frame>();

		while (ss >> quatr.x) {
			ss >> quatr.y >> quatr.z >> quatr.w;
			ss >> pos.x >> pos.y >> pos.z;

			Bone currBone(quatr, pos);
			frame->bones.push_back(currBone);
		}
		frames.push_back(frame);
	}

	ifs.close();

}


//void Anim::genWorldMats(int idx = -1) {
//	// WorldTransform = ParentWorldTransform * T * Roff * Rp * Rpre * R * Rpost^-1 * Rp^-1 * Soff * Sp * S * Sp^-1
//	// missing: R?
//	shared_ptr<Bone> T = stat_trans->attributes[idx][0];
//	shared_ptr<Bone> Roff = stat_trans->attributes[idx][1];
//	shared_ptr<Bone> Rp = stat_trans->attributes[idx][2];
//	shared_ptr<Bone> Rpre = stat_trans->attributes[idx][3];
//	shared_ptr<Bone> Rpost = stat_trans->attributes[idx][4];
//	shared_ptr<Bone> Soff = stat_trans->attributes[idx][5];
//	shared_ptr<Bone> Sp = stat_trans->attributes[idx][6];
//	shared_ptr<Bone> S = stat_trans->attributes[idx][7];
//
//	if (idx == -1) { // base case: root
//		StaticTrans();
//	}
//	else {
//
//	}
//}