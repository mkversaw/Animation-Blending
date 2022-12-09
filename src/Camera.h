#pragma  once
#ifndef CAMERA_H
#define CAMERA_H

#include <memory>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>

class MatrixStack;

class Camera
{
public:
	enum {
		ROTATE = 0,
		TRANSLATE,
		SCALE
	};
	
	Camera();
	virtual ~Camera();
	void setInitDistance(float z) { translations.z = -std::abs(z); }
	void setAspect(float a) { aspect = a; };
	void setRotationFactor(float f) { rfactor = f; };
	void setTranslationFactor(float f) { tfactor = f; };
	void setScaleFactor(float f) { sfactor = f; };
	void mouseClicked(float x, float y, bool shift, bool ctrl, bool alt);
	void mouseClicked2(float x, float y);
	void mouseMoved(float x, float y);
	void applyProjectionMatrix(std::shared_ptr<MatrixStack> P) const;
	void applyViewMatrix(std::shared_ptr<MatrixStack> MV) const;
	
	void moveForward();
	void moveBackward();
	void moveLeft();
	void moveRight();

	void Camera::updatePos(glm::vec3 pos);
	glm::vec2 rotations;

	float charRotation = 0.0f;
	float sf = 300.0f;

	glm::vec3 offset = { 0,0,0 };


	void Camera::zoomIn();
	void Camera::zoomOut();

private:
	float aspect;
	float fovy;
	float znear;
	float zfar;
	
	glm::vec3 translations;
	glm::vec2 mousePrev;
	int state;
	float rfactor;
	float tfactor;
	float sfactor;

	glm::vec3 up = { 0.0f, 1.0f, 0.0f }; // positive y-dir
	glm::vec3 forward = { 0.0f,0.0f,1.0f };
	glm::vec3 position = { 0.0f,200.0f,-200.0f }; // starting position cam coords
	glm::vec3 yaw = { 0.0f,0.0f,0.0f };
	glm::vec3 pitch = { 0.0f,0.0f,0.0f };
	float moveFactor = 1.0f;

	float pitchLowBound;
	float pitchUpBound;
	
	float sf_y = 300;
	float fovLowBound;
	float fovUpBound;
	float fovFactor;

	float distFromPlayerHOR = 250.0f;
	float distFromPlayerVERT = 50.0f;
	
	glm::vec3 objectsPos;
};

#endif
