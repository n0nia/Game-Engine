#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "Fire.h"
#include <time.h>

Fire::Fire() {

}

Fire::~Fire() {

}

void Fire::Draw(bool isWired) {
	glUseProgram(shader->program);
	drawData();

	if (shader->uTime != -1) {
		glUniform1f(shader->uTime, time);
	}

	if (shader->dispMax != -1) {
		glUniform1f(shader->dispMax, dispMax);
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!isWired) {
		glDrawElements(GL_TRIANGLES, model->nrIndici, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	else if (isWired) {
		glDrawElements(GL_LINES, model->nrIndiciWired, GL_UNSIGNED_SHORT, 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

void Fire::Update(GLfloat deltaTime) {

	Matrix translation, rotationX, rotationY, rotationZ, rot, scal;
	
	translation.SetTranslation(position);
	
	rotationX.SetRotationX(rotation.x);
	rotationY.SetRotationY(rotation.y);
	rotationZ.SetRotationZ(rotation.z);
	rot = rotationX * rotationY * rotationZ;
	
	scal.SetScale(scale);
	matrix = scal * rot * translation;


	clock_t t = clock();
	
	char buffer[64];
	snprintf(buffer, sizeof buffer, "%f", (float)t / CLOCKS_PER_SEC);
	//pune in buffer, max bytes, in ce format, ce o sa puna in buffer
	//(float)t / CLOCKS_PER_SEC - nr de secunde

	int length = 0;
	int secunde = (int)t / CLOCKS_PER_SEC;
	while (secunde /= 10) {
		length++;
	}

	char* ti = buffer + length;
	time = atof(ti);
	//float fireSpeed = 0.9;
	//time = deltaTime ;

	//float ti =(float) t / CLOCKS_PER_SEC/ 10;
	//ti -= floor(ti);
	//time = ti;
	//cout << ti << "\n";
}