// NewTrainingFramework.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "../Utilities/utilities.h" // if you use STL, please include this line AFTER all other include
#include "Vertex.h"
#include "Shaders.h"
#include "stdio.h"
#include <conio.h>
#include "Globals.h"
#include "Camera.h"
#include <Windows.h>
//#include "xmlDoc.h"
#include "ResourceManager.h"
#include "SceneManager.h"

/*GLuint vboId, lineId, textureId;
Shaders myShaders, lineShaders;
Matrix rot;
Model myModel;
Camera* myCamera;


xmlDoc myDoc;*/

Matrix model, trans, rot, scale;
Camera myCamera;
Matrix MVP;
GLfloat time;
GLfloat deltaTime;

short int Globals::keyPressed = 0;
//short int Globals::effectPressed = 0;

int Init(ESContext* esContext)
{
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glEnable(GL_DEPTH_TEST);

	ResourceManager::getInstance()->Init();
	SceneManager::getInstance()->Init();
	//myCamera = SceneManager::getInstance()->getActiveCamera();

	return 0;

	/*//glEnable(GL_CULL_FACE);
	//glCullFace(GL_FRONT);
	//glFrontFace(GL_CCW);
	Vertex verticesData[4];
	Vertex line[2];

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);

	//myModel.Load("../../NewResourcesPacket/Models/croco.nfg");
	myModel.Load();

	//triangle data (heap)
	verticesData[0].pos.x = -0.5f;  verticesData[0].pos.y = 0.5f;  verticesData[0].pos.z = 0.0f;
	verticesData[1].pos.x = 0.5f;  verticesData[1].pos.y = 0.5f;  verticesData[1].pos.z = 0.0f;
	verticesData[2].pos.x = 0.5f;  verticesData[2].pos.y = -0.5f;  verticesData[2].pos.z = 0.0f;
	verticesData[3].pos.x = -0.5f;  verticesData[3].pos.y = -0.5f;  verticesData[3].pos.z = 0.0f;

	verticesData[0].color = Vector3(1.0, 0.0, 0.0);
	verticesData[1].color = Vector3(0.0, 1.0, 0.0);
	verticesData[2].color = Vector3(0.0, 0.0, 1.0);
	verticesData[3].color = Vector3(1.0, 1.0, 0.0);

	//line data
	line[0].pos.x = 0.0f; line[0].pos.y = -1.f; line[0].pos.z = 0.0f;
	line[1].pos.x = 0.0f; line[1].pos.y = 1.f; line[0].pos.z = 0.0f;

	//buffer object
	glGenBuffers(1, &vboId);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * myModel.getVertex().size(), &(myModel.getVertex()[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glGenBuffers(1, &lineId);
	glBindBuffer(GL_ARRAY_BUFFER, lineId);

	glBufferData(GL_ARRAY_BUFFER, sizeof(line), line, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//texture data
	GLint width, height, bpp;
	char* bufferTGA = LoadTGA("../../NewResourcesPacket/Textures/croco.tga", &width, &height, &bpp);
	glGenTextures(1, &textureId);
	glBindTexture(GL_TEXTURE_2D, textureId);

	int color_mode = GL_RGBA;
	if (bpp == 24)
		color_mode = GL_RGB;

	glTexImage2D(GL_TEXTURE_2D, 0, color_mode, width, height, 0, color_mode, GL_UNSIGNED_BYTE, bufferTGA);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glEnable(GL_DEPTH_TEST);

	//xml
	myDoc.xmlLoad("exemplu_xml.xml");
	myDoc.childrenNode(myDoc.getRootNode());

	return (myShaders.Init("../Resources/Shaders/TriangleShaderVS.vs", "../Resources/Shaders/TriangleShaderFS.fs") +
		lineShaders.Init("../Resources/Shaders/LineShaderVS.vs", "../Resources/Shaders/LineShaderFS.fs"));
	*/
}

GLfloat alpha = 0;
GLfloat alpha2 = 0;

