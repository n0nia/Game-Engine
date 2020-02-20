#include "stdafx.h"
#include <iostream>
#include "Globals.h"
#include "../Utilities/memDbg.h"
#include "Shaders.h"
#include "Vertex.h"
#include "ResourceManager.h"
#include "Model.h"
#include <string>

using namespace std;

Model::Model() {

}

Model::~Model() {

}

string Model::Extension(string str) {
	size_t found = str.find_last_of(".");
	return str.substr(found + 1);
}

void Model::LoadModel() {
	LoadNFG();
}

void Model::LoadNFG() {

	vboId = 0;
	iboId = 0;
	wiredIboId = 0;

	nrVertex = 0;
	nrIndici = 0;
	nrIndiciWired = 0;

	float px, py, pz, nx, ny, nz, bx, by, bz, tx, ty, tz, ux, uy;
	//Vector3 position, norm, binorm, tgt;
	//Vector2 uv;

	FILE* file;

	GLushort* wiredData;

	char a[250];

	errno_t e = fopen_s(&file, mr->path.c_str(), "r");

	if (e) {
		cout << "Fisierul .nfg nu a fost gasit!";
	}
	else {
		fscanf_s(file, "%s", a, 250);
		fscanf_s(file, "%d", &nrVertex);

		vertexData = new Vertex[nrVertex];


		for (int i = 0; i < nrVertex; i++) {
			fscanf_s(file, "%*d. pos:[%f, %f, %f]; norm:[%f, %f, %f]; binorm:[%f, %f, %f]; tgt:[%f, %f, %f]; uv:[%f, %f];",
				&px, &py, &pz, &nx, &ny, &nz, &bx, &by, &bz, &tx, &ty, &tz, &ux, &uy);
			vertexData[i].position.x = px,	vertexData[i].position.y = py, vertexData[i].position.z = pz;
			vertexData[i].norm.x = nx,			vertexData[i].norm.y = ny,		   vertexData[i].norm.z = nz;
			vertexData[i].binorm.x = bx;		vertexData[i].binorm.y = by,	   vertexData[i].binorm.z = bz;
			vertexData[i].tgt.x = tx,			vertexData[i].tgt.y = ty,		   vertexData[i].tgt.z = tz;
			vertexData[i].uv.x = ux,				vertexData[i].uv.y = uy;
		}

		fscanf_s(file, "%s", a, 250);
		fscanf_s(file, "%d", &nrIndici);

		indiciData = new GLushort[nrIndici];

		GLint index;
		char b[10];

		for (int i = 0; i < nrIndici / 3; i++) {
			fscanf_s(file, "%d", &index);
			fscanf_s(file, "%1c%hu%1c%hu%1c%hu", b, 10, &indiciData[3 * i], b, 10, &indiciData[3 * i + 1], b, 10, &indiciData[3 * i + 2]);
		}


		fclose(file);

		
		nrIndiciWired = nrIndici * 2;
		wiredData = new GLushort[nrIndiciWired];
		int k = 0;
		for (int i = 0; i < nrIndici; i += 3) {
			wiredData[k++] = indiciData[i];
			wiredData[k++] = indiciData[i + 1];
			wiredData[k++] = indiciData[i + 1];
			wiredData[k++] = indiciData[i + 2];
			wiredData[k++] = indiciData[i + 2];
			wiredData[k++] = indiciData[i];
		}



		glGenBuffers(1, &vboId);
		glBindBuffer(GL_ARRAY_BUFFER, vboId);
		glBufferData(GL_ARRAY_BUFFER, nrVertex * sizeof(Vertex), vertexData, GL_STATIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		glGenBuffers(1, &iboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, iboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndici * sizeof(GLushort), indiciData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

		glGenBuffers(1, &wiredIboId);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, wiredIboId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, nrIndiciWired * sizeof(GLushort), wiredData, GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


		delete[] indiciData;
		delete[] wiredData;
	}
}
