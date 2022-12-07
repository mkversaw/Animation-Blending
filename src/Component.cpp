#include "Component.h"

Component::Component() {
	angles = { 0,0,0 }; 
	meshTrans = { 0,0,0 };
	scale = { 1,1,1 };
	kd = { 0,0,1 };
	shape = make_shared<Shape>();
}

Component::Component(const vec3& meshTrans, const vec3& scale, const vec3& angles, const vec3& kd) {
	this->meshTrans = meshTrans;
	this->scale = scale;
	this->angles = angles;
	this->kd = kd;
	shape = make_shared<Shape>();
}

void Component::draw(MatrixStack& MV, shared_ptr<Program>& prog, const double& time) {
	MV.pushMatrix(); // put new matrix on top of stack
	
	if (!spin) {
		MV.translate(meshTrans);

		MV.rotate(angles.x, 1, 0, 0); // handle rotation for x,y,z
		MV.rotate(angles.y, 0, 1, 0);
		MV.rotate(angles.z, 0, 0, 1);
	}
	else {
		MV.translate(meshTrans);
		MV.rotate(time, spinAngle.x, spinAngle.y, spinAngle.z);
		MV.translate(-meshTrans);
	}

	MV.scale(scale);

	glUniformMatrix4fv(prog->getUniform("MV"), 1, GL_FALSE, glm::value_ptr(MV.topMatrix())); // send the matrix at the top of the stack to the GPU
	//glUniform3f(prog->getUniform("kd"), kd.r, kd.g, kd.b);

	shape->draw(prog);
	
	MV.popMatrix(); // pop the matrix off
}

void Component::debug() { // quality of life debugging function
	std::cout << "meshTrans: " << meshTrans[0] << " " << meshTrans[1] << " " << meshTrans[2] << "\n";
	std::cout << "scale: " << scale[0] << " " << scale[1] << " " << scale[2] << "\n";
	std::cout << "angles: " << angles[0] << " " << angles[1] << " " << angles[2] << "\n";
	std::cout << "Spin Angles: " << spinAngle[0] << " " << spinAngle[1] << " " << spinAngle[2] << "\n";
}