void Draw(ESContext* esContext)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	SceneManager::getInstance()->Draw(deltaTime);

	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);

	return;
	/*glUseProgram(myShaders.program);
	glBindBuffer(GL_ARRAY_BUFFER, vboId);

	rot.SetRotationZ(Globals::angle);
	if (myShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.positionAttribute);
		glVertexAttribPointer(myShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	if (myShaders.colorAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.colorAttribute);
		glVertexAttribPointer(myShaders.colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)sizeof(Vector3));
	}
	if (myShaders.matrixUniform != -1)
	{
		glUniformMatrix4fv(myShaders.matrixUniform, 1, GL_FALSE, (float*)rot.m);
	}
	if (myShaders.viewUniform != -1)
	{
		glUniformMatrix4fv(myShaders.viewUniform, 1, GL_FALSE, (float*)myCamera.getViewMatrix().m);
	}

	if (myShaders.textureAttribute != -1)
	{
		glEnableVertexAttribArray(myShaders.textureAttribute);
		glVertexAttribPointer(myShaders.textureAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(Vector3)));
	}

	if (Globals::mode == GL_TRIANGLES)
		glDrawElements(Globals::mode, myModel.getIndices().size(), GL_UNSIGNED_INT, &(myModel.getIndices()[0]));
	else
		glDrawElements(Globals::mode, myModel.getWiredIndices().size(), GL_UNSIGNED_INT, &(myModel.getWiredIndices()[0]));

	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glUseProgram(lineShaders.program);
	glBindBuffer(GL_ARRAY_BUFFER, lineId);
	if (lineShaders.positionAttribute != -1)
	{
		glEnableVertexAttribArray(lineShaders.positionAttribute);
		glVertexAttribPointer(lineShaders.positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}
	glDrawArrays(GL_LINES, 0, 2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	eglSwapBuffers(esContext->eglDisplay, esContext->eglSurface);
	*/
}

void Key(ESContext* esContext, unsigned char key, bool bIsPressed)
{
	if (bIsPressed) {
		switch (key) {
		case 'W':
			Globals::keyPressed |= 1 << Globals::moveForward;
			break;
		case 'S':
			Globals::keyPressed |= 1 << Globals::moveBack;
			break;
		case 'A':
			Globals::keyPressed |= 1 << Globals::moveLeft;
			break;
		case 'D':
			Globals::keyPressed |= 1 << Globals::moveRight;
			break;
		case 'Q':
			Globals::keyPressed |= 1 << Globals::moveDown;
			break;
		case 'E':
			Globals::keyPressed |= 1 << Globals::moveUp;
			break;

		case 'I':
			Globals::keyPressed |= 1 << Globals::rotateUp;
			break;
		case 'K':
			Globals::keyPressed |= 1 << Globals::rotateDown;
			break;
		case 'L':
			Globals::keyPressed |= 1 << Globals::rotateRight;
			break;
		case 'J':
			Globals::keyPressed |= 1 << Globals::rotateLeft;
			break;
		case 'P':
			SceneManager::getInstance()->wired();
			break;
		case 'U':
			SceneManager::getInstance()->debug();
			break;
		default:
			break;
		}
	}

	else {
		switch (key) {
		case 'W':
			Globals::keyPressed &= ~(1 << Globals::moveForward);
			break;
		case 'S':
			Globals::keyPressed &= ~(1 << Globals::moveBack);
			break;
		case 'A':
			Globals::keyPressed &= ~(1 << Globals::moveLeft);
			break;
		case 'D':
			Globals::keyPressed &= ~(1 << Globals::moveRight);
			break;
		case 'Q':
			Globals::keyPressed &= ~(1 << Globals::moveDown);
			break;
		case 'E':
			Globals::keyPressed &= ~(1 << Globals::moveUp);
			break;
		
		case 'I':
			Globals::keyPressed &= ~(1 << Globals::rotateUp);
			break;
		case 'K':
			Globals::keyPressed &= ~(1 << Globals::rotateDown);
			break;
		case 'L':
			Globals::keyPressed &= ~(1 << Globals::rotateRight);
			break;
		case 'J':
			Globals::keyPressed &= ~(1 << Globals::rotateLeft);
			break;
		default:
			break;
		}
	}

	/*if (bIsPressed)
		Globals::isPressed[key] = true;

	else Globals::isPressed[key] = false;*/
}

