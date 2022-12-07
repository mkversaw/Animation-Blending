#define _USE_MATH_DEFINES
#include <cmath> 
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"
#include "MatrixStack.h"

Camera::Camera() :
	aspect(1.0f),
	fovy((float)(45.0*M_PI/180.0)),
	znear(5.0f),
	zfar(5000.0f),
	rotations(0.0, 0.0),
	translations(0.0f, 0.0f, -200.0f),
	rfactor(0.01f),
	tfactor(0.001f),
	sfactor(0.005f),
	/////////
	pitchLowBound(-60 * (float)M_PI / 180),
	pitchUpBound(60 * (float)M_PI / 180),
	
	fovLowBound(4 * (float)M_PI / 180),
	fovUpBound(114 * (float)M_PI / 180),
	fovFactor(1 * (float)M_PI / 180)
{
	// pitch angle, capped between -60 and 60 degrees
	if (rotations.y > pitchUpBound) {
		rotations.y = pitchUpBound;
	}
	else if (rotations.y < pitchLowBound) {
		rotations.y = pitchLowBound;
	}

	forward = { sin(rotations.x),0.0f,cos(rotations.x) };

	objectsPos = glm::vec3(position);
}

Camera::~Camera()
{
}

void Camera::moveForward() { // forward = -z direction
	//position.z -= moveFactor;
	position += (glm::normalize(forward)) * moveFactor;
}

void Camera::moveBackward() { // backward = +z direction
	//position.z += moveFactor;
	position -= (glm::normalize(forward)) * moveFactor;
}

void Camera::moveRight() { // right = +x dirrection
	//position.x += moveFactor;
	position += glm::normalize(glm::cross(forward, up)) * moveFactor;
}

void Camera::moveLeft() { // left = -x dirrection
	//position.x -= moveFactor;
	position -= glm::normalize(glm::cross(forward, up)) * moveFactor;
}

void Camera::updatePos(glm::vec3 pos) {
	this->objectsPos = pos; // add obj pos to cam pos
}

void Camera::mouseClicked(float x, float y, bool shift, bool ctrl, bool alt)
{
	mousePrev.x = x;
	mousePrev.y = y;
	if(shift) {
		state = Camera::TRANSLATE;
	} else if(ctrl) {
		state = Camera::SCALE;
	} else {
		state = Camera::ROTATE;
	}
}

void Camera::mouseClicked2(float x, float y)
{
	mousePrev.x = x;
	mousePrev.y = y;
	state = Camera::ROTATE;
}

void Camera::mouseMoved(float x, float y)
{
	glm::vec2 mouseCurr(x, y);
	glm::vec2 dv = mouseCurr - mousePrev;
	switch(state) {
		case Camera::ROTATE:
			rotations += rfactor * dv;

			// pitch angle, capped between -60 and 60 degrees
			if (rotations.y > pitchUpBound) {
				rotations.y = pitchUpBound;
			}
			else if (rotations.y < pitchLowBound) {
				rotations.y = pitchLowBound;
			}

			//if (rotations.x < 0) {
			//	rotations.x = 0;
			//}
			//else if (rotations.x > 2 * M_PI) {
			//	rotations.x = 2 * M_PI;
			//}

			forward = { sin(rotations.x),0.0f,cos(rotations.x) };


			break;
		case Camera::TRANSLATE:
			translations.x -= translations.z * tfactor * dv.x;
			translations.y += translations.z * tfactor * dv.y;
			break;
		case Camera::SCALE:
			translations.z *= (1.0f - sfactor * dv.y);
			break;
	}
	mousePrev = mouseCurr;
}

void Camera::applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const
{
	// Modify provided MatrixStack
	P->multMatrix(glm::perspective(fovy, aspect, znear, zfar));
}

void Camera::applyViewMatrix(std::shared_ptr<MatrixStack> MV) const
{
	MV->translate(translations);
	MV->rotate(rotations.y, glm::vec3(1.0f, 0.0f, 0.0f));
	MV->rotate(rotations.x, glm::vec3(0.0f, 1.0f, 0.0f));

	// eye: cam pos
	// target: cam pos + "forward" dir
	// up: the Y vec (0,1,0)

	//MV->multMatrix(glm::lookAt(position, position + glm::vec3(forward.x, rotations.y, forward.z), up));
	
	//glm::vec3 cameraTarget = objectsPos;
	//
	//float distHor = distFromPlayerHOR * cos(rotations.y);
	//float distVert = distFromPlayerVERT * sin(rotations.y);
	//
	//float theta = rotations.x; // player rotation + angle around the player
	//float offsetX = distHor * sin(theta);
	//float offsetZ = distHor * cos(theta);

	//glm::vec3 cameraPosition = objectsPos + glm::vec3(forward.x,rotations.y,forward.z);

	//MV->multMatrix(glm::lookAt(cameraPosition, cameraTarget, up));
	//glm::vec3 test = glm::vec3(sin(rotations.x),0,cos(rotations.x));
	////MV->multMatrix(glm::lookAt(position + objectsPos , objectsPos, up));
	//
	//glm::vec3 camPos = objectsPos;
	//
	//
	//MV->multMatrix(glm::lookAt(position + objectsPos, position + objectsPos + test, up));
}
