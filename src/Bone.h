#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <glm/gtx/string_cast.hpp>


struct Bone {
	glm::mat4 mat; // 4x4 rigid transformation matrix
	Bone(glm::vec4& quatr, glm::vec3& pos);
	void print();
};