#include "stdafx.h"
#include "Shaders.h"

int Shaders::LoadShader() {
	vertexShader = esLoadShader(GL_VERTEX_SHADER, (char*)sr->vs.c_str());

	if (vertexShader == 0) {
		return -1;
	}

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, (char*)sr->fs.c_str());

	if (fragmentShader == 0) {
		glDeleteShader(vertexShader);
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	positionAttribute = glGetAttribLocation(program, "a_posL");
	normAttribute = glGetAttribLocation(program, "a_normL");
	binormAttribute = glGetAttribLocation(program, "a_binormL");
	tgtAttribute = glGetAttribLocation(program, "a_tgtL");
	
	colorAttribute = glGetAttribLocation(program, "a_color");

	mvpUniform = glGetUniformLocation(program, "u_MVP");
	modelUniform = glGetUniformLocation(program, "u_Model");
	viewUniform = glGetUniformLocation(program, "u_View");
	projectionUniform = glGetUniformLocation(program, "u_Projection");

	uvAttribute = glGetAttribLocation(program, "a_uv");
	uvtAttribute = glGetAttribLocation(program, "a_uvt");

	nrCel = glGetUniformLocation(program, "nrCel");
	heightTerrainUniform = glGetUniformLocation(program, "u_height");

	/*fire*/
	uTime = glGetUniformLocation(program, "u_Time");
	dispMax = glGetUniformLocation(program, "u_DispMax");
	
	/*fog*/
	rr = glGetUniformLocation(program, "u_r");
	rR = glGetUniformLocation(program, "u_R");
	rFog = glGetUniformLocation(program, "u_rFog");
	colorFog = glGetUniformLocation(program, "u_colorFog");

	/*light*/
	positionLight = glGetUniformLocation(program, "u_positionLight");
	ambientalColor = glGetUniformLocation(program, "u_ambiental");
	diffuseColor = glGetUniformLocation(program, "u_diffuseColor");
	specularColor = glGetUniformLocation(program, "u_specularColor");
	specularCoef = glGetUniformLocation(program, "u_specularCoef");
	diffuseCoef = glGetUniformLocation(program, "u_diffCoef");
	shininess = glGetUniformLocation(program, "u_shininess");
	ratio = glGetUniformLocation(program, "u_ratio");
	camera = glGetUniformLocation(program, "u_cam");
	specularPower = glGetUniformLocation(program, "u_specularPower");
	attenuation = glGetUniformLocation(program, "u_attenuation");
	type = glGetUniformLocation(program, "u_type");



	texture = new GLuint[10];
	texture[0] = glGetUniformLocation(program, "u_texture_0");
	texture[1] = glGetUniformLocation(program, "u_texture_1");
	texture[2] = glGetUniformLocation(program, "u_texture_2");
	texture[3] = glGetUniformLocation(program, "u_texture_3");

	cout << "Textura: ";

	for (int i = 0; i < 4; i++) {
		cout << texture[i] << " ";
	}
	cout << "\n";

	return 0;
}

/*int Shaders::Init(char * fileVertexShader, char * fileFragmentShader)
{
	vertexShader = esLoadShader(GL_VERTEX_SHADER, fileVertexShader);

	if ( vertexShader == 0 )
		return -1;

	fragmentShader = esLoadShader(GL_FRAGMENT_SHADER, fileFragmentShader);

	if ( fragmentShader == 0 )
	{
		glDeleteShader( vertexShader );
		return -2;
	}

	program = esLoadProgram(vertexShader, fragmentShader);

	//finding location of uniforms / attributes
	positionAttribute = glGetAttribLocation(program, "a_posL");

	return 0;
}*/

Shaders::~Shaders()
{
	glDeleteProgram(program);
	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);
}