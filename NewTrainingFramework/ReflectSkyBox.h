#pragma once

#include "../Utilities/utilities.h"
#include "SceneObject.h"
#include "SceneManager.h"
#include "Camera.h"
#include "Vertex.h"
#include <string>
#include <vector>

class ReflectSkyBox: public SceneObject {
public:
	ReflectSkyBox();
	~ReflectSkyBox();

	void Draw(bool);
};

