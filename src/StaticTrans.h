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

struct StaticTrans {
	//Bone T; // Translation
	//Bone Roff; // Rotation offset
	//Bone Rp; // Rotation pivot
	//Bone Rpre; // Pre-rotation
	//Bone Rpost; // Inverse of the post-rotation
	//Bone Soff; // Scaling offset
	//Bone Sp; // Scaling pivot
	//Bone S; // Scaling
	StaticTrans(){}
	StaticTrans(std::string DATA_DIR, std::string staticFile);

	std::vector<std::vector<std::shared_ptr<Bone> > > attributes;
	int boneCount;

	void Parse(std::string DATA_DIR, std::string staticFile);
};