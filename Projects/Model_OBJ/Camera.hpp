#pragma once

#include "../../ESGIGL/common/matrix.h"
#include "../../ESGIGL/common/vector.h"
#include "../../ESGIGL/EsgiGL.h"

#define MAX_SPEED 2.f
#define GRAVITY 0.3f
#define JUMP_FORCE 6.f
#define CROUCH_SPEED 0.1f

class Camera
{
public:
	Camera(void);
	~Camera(void);

	mat4 getViewMatrix() const { return esgiMultiplyMatrix(_pitchMatrix, _viewMatrix); }
	mat4 getProjectionMatrix() const { return _projectionMatrix; }

	void setViewMatrix(const mat4& viewMatrix) { _viewMatrix = viewMatrix; }
	void setProjectionMatrix(const mat4& projectionMatrix) { _projectionMatrix = projectionMatrix; }
	void setRotationSpeed(float speed) { _rotationSpeed = speed; }
	void setRotation(float pitch, float yaw) { _pitch = pitch; _yaw = yaw; }

	void rotate(vec3 rotation);
	void translate(vec3 translation);

	void moveLeft(bool state) { _moveLeft = state; }
	void moveRight(bool state) { _moveRight = state; }
	void moveForward(bool state) { _moveForward = state; }
	void moveBackwards(bool state) { _moveBackwards = state; }
	void moveUp(bool state) { _moveUp = state; }
	void moveDown(bool state) { _moveDown = state; }
	void jump();

	void freezeRotation() { _pitch = _yaw = 0.f; }
	void update(float elapsedTime);

private:
	mat4 _pitchMatrix;
	mat4 _viewMatrix;
	mat4 _projectionMatrix;

	float _acceleration;
	float _maxSpeed;

	vec3 _translationSpeed;
	float _rotationSpeed;
	float _pitch;
	float _yaw;
	float _jumpSpeed;

	bool _moveLeft;
	bool _moveRight;
	bool _moveForward;
	bool _moveBackwards;
	bool _moveUp;
	bool _moveDown;
	bool _jumping;

	float _groundZ;

	void updateRotation(float elapsedTime) { rotate(vec3(_pitch, _yaw, 0.f) * elapsedTime); }
	void updatePosition(float elapsedTime);
};