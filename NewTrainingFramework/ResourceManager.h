#pragma once
#include "../Utilities/utilities.h"
#include <map>
#include "Texture.h"
#include "Model.h"
#include "Shaders.h"
#include "Structures.h"
#include "Sound.h"
#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"

using namespace std;

class ResourceManager {
protected:
	ResourceManager();
	static ResourceManager* spInstance;

public:
	~ResourceManager();

	map<int, Model*> mrMap;
	map<int, ModelResource*> mrMapR;

	map<int, Shaders*> srMap;
	map<int, ShaderResource*> srMapR;

	map<int, Texture*> trMap;
	map<int, TextureResource*> trMapR;

	void Init();
	static ResourceManager* getInstance();
	void freeResources();
	
	Model* getModel(int);
	Shaders* getShader(int);
	Texture* getTexture(int);

	//sound
	/*map<int, Sound*> soundMap;
	map<int, SoundResource*> soundMapR;
	Sound* getSound(int id);
	FMOD::System* fmodSystem;*/
};

