#pragma once
#include "../Utilities/utilities.h"
#include "Structures.h"

using namespace std;

class Texture
{
public:
	Texture();
	~Texture();

	void LoadTexture();

	TextureResource* tr;
	string file;
	GLuint idTexture;
};

