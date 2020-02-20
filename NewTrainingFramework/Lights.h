#pragma once

#include "../Utilities/utilities.h"
#include "../Utilities/memDbg.h"
#include <iostream>

class Lights {
public:
	int id, associatedObj;
	float type, specularCoef, diffuseCoef, specularPower, shininess, ratio, attenuation;
	Vector3 position, ambiental, specularColor, diffuseColor;
};

