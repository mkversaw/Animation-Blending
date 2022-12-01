#include "Anim.h"
using namespace std;

void Anim::genHierarchy(std::string& DATA_DIR, std::string& PCHierarchyFile) {
	hierarchy = make_shared<PCHierarchy>();
	hierarchy->Parse(DATA_DIR, PCHierarchyFile, joints);
	boneCount = hierarchy->jointCount;
}

void Anim::genStaticTransforms(std::string& DATA_DIR, std::string& staticFile) {
	std::vector<std::vector<shared_ptr<Bone>> > MY_attributes;

	stat_trans = make_shared<StaticTrans>();
	stat_trans->Parse(DATA_DIR, staticFile);
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