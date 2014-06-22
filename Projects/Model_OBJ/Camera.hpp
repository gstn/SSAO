#pragma once

#include "../../ESGIGL/common/matrix.h"
#include <iostream>
#include "../../ESGIGL/common/vector.h"

class Camera
{
public:
	Camera(void);
	~Camera(void);

	mat4 getViewMatrix() const { return esgiMultiplyMatrix(_pitchMatrix, _viewMatrix); }
	mat4 getProjectionMatrix() const { return _projectionMatrix; }

	void setViewMatrix(const mat4& viewMatrix) { _viewMatrix = viewMatrix; }
	void setProjectionMatrix(const mat4& projectionMatrix) { _projectionMatrix = projectionMatrix; }

	void rotate(vec3 rotation);
	void translate(vec3 translation);

	void updateRotation() { rotate(vec3(_pitch, _yaw, 0.f)); }
	void setRotation(float pitch, float yaw) { _pitch = pitch; _yaw = yaw; }

private:
	mat4 _pitchMatrix;
	mat4 _viewMatrix;
	mat4 _projectionMatrix;
	float _pitch;
	float _yaw;
};