#pragma once
#include <memory>
#include <vector>
#include <map>
#include <iostream>
#include <cstring>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "MatrixStack.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "Program.h"
#include "Shape.h"

using glm::vec3;
using std::string;
using std::shared_ptr;
using std::make_shared;
using std::vector;
using std::map;

using glm::value_ptr;

struct Component {
	vec3 angles; // current joint angles about the X, Y, and Z axes of the component’s joint
	vec3 meshTrans; // translation of the component’s mesh with respect to its joint
	vec3 scale; //  X, Y, and Z scaling factors for the mesh
	vec3 kd;

	bool spin = false;
	vec3 spinAngle = { 0,0,0 };
	
	shared_ptr<Shape> shape;

	bool shear = false;

	Component();
	Component(const vec3& meshTrans, const vec3& scale = { 1,1,1 }, const vec3& angles = { 0,0,0 }, const vec3& kd = { 1,0,0 }); // angles should always default to 0,0,0
	void draw(MatrixStack& MV, shared_ptr<Program>& prog, const double& time); // draw component
	void debug();
};