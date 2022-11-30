#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "Bone.h"

#include <vector>

#include <string>

struct Joint {
	int jointIdx; // if this is -1 then its a root joint!
	int parentIdx;
	std::string rotOrder;
	std::string name;

	Joint() {};
	Joint(int jointIdx, int parentIdx, std::string rotOrder, std::string name) : jointIdx(jointIdx), parentIdx(parentIdx), rotOrder(rotOrder), name(name) {}
};

struct PCHierarchy {
	PCHierarchy() {}
	PCHierarchy(std::string DATA_DIR, std::string PCHierarchyFile);

	std::vector<Joint> joints;

	int jointCount;

	void Parse(std::string DATA_DIR, std::string PCHierarchyFile);
};