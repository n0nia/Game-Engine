#include "stdafx.h"
#include <iostream>
#include "Globals.h"
#include "../Resources/Xml/rapidxml-1.13/rapidxml.hpp"
#include "../Resources/Xml/rapidxml-1.13/rapidxml_iterators.hpp"
#include "../Resources/Xml/rapidxml-1.13/rapidxml_print.hpp"
#include "../Resources/Xml/rapidxml-1.13/rapidxml_utils.hpp"
#include "../Utilities/memDbg.h"
#include "Shaders.h"
#include "Vertex.h";
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Terrain.h"
#include "SkyBox.h"
#include "Fire.h"
#include "Sound.h"

#include <string>

using namespace std;
using namespace rapidxml;

SceneManager::SceneManager() {
	isWired = false;
}

SceneManager::~SceneManager() {
}

void SceneManager::wired() {
	if (!isWired) {
		isWired = true;
	}

	else if (isWired) {
		isWired = false;
	}
}

void SceneManager::debug() {
	if (!isDebug) {
		isDebug = true;
	}

	else if (isDebug) {
		isDebug = false;
	}
}

SceneManager* SceneManager::spInstance = NULL;

SceneManager* SceneManager::getInstance() {
	if (!spInstance)
		spInstance = new SceneManager();

	return spInstance;
}

