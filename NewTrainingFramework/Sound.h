#pragma once
#include "../Utilities/utilities.h"
#include "Structures.h"
#include <vector>
#include "Vertex.h"
#include "FMOD/fmod.h"
#include "FMOD/fmod.hpp"

class Sound {
public:
	Sound();
	~Sound();

	int id;
	string path, name;

	SoundResource* sR;
	
	void LoadSound();
	void play();

	FMOD::Sound* soundA;
};
