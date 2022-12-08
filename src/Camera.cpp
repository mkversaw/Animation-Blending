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
	//if (rotations.y > pitchUpBound) {
	//	rotations.y = pitchUpBound;
	//}
	//else if (rotations.y < pitchLowBound) {
	//	rotations.y = pitchLowBound;
	//}

	rotations.y = 1;

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
	glm::vec2 mouseCurr(x, y);
	glm::vec2 dv = mouseCurr - mousePrev;

	rotations.y += rfactor * dv.y;
	if (rotations.y > pitchUpBound) {
		rotations.y = pitchUpBound;
	}
	else if (rotations.y < pitchLowBound) {
		rotations.y = pitchLowBound;
	}


	//charRotation += 0.05f * rfactor * dv.x;
	//
	//if (charRotation <= 0.0f) {
	//	charRotation = 2.0f * (float)M_PI;
	//}
	//else if (charRotation >= 2.0f * (float)M_PI) {
	//	charRotation = 0.0f;
	//}
	//
	//std::cout << charRotation << "\n";
}

void Camera::mouseMoved(float x, float y)
{
	glm::vec2 mouseCurr(x, y);
	glm::vec2 dv = mouseCurr - mousePrev;
	switch(state) {
		case Camera::ROTATE:
			rotations.x += rfactor * dv.x;

			//rotations += rfactor * dv;

			// pitch angle, capped between -60 and 60 degrees
			//if (rotations.y > pitchUpBound) {
			//	rotations.y = pitchUpBound;
			//}
			//else if (rotations.y < pitchLowBound) {
			//	rotations.y = pitchLowBound;
			//}

			if (rotations.x < 0) {
				//rotations.x = 0;
				rotations.x = 2 * M_PI;
			}
			else if (rotations.x > 2 * M_PI) {
				//rotations.x = 2 * M_PI;
				rotations.x = 0;
			}

			forward = { sin(rotations.x),0.0f,cos(rotations.x) };


			break;
		case Camera::TRANSLATE:
			translations.x -= translations.z * tfactor * dv.x;
			translations.y += translations.z * tfactor * dv.y;
			break;
		case Camera::SCALE:
			//translations.z *= (1.0f - sfactor * dv.y);
			sf *= (1.0f - sfactor * dv.y);
			//std::cout << sf << "\n";
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
	//MV->translate(translations);
	//MV->rotate(rotations.y, glm::vec3(1.0f, 0.0f, 0.0f));
	//MV->rotate(rotations.x, glm::vec3(0.0f, 1.0f, 0.0f));
	
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


	//glm::vec4 cp;
	//float hoverRadius = 10.0f;
	//
	//cp.x = hoverRadius * sin(glm::radians(rotations.x));
	////cp.y = 1;
	//cp.z = hoverRadius * cos(glm::radians(rotations.x));
	////cp.w = 1;
	//
	//glm::vec3 cameraPosition = glm::vec3(objectsPos.x + cp.x, objectsPos.y + 300.0f, objectsPos.z + cp.z + 20.0f);
	//
	//glm::vec3 focusTarget = glm::normalize(glm::vec3(cameraPosition.x, cameraPosition.y, cameraPosition.z) - objectsPos);
	//glm::vec3 rightVec = glm::normalize(glm::cross(focusTarget, glm::vec3(0.0f, 1.0f, 0.0f)));
	//glm::vec3 cameraUp = glm::normalize(glm::cross(rightVec,focusTarget));
	//glm::mat4 newMat = glm::lookAt(cameraPosition, focusTarget, cameraUp);
	//
	//MV->multMatrix(newMat);
	

	//glm::vec3 temp = glm::vec3(objectsPos.x * sin(charRotation), objectsPos.y, objectsPos.z * cos(charRotation));
	glm::vec3 temp = objectsPos;
	glm::vec3 cameraPosition = temp + 2.0f*glm::vec3(-sf * sin(rotations.x), sf_y * rotations.y, -sf * cos(rotations.x));
	
	glm::vec3 cameraTarget = temp;
	glm::vec3 cameraUp = glm::vec3(0, 1, 0);
	//std::cout << rotations.x << "\n";
	glm::mat4 newMat = glm::lookAt(cameraPosition, cameraTarget, cameraUp);
	MV->multMatrix(newMat);
}
