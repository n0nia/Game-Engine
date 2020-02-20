#pragma once

#include "../Utilities/utilities.h"
#include "SceneObject.h"
#include "SceneManager.h"

class Fire : public SceneObject {
public:
	Fire();
	~Fire();

	float time;
	float dispMax;
	void Draw(bool);
	void Update(GLfloat);
};

