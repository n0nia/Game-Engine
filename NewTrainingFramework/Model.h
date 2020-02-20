#pragma once
#include "../Utilities/utilities.h"
#include "Structures.h"
#include <vector>
#include "Vertex.h"
using namespace std;

class Model {
public:
	Model();
	~Model();

	ModelResource* mr;
	GLuint id, iboId, wiredIboId, vboId;
	GLushort nrIndici, nrIndiciWired;
	
	string file;

	string Extension(string str);//nfg sau obj
	void LoadModel();
	void LoadNFG();
	
	Vertex* vertexData;
	GLushort* indiciData;
	int nrVertex;
};


