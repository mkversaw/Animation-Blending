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

struct LocalSkele {
	LocalSkele() {}
	LocalSkele(std::string DATA_DIR, std::string localSkeleFile);

	std::vector<glm::vec3> rootPositions; // first 3 digits of a line are the position for the root joint
	std::vector<std::vector<glm::vec3>> eulerAngles; // every 3 digits that come after are euler angles (the first of these 3 is for the root!)
	
	int frameCount;
	int boneCount;

	void Parse(std::string DATA_DIR, std::string localSkeleFile);
};