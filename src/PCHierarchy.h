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
#include "Joint.h"



struct PCHierarchy {
	PCHierarchy() {}

	int jointCount; // = boneCount
	int handIdx_LEFT = -1;
	int handIdx_RIGHT = -1;
	std::vector<std::shared_ptr<Joint>> joints;
	void Parse(std::string& DATA_DIR, std::string& PCHierarchyFile, std::vector<std::shared_ptr<Joint>>& joints);
};