#pragma once

#include "../Utilities/utilities.h"
#include "SceneObject.h"
#include "SceneManager.h"

class SkyBoxReflect: public SceneObject {
public:
	SkyBoxReflect();
	~SkyBoxReflect();

	void Draw(bool);
};

