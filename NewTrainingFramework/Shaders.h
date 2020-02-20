#pragma once
#include "../Utilities/utilities.h"
#include "Structures.h"

class Shaders 
{
public:
	ShaderResource* sr;
	GLuint idShader;
	GLuint program, vertexShader, fragmentShader;
	GLint positionAttribute, normAttribute, binormAttribute, tgtAttribute, mvpUniform, modelUniform, colorAttribute, heightTerrainUniform;
	GLint nrCel, uvAttribute, uvtAttribute, blendMapUniform, grassUniform, rockUniform, dirtUniform;
	GLint textureUniform, normAttributealMap;
	GLint skyBoxTextureUniform;

	/*fire*/
	GLint fireMask, displacementMap;
	GLint uTime, dispMax;

	/*fog*/
	GLint rr, rR, rFog, colorFog;

	/*light*/
	GLint ambientalColor, diffuseColor, diffuseCoef, specularColor, specularCoef, shininess, ratio, positionLight, specularPower, attenuation, type;

	GLint viewUniform, projectionUniform;
	GLint attributeCoord, attributeUv;
	GLint uTexture;

	GLint camera; //SceneObject

	GLuint* texture;

	int LoadShader();
	~Shaders();

	/*GLuint program, vertexShader, fragmentShader;
	char fileVS[260];
	char fileFS[260];
	GLint positionAttribute;

	int Init(char * fileVertexShader, char * fileFragmentShader);
	~Shaders();*/
};