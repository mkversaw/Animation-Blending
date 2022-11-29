#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <vector>
#include "Bone.h"

struct Frame {
	std::vector<Bone> bones;
	Frame();
	Frame(std::vector<Bone>& bones);
	void print(int i);
};