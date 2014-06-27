#include "Camera.hpp"

Camera::Camera(void)
{
	_viewMatrix.Identity();
	_pitchMatrix.Identity();
	_projectionMatrix.Identity();
	_translationSpeed = vec3(0.f, 0.f, 0.f);
	_jumpSpeed = 0.f;
	_rotationSpeed = 0.1f;
	_pitch = 0.f;
	_yaw = 0.f;
}


Camera::~Camera(void)
{
}


void Camera::rotate(vec3 rotation) {
	
	_pitchMatrix = esgiMultiplyMatrix(esgiRotateX(rotation.x), _pitchMatrix);
	_viewMatrix = esgiMultiplyMatrix(esgiRotateY(rotation.y), _viewMatrix);
	//_viewMatrix = esgiMultiplyMatrix(esgiRotateZ(rotation.z), _viewMatrix);
}

void Camera::translate(vec3 translation) {
	_viewMatrix.T.x += translation.x;
	_viewMatrix.T.y += translation.y;
	_viewMatrix.T.z += translation.z;
}

void Camera::updatePosition() {

	bool noInput = true;

	if(_moveLeft) {
		_translationSpeed.x = MAX_SPEED;
		noInput = false;
	}
	if(_moveRight) {
		_translationSpeed.x = -MAX_SPEED;
		noInput = false;
	}
	if(_moveForward) {
		_translationSpeed.z = MAX_SPEED;
		noInput = false;
	}
	if(_moveBackwards) {
		_translationSpeed.z = -MAX_SPEED;
		noInput = false;
	}

	if(_jumping) {

		_jumpSpeed += GRAVITY;

		if(_viewMatrix.T.y > _groundZ) {
			_jumping = false;
			_jumpSpeed = 0.f;
			_viewMatrix.T.y = _groundZ;
		}
	}

	if(noInput) {
		_translationSpeed = vec3(0.f, 0.f, 0.f);
	}
	
	if(_translationSpeed.Length() >= MAX_SPEED) {
		_translationSpeed = _translationSpeed.Normalized() * MAX_SPEED;
	}

	translate(vec3(_translationSpeed.x, _jumpSpeed, _translationSpeed.z));
}

void Camera::update() {
	updateRotation();
	updatePosition();
}

void Camera::jump() {

	if(!_jumping) {
		_groundZ = _viewMatrix.T.y;
		_jumpSpeed = -JUMP_FORCE;
		_jumping = true;
	}
}