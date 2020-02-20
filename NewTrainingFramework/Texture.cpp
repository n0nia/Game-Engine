#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "Globals.h"
#include "Shaders.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include "Texture.h"
#include <iostream>
#include <string>

using namespace std;

Texture::Texture() {
	idTexture = 0;
}

Texture::~Texture() {

}

void Texture::LoadTexture() {
	glGenTextures(1, &idTexture);
	GLuint glTextureType;

	if (tr->type == "2d") {
		glTextureType = GL_TEXTURE_2D;
	}
	else if (tr->type == "cube") {
		glTextureType = GL_TEXTURE_CUBE_MAP;
	}

	glBindTexture(glTextureType, idTexture);

	if (tr->wrap_s == "CLAMP_TO_EDGE") {
		glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}

	else if (tr->wrap_s == "GL_REPEAT") {
		glTexParameteri(glTextureType, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(glTextureType, GL_TEXTURE_WRAP_T, GL_REPEAT);
	}

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); //???

	GLint width, height, bpp;

	GLchar* pixelArray = LoadTGA(tr->path.c_str(), &width, &height, &bpp);
	
	GLuint textureRGB;
	if (bpp == 24) {
		textureRGB = GL_RGB;
	}
	else {
		textureRGB = GL_RGBA;
	}

	if (tr->type == "2d") {
		glTexImage2D(GL_TEXTURE_2D, 0, textureRGB, width, height, 0, textureRGB, GL_UNSIGNED_BYTE, (const GLvoid*)pixelArray);
	}
	else if (tr->type == "cube") {
		GLchar* buffer = new GLchar[width / 4 * height / 3 * (bpp / 8)];

		int offsetWNX = 0,			offsetWPX = (2 * (width / 4)) * (bpp / 8);
		int offsetHNX = height / 3, offsetHPX = height / 3;

		int offsetWNY = (width / 4) * (bpp / 8), offsetWPY = (width / 4) * (bpp / 8);
		int offsetHNY = 2 * (height / 3),		 offsetHPY = 0;

		int offsetWNZ = (3 * (width / 4)) * (bpp / 8), offsetWPZ = (width / 4) * (bpp / 8);
		int offsetHNZ = height / 3,					   offsetHPZ = height / 3;
	
		/*POSITIVE X*/
		int k = 0;
		for (int i = 0; i < height / 3; i++) {
			for (int j = 0; j < (width * (bpp / 8)) / 4; j++) {
				buffer[k++] = pixelArray[offsetWPX + offsetHPX * (bpp / 8) * width + (i * width * (bpp / 8)) + j];
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X, 0, textureRGB, width / 4, height / 3, 0, textureRGB, GL_UNSIGNED_BYTE, (const GLvoid*)buffer);

		
		/*NEGATIVE X*/
		k = 0;
		for (int i = 0; i < height / 3; i++) {
			for (int j = 0; j < (width * (bpp / 8)) / 4; j++) {
				buffer[k++] = pixelArray[offsetWNX + offsetHNX * (bpp / 8) * width + (i * width * (bpp / 8)) + j];
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_X, 0, textureRGB, width / 4, height / 3, 0, textureRGB, GL_UNSIGNED_BYTE, (const GLvoid*)buffer);

		
		/*POSITIVE Y*/
		k = 0;
		for (int i = 0; i < height / 3; i++) {
			for (int j = 0; j < (width * (bpp / 8)) / 4; j++) {
				buffer[k++] = pixelArray[offsetWPY + offsetHPY * (bpp / 8) * width + (i * width * (bpp / 8)) + j];
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Y, 0, textureRGB, width / 4, height / 3, 0, textureRGB, GL_UNSIGNED_BYTE, (const GLvoid*)buffer);

		/*NEGATIVE Y*/
		k = 0;
		for (int i = 0; i < height / 3; i++) {
			for (int j = 0; j < (width * (bpp / 8)) / 4; j++) {
				buffer[k++] = pixelArray[offsetWNY + offsetHNY * (bpp / 8) * width + (i * width * (bpp / 8)) + j];
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Y, 0, textureRGB, width / 4, height / 3, 0, textureRGB, GL_UNSIGNED_BYTE, (const GLvoid*)buffer);

		
		
		/*POSITIVE Z*/
		k = 0;
		for (int i = 0; i < height / 3; i++) {
			for (int j = 0; j < (width * (bpp / 8)) / 4; j++) {
				buffer[k++] = pixelArray[offsetWPZ + offsetHPZ * (bpp / 8) * width + (i * width * (bpp / 8)) + j];
			}
		}
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_Z, 0, textureRGB, width / 4, height / 3, 0, textureRGB, GL_UNSIGNED_BYTE, (const GLvoid*)buffer);

		/*NEGATIVE Z*/
		k = 0;
		for (int i = 0; i < height / 3; i++) {
			for (int j = 0; j < (width * (bpp / 8)) / 4; j++) {
				buffer[k++] = pixelArray[offsetWNZ + offsetHNZ * (bpp / 8) * width + (i * width * (bpp / 8)) + j];
			}
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_NEGATIVE_Z, 0, textureRGB, width / 4, height / 3, 0, textureRGB, GL_UNSIGNED_BYTE, (const GLvoid*)buffer);

	}

	//glGenerateMipmap(glTextureType); //???
	glTexParameteri(glTextureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR); //GL_LINEAR | GL_LINEAR_MIPMAP_LINEAR
	glTexParameteri(glTextureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	delete[] pixelArray;
}
