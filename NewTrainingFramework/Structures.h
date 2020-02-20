#pragma once

#include "stdafx.h"
#include "../Utilities/utilities.h"
#include <iostream>
#include <string>

using namespace std;

struct ModelResource {
	GLuint idModel;
	string path;
};

struct ShaderResource {
	GLuint idShader;
	string vs, fs;
};

struct TextureResource {
	GLuint idTexture, id;
	string type, path, min_filter, mag_filter, wrap_s, wrap_t;
};

struct SoundResource {
	int id;
	string path;
};