void Update(ESContext* esContext, float deltaTime)
{
	int idCamera = 1;
	SceneManager::getInstance()->cameraMap[idCamera]->setDeltaTime(deltaTime);
	SceneManager::getInstance()->Update(deltaTime);


	if (Globals::keyPressed & (1 << Globals::moveForward))
		SceneManager::getInstance()->cameraMap[idCamera]->moveOz(-1);
	if (Globals::keyPressed & (1 << Globals::moveBack))
		SceneManager::getInstance()->cameraMap[idCamera]->moveOz(1);
	if (Globals::keyPressed & (1 << Globals::moveLeft))
		SceneManager::getInstance()->cameraMap[idCamera]->moveOx(-1);
	if (Globals::keyPressed & (1 << Globals::moveRight))
		SceneManager::getInstance()->cameraMap[idCamera]->moveOx(1);
	if (Globals::keyPressed & (1 << Globals::moveDown))
		SceneManager::getInstance()->cameraMap[idCamera]->moveOy(-1);
	if (Globals::keyPressed & (1 << Globals::moveUp))
		SceneManager::getInstance()->cameraMap[idCamera]->moveOy(1);

	if (Globals::keyPressed & (1 << Globals::rotateDown))
		SceneManager::getInstance()->cameraMap[idCamera]->rotateOx(-1);
	if (Globals::keyPressed & (1 << Globals::rotateUp))
		SceneManager::getInstance()->cameraMap[idCamera]->rotateOx(1);
	if (Globals::keyPressed & (1 << Globals::rotateRight))
		SceneManager::getInstance()->cameraMap[idCamera]->rotateOy(-1);
	if (Globals::keyPressed & (1 << Globals::rotateLeft))
		SceneManager::getInstance()->cameraMap[idCamera]->rotateOy(1);
	if (Globals::keyPressed & (1 << Globals::rotateQ))
		SceneManager::getInstance()->cameraMap[idCamera]->rotateOz(-1);
	if (Globals::keyPressed & (1 << Globals::rotateE))
		SceneManager::getInstance()->cameraMap[idCamera]->rotateOz(1);


	/*Globals::elapsedUpdate += deltaTime;
	myCamera.setDeltaTime(deltaTime);
	myCamera.updateWorldView();

	if (Globals::elapsedUpdate >= Globals::tillUpdate)
	{
		POINT pct;
		GetCursorPos(&pct);
		ScreenToClient(esContext->hWnd, &pct);
		if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			if (pct.x < Globals::screenWidth / 2)
				Globals::angle += Globals::step * deltaTime;
			if (pct.x > Globals::screenWidth / 2)
				Globals::angle -= Globals::step * deltaTime;
		}

		//move
		if (Globals::isPressed['A'])
			myCamera.moveOx(-1);
		if (Globals::isPressed['D'])
			myCamera.moveOx(1);
		if (Globals::isPressed['W'])
			myCamera.moveOz(-1);
		if (Globals::isPressed['S'])
			myCamera.moveOz(1);
		if (Globals::isPressed['Q'])
			myCamera.moveOy(-1);
		if (Globals::isPressed['E'])
			myCamera.moveOy(1);

		//rotate
		if (Globals::isPressed[VK_DOWN])
			myCamera.rotateOx(-1);
		if (Globals::isPressed[VK_UP])
			myCamera.rotateOx(1);
		if (Globals::isPressed[VK_RIGHT])
			myCamera.rotateOy(-1);
		if (Globals::isPressed[VK_LEFT])
			myCamera.rotateOy(1);
		if (Globals::isPressed['O'])
			myCamera.rotateOz(-1);
		if (Globals::isPressed['P'])
			myCamera.rotateOz(1);

		if (Globals::isPressed['V'])
		{
			if (Globals::mode == GL_TRIANGLES)
				Globals::mode = GL_LINES;
			else
				Globals::mode = GL_TRIANGLES;
			Globals::isPressed['V'] = 0;
		}

		Globals::elapsedUpdate -= Globals::tillUpdate;
	}*/
}

void CleanUp()
{
	//glDeleteBuffers(1, &vboId);
	//glDeleteBuffers(1, &lineId);
}

int _tmain(int argc, _TCHAR* argv[])
{

	//identifying memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	ESContext esContext;

	esInitContext(&esContext);

	esCreateWindow(&esContext, "Hello Triangle", Globals::screenWidth, Globals::screenHeight, ES_WINDOW_RGB | ES_WINDOW_DEPTH);

	if (Init(&esContext) != 0)
		return 0;

	esRegisterDrawFunc(&esContext, Draw);
	esRegisterUpdateFunc(&esContext, Update);
	esRegisterKeyFunc(&esContext, Key);
	esMainLoop(&esContext);

	//releasing OpenGL resources
	CleanUp();


	printf("Press any key...\n");
	_getch();


	return 0;
}

