#pragma once

#include "../Utilities/utilities.h"
#include "Camera.h"
#include "SceneObject.h"
#include <map>
#include "Model.h"
#include "Shaders.h"
#include "Texture.h"
#include "Lights.h"
#include "Events.h"

using namespace std;

class SceneManager {
protected:
	SceneManager();
	static SceneManager* spInstance;

public:
	~SceneManager();

	static SceneManager* getInstance();

	map<GLint, SceneObject*> objectMap;
	map<GLint, Camera*> cameraMap;

	int activeCamera;

	char* gameName;
	float height, width;
	bool fullscreen = false;
	ESContext* esContext;

	void wired();
	bool isWired;

	void debug();
	bool isDebug;

	void Init();
	void Draw(GLfloat);
	void Update(GLfloat);

	//fog
	float rFog, RFog, rFogFull;
	Vector3 colorFog;

	//light
	map<GLint, Lights*> lightMap;

	//collision
	//map<GLint, Sound*> soundMap;
	Events* event;
};

