#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"

Camera::Camera() {
	updateWorldView();
};

Camera::~Camera() {

}

void Camera::setDeltaTime(GLfloat deltaTime) {
	this->deltaTime = deltaTime;
}

Vector3 Camera::convertToVector3(Vector4 vec) {
	GLfloat x = vec.x, y = vec.y, z = vec.z;
	
	Vector3 vec3; 
	vec3.x = x, vec3.y = y, vec3.z = z;
	
	return vec3;
}

Vector4 Camera::convertToVector4(Vector3 vec3, GLfloat vec) {
	GLfloat x = vec3.x, y = vec3.y, z = vec3.z;

	return Vector4(x, y, z, vec);
}

void Camera::moveOx(GLfloat directie) {
	Vector3 position3 = convertToVector3(position);
	Vector3 target3 = convertToVector3(target);
	Vector3 up3 = convertToVector3(up);

	//moveLeft
	Vector3 move3 = (-(target3 - position3).Cross(up3)).Normalize();
	Vector4 move4 = convertToVector4(move3, 0.0f);
	Vector4 move = move4 * directie;
	Vector4 vectorDeplasare = move * moveSpeed * deltaTime;
	
	position += vectorDeplasare;
	target += vectorDeplasare;
	
	updateWorldView();
}

void Camera::moveOy(GLfloat directie) {
	Vector4 moveUp = up.Normalize() * directie;
	Vector4 vectorDeplasare = moveUp * moveSpeed * deltaTime;
	
	position += vectorDeplasare;
	target += vectorDeplasare;
	
	updateWorldView();
}

void Camera::moveOz(GLfloat directie) {
	Vector4 forwardLocal = -(target - position).Normalize() * directie;
	Vector4 vectorDeplasare = forwardLocal * moveSpeed * deltaTime;
	
	position += vectorDeplasare;
	target += vectorDeplasare;
	
	updateWorldView();
}

void Camera::rotateOx(GLfloat directie) {
	GLfloat angle = rotationSpeed * deltaTime * directie;
	
	Matrix mRotateOX;
	mRotateOX.SetRotationX(angle);
	
	Vector4 rotatedLocalUp = localUp * mRotateOX;
	up = rotatedLocalUp * worldMatrix;
	up = up.Normalize();
	
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f);
	Vector4 rotatedTarget = localTarget * mRotateOX;
	target = rotatedTarget * worldMatrix;
	
	updateWorldView();
}

void Camera::rotateOy(GLfloat directie) {
	GLfloat angle = rotationSpeed * deltaTime * directie;
	
	Matrix mRotateOY;
	mRotateOY.SetRotationY(angle);
	
	Vector4 localTarget = Vector4(0.0f, 0.0f, -(target - position).Length(), 1.0f); //target
	Vector4 rotatedTarget = localTarget * mRotateOY;
	//matrice de rotatie apelare set rotation de y cu 0.3 ... * timp (deltaTime)
	target = rotatedTarget * worldMatrix;
	
	updateWorldView();
}

void Camera::rotateOz(GLfloat directie) {
	GLfloat angle = rotationSpeed * deltaTime * direction;

	Matrix mRotateOZ;
	mRotateOZ.SetRotationZ(angle);

	Vector4 rotatedLocalUp = localUp * mRotateOZ;
	Vector4 up = rotatedLocalUp * worldMatrix;
	up.Normalize();

	updateWorldView();
}

void Camera::updateWorldView() {
	zAxis = -(target - position).Normalize();
	yAxis = up.Normalize();
	
	Vector3 xAxis3 = convertToVector3(xAxis);
	Vector3 yAxis3 = convertToVector3(yAxis);
	Vector3 zAxis3 = convertToVector3(zAxis);
	
	xAxis3 = zAxis3.Cross(yAxis3).Normalize();
	xAxis = convertToVector4(xAxis3, 0.0);

	Matrix R, T, invR, invT;
	R.m[0][0] = xAxis.x, R.m[0][1] = xAxis.y, R.m[0][2] = xAxis.z; R.m[0][3] = 0;
	R.m[1][0] = yAxis.x, R.m[1][1] = yAxis.y, R.m[1][2] = yAxis.z; R.m[1][3] = 0;
	R.m[2][0] = zAxis.x, R.m[2][1] = zAxis.y, R.m[2][2] = zAxis.z; R.m[2][3] = 0;
	R.m[3][0] = 0,       R.m[3][1] = 0,       R.m[3][2] = 0;       R.m[3][3] = 1;

	T.SetTranslation(position.x, position.y, position.z);

	worldMatrix = R * T;
	
	invT.SetTranslation(-position.x, -position.y, -position.z);
	invR = R.Transpose();
	viewMatrix = invT * invR;
}