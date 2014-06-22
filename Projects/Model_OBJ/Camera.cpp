#include "Camera.hpp"

Camera::Camera(void)
{
	_viewMatrix.Identity();
	_pitchMatrix.Identity();
	_projectionMatrix.Identity();
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