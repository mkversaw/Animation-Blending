#include "Bone.h"
#include <string>

Bone::Bone(glm::vec4& quatr, glm::vec3& pos) {
	mat = glm::mat4_cast(glm::quat(quatr.w,quatr.x,quatr.y,quatr.z));
	mat[3] = glm::vec4(pos, 1.0f);
}

Bone::Bone(glm::mat4& newMat) {
	this->mat = newMat;
}

void Bone::print() {
	std::cout << glm::to_string(transpose(mat)) << "\n";
}