void SceneManager::Init() {

	event = new Events();

	xml_document<> doc;

	file<> xmlFile("../Resources/Xml/sceneManager.xml");
	doc.parse<0>(xmlFile.data());

	xml_node<>* node = doc.first_node(doc.first_node()->name());

	gameName = node->first_node("gameName")->value();

	width = atof(node->first_node("defaultScreenSize")->first_node("width")->value());
	height = atof(node->first_node("defaultScreenSize")->first_node("height")->value());

	Vector3 backgroundColor;
	backgroundColor.x = atof(node->first_node("backgroundColor")->first_node("r")->value());
	backgroundColor.y = atof(node->first_node("backgroundColor")->first_node("g")->value());
	backgroundColor.z = atof(node->first_node("backgroundColor")->first_node("b")->value());

	for (xml_node<>* cameraNode = node->first_node("cameras")->first_node("camera"); cameraNode; cameraNode = cameraNode->next_sibling()) {
		Camera* camera = new Camera;

		xml_node<>* positionNode = cameraNode->first_node("position");
		camera->position.x = atof(positionNode->first_node("x")->value());
		camera->position.y = atof(positionNode->first_node("y")->value());
		camera->position.z = atof(positionNode->first_node("z")->value());
		camera->position.w = 1;

		/*xml_node<>* rotationNode = camNode->first_node("rotation");
		camera->rotation.x = atof(rotationNode->first_node("x")->value());
		camera->rotation.y = atof(rotationNode->first_node("y")->value());
		camera->rotation.z = atof(rotationNode->first_node("z")->value());
		camera->rotation.w = 1;*/

		xml_node<>* targetNode = cameraNode->first_node("target");
		camera->target.x = atof(targetNode->first_node("x")->value());
		camera->target.y = atof(targetNode->first_node("y")->value());
		camera->target.z = atof(targetNode->first_node("z")->value());
		camera->target.w = 0;

		xml_node<>* upNode = cameraNode->first_node("up");
		camera->up.x = atof(upNode->first_node("x")->value());
		camera->up.y = atof(upNode->first_node("y")->value());
		camera->up.z = atof(upNode->first_node("z")->value());
		camera->up.w = 0;

		camera->moveSpeed = atof(cameraNode->first_node("translationSpeed")->value());
		camera->rotationSpeed = atof(cameraNode->first_node("rotationSpeed")->value());
		camera->fov = atof(cameraNode->first_node("fov")->value());
		camera->Far = atof(cameraNode->first_node("far")->value());
		camera->Near = atof(cameraNode->first_node("near")->value());
		int id = atoi(cameraNode->first_attribute("id")->value());
		cameraMap[id] = camera;
	}

	activeCamera = atoi(node->first_node("activeCamera")->value());

	for (xml_node<>* objectNode = node->first_node("objects")->first_node("object"); objectNode; objectNode = objectNode->next_sibling()) {
		SceneObject* object;

		if (!strcmp(objectNode->first_node("type")->value(), "terrain")) {

			Terrain* terrain = new Terrain();
			terrain->nrCel = atoi(objectNode->first_node("nrCel")->value());
			terrain->dimCel = atof(objectNode->first_node("dimCel")->value());
			terrain->inaltimi = Vector3(atof(objectNode->first_node("inaltimi")->first_node("r")->value()),
				atof(objectNode->first_node("inaltimi")->first_node("g")->value()),
				atof(objectNode->first_node("inaltimi")->first_node("b")->value()));
			int activeCam = atoi(objectNode->first_node("followingCamera")->first_node("cameras")->first_node("camera")->value());
			terrain->camera = cameraMap[activeCam];
			terrain->generateModel();

			object = (SceneObject*)terrain;
		}

		else if (!strcmp(objectNode->first_node("type")->value(), "SkyBox")) {
			SkyBox* newSkyBox = new SkyBox();
			object = (SceneObject*)newSkyBox;
		}

		else if (!strcmp(objectNode->first_node("type")->value(), "fire")) {
			Fire* newFire = new Fire();
			newFire->dispMax = atof(objectNode->first_node("dispMax")->value());
			object = (SceneObject*)newFire;
		}

		/*else if (!strcmp(objectNode->first_node("type")->value(), "ReflectSkyBox")) {
			ReflectSkyBox* reflexteObj = new ReflectSkyBox();


			objNou = (SceneObject*)reflexteObj;
		}
		}*/
		else if (!strcmp(objectNode->first_node("type")->value(), "normal")) {
			object = new SceneObject();
		}
		if (!strcmp(objectNode->first_node("model")->value(), "generated")) {

		}

		else {
			object->model = ResourceManager::getInstance()->getModel(atof(objectNode->first_node("model")->value()));
		}

		if (objectNode->first_node("name"))
			object->name = objectNode->first_node("name")->value();

		object->shader = ResourceManager::getInstance()->getShader(atoi(objectNode->first_node("shader")->value()));

		object->isWired = false;

		object->id = atof(objectNode->first_attribute("id")->value());

		object->type = objectNode->first_node("type")->value();

		if (xml_node<>* textureNode = objectNode->first_node("textures")) {
			for (xml_node<>* textureNode2 = textureNode->first_node("texture"); textureNode2; textureNode2 = textureNode2->next_sibling()) {
				object->texture.push_back(ResourceManager::getInstance()->getTexture(atoi(textureNode2->first_attribute("id")->value())));
				cout << "\n" << atoi(textureNode2->first_attribute("id")->value()) << "\n";
			}
		}

		else if (xml_node<>* wiredNode = objectNode->first_node("wired")) {
			object->isWired = true;
		}

		object->position.x = atof(objectNode->first_node("position")->first_node("x")->value());
		object->position.y = atof(objectNode->first_node("position")->first_node("y")->value());
		object->position.z = atof(objectNode->first_node("position")->first_node("z")->value());

		object->pointPosition.push_back(Vector3(atof(objectNode->first_node("position")->first_node("x")->value()),
			atof(objectNode->first_node("position")->first_node("y")->value()),
			atof(objectNode->first_node("position")->first_node("z")->value())));


		object->rotation.x = atof(objectNode->first_node("rotation")->first_node("x")->value());
		object->rotation.y = atof(objectNode->first_node("rotation")->first_node("y")->value());
		object->rotation.z = atof(objectNode->first_node("rotation")->first_node("z")->value());

		object->scale.x = atof(objectNode->first_node("scale")->first_node("x")->value());
		object->scale.y = atof(objectNode->first_node("scale")->first_node("y")->value());
		object->scale.z = atof(objectNode->first_node("scale")->first_node("z")->value());

		/*if (objectNode->first_node("selfRotateSpeed")->value() != NULL) {
			object->selfRotateSpeed = atof(objectNode->first_node("selfRotateSpeed")->value());
		}*/

		if (objectNode->first_node("lights") != NULL) {
			object->lights = atof(objectNode->first_node("lights")->value());
		}




		if (objectNode->first_node("trajectory") != NULL) {
			object->startPoint = object->position;


			object->directionTrajectory = objectNode->first_node("trajectory")->first_attribute("direction")->value();
			object->speed = atof(objectNode->first_node("trajectory")->first_attribute("speed")->value());
			string altTr = "alternate", normTr = "normal";
			if (objectNode->first_node("trajectory")->first_attribute("direction")->value() == altTr) {
				if (atoi(objectNode->first_node("trajectory")->first_attribute("iteration")->value()) == -1) {
					object->iterationTrajectory = atoi(objectNode->first_node("trajectory")->first_attribute("iteration")->value());

				}
				else {
					object->iterationTrajectory = (2 * atoi(objectNode->first_node("trajectory")->first_attribute("iteration")->value())) - 1;
				}

			}
			else if (objectNode->first_node("trajectory")->first_attribute("direction")->value() == normTr) {
				if (atoi(objectNode->first_node("trajectory")->first_attribute("iteration")->value()) == -1) {
					object->iterationTrajectory = atoi(objectNode->first_node("trajectory")->first_attribute("iteration")->value());
				}
				else {
					object->iterationTrajectory = atoi(objectNode->first_node("trajectory")->first_attribute("iteration")->value()) - 1;
				}

			}





			if (!strcmp(objectNode->first_node("trajectory")->first_attribute("type")->value(), "linear")) {
				object->typeTrajectory = SceneObject::TypeTraj::T_LINEAR;
				for (xml_node<>* positionNode = objectNode->first_node("trajectory")->first_node("points")->first_node("point"); positionNode; positionNode = positionNode->next_sibling()) {
					object->pointPosition.push_back(Vector3(atof(positionNode->first_node("x")->value()),
						atof(positionNode->first_node("y")->value()),
						atof(positionNode->first_node("z")->value())));
				}
				object->pointSize = object->pointPosition.size() - 1;

			}
			else if (!strcmp(objectNode->first_node("trajectory")->first_attribute("type")->value(), "line_strip")) {
				object->typeTrajectory = SceneObject::TypeTraj::T_LINE_STRIP;
				for (xml_node<>* positionNode = objectNode->first_node("trajectory")->first_node("points")->first_node("point"); positionNode; positionNode = positionNode->next_sibling()) {
					object->pointPosition.push_back(Vector3(atof(positionNode->first_node("x")->value()),
						atof(positionNode->first_node("y")->value()),
						atof(positionNode->first_node("z")->value())));
				}
				object->pointSize = object->pointPosition.size() - 1;





			}
			else if (!strcmp(objectNode->first_node("trajectory")->first_attribute("type")->value(), "line_loop")) {
				object->typeTrajectory = SceneObject::TypeTraj::T_LINE_LOOP;
				for (xml_node<>* positionNode = objectNode->first_node("trajectory")->first_node("points")->first_node("point"); positionNode; positionNode = positionNode->next_sibling()) {
					object->pointPosition.push_back(Vector3(atof(positionNode->first_node("x")->value()),
						atof(positionNode->first_node("y")->value()),
						atof(positionNode->first_node("z")->value())));
				}
				object->pointPosition.push_back(Vector3(atof(objectNode->first_node("position")->first_node("x")->value()),
					atof(objectNode->first_node("position")->first_node("y")->value()),
					atof(objectNode->first_node("position")->first_node("z")->value())));
				object->pointSize = object->pointPosition.size() - 1;
			}
			else if (!strcmp(objectNode->first_node("trajectory")->first_attribute("type")->value(), "circle")) {
				object->typeTrajectory = SceneObject::TypeTraj::T_CIRCLE;
				xml_node<>* trajNode = objectNode->first_node("trajectory");
				object->center = Vector3(atof(trajNode->first_node("center")->first_node("x")->value()),
					atof(trajNode->first_node("center")->first_node("y")->value()),
					atof(trajNode->first_node("center")->first_node("z")->value()));
				object->radius = atoi(trajNode->first_node("radius")->value());
				xml_node<>* rotPlaneNode = objectNode->first_node("trajectory")->first_node("rotationPlane");
				object->rotationPlane = Vector3(atof(rotPlaneNode->first_node("x")->value()),
					atof(rotPlaneNode->first_node("y")->value()),
					atof(rotPlaneNode->first_node("z")->value()));

			}
		}
			objectMap[object->id] = object;
	}

		//fog
		xml_node<>* fogNode = node->first_node("fog");
		colorFog = Vector3(atof(fogNode->first_node("color")->first_node("r")->value()),
			atof(fogNode->first_node("color")->first_node("g")->value()),
			atof(fogNode->first_node("color")->first_node("b")->value()));

		rFog = atof(fogNode->first_node("r")->value());
		RFog = atof(fogNode->first_node("R")->value());
		rFogFull = atof(fogNode->first_node("Rfog")->value());

		//lights
		for (xml_node<>* lightNode = node->first_node("lights")->first_node("light"); lightNode; lightNode = lightNode->next_sibling()) {
			Lights* newLight = new Lights();

			newLight->id = atoi(lightNode->first_attribute("id")->value());
			newLight->associatedObj = atoi(lightNode->first_node("associatedObject")->value());

			string type = lightNode->first_node("type")->value();
			if (type == "directional") {
				newLight->type = 0.0;
			}
			else if (type == "point") {
				newLight->type = 1.0;
			}

			newLight->specularCoef = atof(lightNode->first_node("specCoef")->value());
			newLight->specularPower = atof(lightNode->first_node("specPower")->value());
			newLight->diffuseCoef = atof(lightNode->first_node("diffCoef")->value());
			newLight->shininess = atof(lightNode->first_node("shininess")->value());
			newLight->attenuation = atof(lightNode->first_node("attenuation")->value());
			newLight->ratio = atof(lightNode->first_node("ratio")->value());

			newLight->position = Vector3(atof(lightNode->first_node("position")->first_node("x")->value()),
				atof(lightNode->first_node("position")->first_node("y")->value()),
				atof(lightNode->first_node("position")->first_node("z")->value()));

			newLight->ambiental = Vector3(atof(lightNode->first_node("ambiental")->first_node("r")->value()),
				atof(lightNode->first_node("ambiental")->first_node("g")->value()),
				atof(lightNode->first_node("ambiental")->first_node("b")->value()));

			newLight->diffuseColor = Vector3(atof(lightNode->first_node("diffuseColor")->first_node("r")->value()),
				atof(lightNode->first_node("diffuseColor")->first_node("g")->value()),
				atof(lightNode->first_node("diffuseColor")->first_node("b")->value()));

			newLight->specularColor = Vector3(atof(lightNode->first_node("specularColor")->first_node("r")->value()),
				atof(lightNode->first_node("specularColor")->first_node("g")->value()),
				atof(lightNode->first_node("specularColor")->first_node("b")->value()));


			lightMap[atoi(lightNode->first_attribute("id")->value())] = newLight;

			int asffff = 3;
		}

		//sound
		/*xml_node<>* soundNode = node->first_node("sounds");

		for (xml_node<>* sounds = soundNode->first_node("sound"); sounds; sounds = sounds->next_sibling()) {
			Sound* newSound = new Sound();
			newSound = ResourceManager::getInstance()->getSound(atoi(sounds->first_attribute("id")->value()));
			newSound->name = sounds->first_node("name")->value();
			soundMap[newSound->id] = newSound;
		}*/
}

void SceneManager::Draw(GLfloat deltaTime) {

	cameraMap[1]->updateWorldView();

	for (map<GLint, SceneObject*>::iterator it = objectMap.begin(); it != objectMap.end(); it++) {
		it->second->Draw(it->second->isWired);
	}
}

void SceneManager::Update(GLfloat deltaTime) {
	
	for (map<GLint, SceneObject*>::iterator it = objectMap.begin(); it != objectMap.end(); it++) {
		it->second->Update(deltaTime);
		if (it->second->texture.size()) {
			it->second->isWired = isWired;
		}
	}

	event->Collisionn(2);
	//event->playSound(1);
}
