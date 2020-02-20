#pragma once

#include "../Utilities/utilities.h"
using namespace std;

class Camera {
public:
	Camera();
	~Camera();

	Vector4 position, rotation, target, up, xAxis, yAxis, zAxis;
	Matrix viewMatrix, worldMatrix, perspMatrix;
	GLfloat moveSpeed, rotationSpeed, Near, Far, fov;
	Matrix MVP;
	GLfloat deltaTime;


	//GLfloat angle;
	Vector4 localUp = Vector4(0.0f, 1.0f, 0.0f, 0.0f);
	GLfloat direction = 1.0f; //1 sau -1

	void setDeltaTime(GLfloat);

	//convertori
	Vector3 convertToVector3(Vector4);
	Vector4 convertToVector4(Vector3, GLfloat);

	void moveOx(GLfloat);
	void moveOy(GLfloat);
	void moveOz(GLfloat);

	void rotateOx(GLfloat);
	void rotateOy(GLfloat);
	void rotateOz(GLfloat);

	void updateWorldView();
};


