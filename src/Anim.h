#pragma once

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/gtx/string_cast.hpp>

#include "PCHierarchy.h"
#include "StaticTrans.h"
#include "LocalSkele.h"
#include "LocalBind.h"
#include "Frame.h"

struct Anim {
	std::shared_ptr<PCHierarchy> hierarchy;
	std::shared_ptr<StaticTrans> stat_trans;
	std::shared_ptr<LocalSkele> local_skele;
	std::shared_ptr<LocalBind> local_bind;
	

	std::vector<std::shared_ptr<Joint>> joints;

	Anim() {};

	void genHierarchy(std::string& DATA_DIR, std::string& PCHierarchyFile);
	void genStaticTransforms(std::string& DATA_DIR, std::string& staticFile);

	//void genWorldMats(int idx = -1);

	int boneCount;
};