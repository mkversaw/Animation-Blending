#pragma once

#include <string>
#include <memory>

struct Joint {
	int jointIdx; // this joints index
	int parentIdx; // if this is -1 then its the root!!!
	std::string rotOrder;
	std::string name;

	std::vector<int> childrenIdx; // indices of any children joints

	glm::mat4 worldTransform;

	Joint() {};
	Joint(int jointIdx, int parentIdx, std::string rotOrder, std::string name) : jointIdx(jointIdx), parentIdx(parentIdx), rotOrder(rotOrder), name(name) {}
};