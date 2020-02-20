#pragma once

#include "../Utilities/utilities.h"
#include "Model.h"
#include <vector>
#include <string>
#include "Shaders.h"
#include "Texture.h"


using namespace std;

class SceneObject {
public:
	SceneObject();
	~SceneObject();

	Model* model;
	vector<Texture*> texture;
	Shaders* shader;

	GLint id, followingCamera;

	Vector3 position, rotation, scale;

	string  name, type;
	string viewMode;

	GLfloat rotateSpeed, dispMax;

	Matrix matrix;

	bool isWired;

	void virtual Draw(bool);
	void virtual Update(GLfloat);
	void drawData();

	//light
	GLint lights;

	//trajectory
	Vector3 startPoint, endPoint, center;
	vector<Vector3> pointPosition;
	Vector3 rotationPlane;

	enum TypeTraj { 
		T_STATIC, T_LINEAR, T_LINE_STRIP, T_LINE_LOOP, T_CIRCLE 
	};

	TypeTraj typeTrajectory;

	bool trajectory;
	int iterationTrajectory;

	const GLfloat PI = 3.1415;

	string directionTrajectory;
	Vector3 color;
	GLfloat speed;
	int radius, pointSize, sens, inc;
	GLfloat angle, selfRotateSpeed;

	/*debug*/
	GLuint vboDebugId, indDebug;
	Vector3	Min, Max;
	int nrIndiciWiredDebug = 24;
	int nrVertexDebug = 8;
	Vertex* vertexData;
	Vector3* AABB;
};

