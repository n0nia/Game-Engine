#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "stdio.h"
#include "SceneObject.h"
#include "Vertex.h"
#include "Model.h"
#include "Camera.h"

using namespace std;

class Terrain: public SceneObject
{
public:
	Terrain();
	~Terrain();

	GLuint id, iboId, wiredIboId, nrVertex, nrIndici, nrIndiciWired;
	GLuint vboId;

	int nrCel;
	float dimCel, offsetY;
	Vector3 inaltimi;

	void generateModel();

	Vertex* vertexData;
	Camera* camera;
	
	virtual void Draw(bool);
	virtual void Update(GLfloat);

	float f1, f2;
};

