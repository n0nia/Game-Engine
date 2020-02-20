#include "stdafx.h"
#include "SceneObject.h"
#include "Terrain.h"
#include "SceneManager.h"
#include "Globals.h"
#include "ResourceManager.h"

Terrain::Terrain() {
	f1 = 0, f2 = 0;
}

Terrain::~Terrain() {

}

void Terrain::generateModel() {
	model = new Model();

	nrVertex = (nrCel + 1) * (nrCel + 1);
	nrIndici = ((nrCel * nrCel) * 2) * 3;
	vertexData = new Vertex[nrVertex];

	GLuint* indiciData = new GLuint[nrIndici];

	offsetY = 0;
	int nrV = 0;

	for (int i = 0; i <= nrCel; i++) {
		for (int j = 0; j <= nrCel; j++) {
			GLfloat z = camera->position.z - ((nrCel / 2) * dimCel) + (i * dimCel);
			GLfloat x = camera->position.x - ((nrCel / 2) * dimCel) + (j * dimCel);

			vertexData[nrV].position.x = x,			  vertexData[nrV].position.y = offsetY,		vertexData[nrV].position.z = z;
			vertexData[nrV].norm.x = 0,				  vertexData[nrV].norm.y = 1;				vertexData[nrV].norm.z = 0;
			vertexData[nrV].uv.x = j,				  vertexData[nrV].uv.y = i;
			vertexData[nrV].uvt.x = (float)j / nrCel, vertexData[nrV].uvt.y = (float)i / nrCel;

			nrV++;
		}
	}

	int nrI = 0;
	for (int i = 0; i < nrCel; i++) {
		for (int j = 0; j < nrCel; j++) {

			GLuint x = ((nrCel + 1) * i) + j;
			GLuint y = ((nrCel + 1) * i) + j + 1;
			GLuint z = nrCel * (i + 1) + i + j + 1;
			GLuint t = nrCel * (i + 1) + i + j + 2;

			indiciData[nrI] = x, indiciData[nrI + 1] = y, indiciData[nrI + 2] = z, indiciData[nrI + 3] = y, indiciData[nrI + 4] = z, indiciData[nrI + 5] = t;

			nrI += 6;
		}
	}

	nrIndiciWired = nrIndici * 2;

	GLuint* wiredData = new GLuint[nrIndiciWired];
	
	int j = 0;
	for (int i = 0; i < nrIndici; i += 3) {
		wiredData[j++] = indiciData[i];
		wiredData[j++] = indiciData[i + 1];
		wiredData[j++] = indiciData[i + 1];
		wiredData[j++] = indiciData[i + 2];
		wiredData[j++] = indiciData[i + 2];
		wiredData[j++] = indiciData[i];
	}

	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, nrVertex * sizeof(Vertex), vertexData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &iboId);
	glBindBuffer(GL_ARRAY_BUFFER, iboId);
	glBufferData(GL_ARRAY_BUFFER, nrIndici * sizeof(GLuint), indiciData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &wiredIboId);
	glBindBuffer(GL_ARRAY_BUFFER, wiredIboId);
	glBufferData(GL_ARRAY_BUFFER, nrIndiciWired * sizeof(GLuint), wiredData, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	delete[] indiciData;
	delete[] wiredData;

	model->nrIndici = nrIndici;
	model->nrIndiciWired = nrIndiciWired;
	model->iboId = iboId;
	model->vboId = vboId;
	model->wiredIboId = wiredIboId;
}

void Terrain::Draw(bool isWired) {
	glUseProgram(shader->program);

	drawData();


	if (shader->uvtAttribute != -1) {
		glEnableVertexAttribArray(shader->uvtAttribute);
		glVertexAttribPointer(shader->uvtAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(Vector3) + sizeof(Vector2)));
	}

	if (shader->heightTerrainUniform != -1) {
		glUniform3f(shader->heightTerrainUniform, inaltimi.x, inaltimi.y, inaltimi.z);
		int test = 3;
	}

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!isWired) {
		glDrawElements(GL_TRIANGLES, model->nrIndici, GL_UNSIGNED_INT, 0);
	}

	else if (isWired) {
		glDrawElements(GL_LINES, model->nrIndiciWired, GL_UNSIGNED_INT, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Terrain::Update(GLfloat deltaTime) {
	Matrix translation, rotationX, rotationY, rotationZ, rot, scal;

	int test = 0;
	if ((camera->position.z - position.z) < -dimCel) {
		position.z -= dimCel;
		f1 = 0, f2 = -1;
		test = 1;
	}

	else if ((camera->position.z - position.z) > dimCel) {
		position.z += dimCel;
		f1 = 0, f2 = 1;
		test = 1;
	}

	else if ((camera->position.x - position.x) < -dimCel) {
		position.x -= dimCel;
		f1 = -1, f2 = 0;
		test = 1;
	}

	else if ((camera->position.x - position.x) > dimCel) {
		position.x += dimCel;
		f1 = 1, f2 = 0;
		test = 1;
	}

	if (test == 1) {
		test = 0;

		for (int i = 0; i < nrVertex; i++) {
			vertexData[i].uvt.x += f1 / nrCel, vertexData[i].uvt.y += f2 / nrCel;
		}

		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, nrVertex * sizeof(Vertex), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	translation.SetTranslation(position);

	rotationX.SetRotationX(rotation.x);
	rotationY.SetRotationY(rotation.y);
	rotationZ.SetRotationZ(rotation.z);
	rot = rotationX * rotationY * rotationZ;
	
	scal.SetScale(scale);
	matrix = scal * rot * translation;
}