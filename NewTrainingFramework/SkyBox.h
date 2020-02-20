#pragma once

#include "../Utilities/utilities.h"
#include "SceneObject.h"

class SkyBox: public SceneObject {
public:

	SkyBox();
	~SkyBox();

	void Update(GLfloat);
};

