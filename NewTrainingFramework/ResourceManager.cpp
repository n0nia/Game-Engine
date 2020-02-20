#include "stdafx.h"

#include <iostream>
#include "Globals.h"
#include "../Resources/Xml/rapidxml-1.13/rapidxml.hpp"
#include "../Resources/Xml/rapidxml-1.13/rapidxml_iterators.hpp"
#include "../Resources/Xml/rapidxml-1.13/rapidxml_print.hpp"
#include "../Resources/Xml/rapidxml-1.13/rapidxml_utils.hpp"
#include "ResourceManager.h"
#include "../Utilities/memDbg.h"
#include <string>

using namespace rapidxml;

ResourceManager::ResourceManager() {

}

ResourceManager::~ResourceManager() {

}

void ResourceManager::freeResources() {
	mrMapR.clear();
	srMapR.clear();
	trMapR.clear();
}

ResourceManager* ResourceManager::spInstance = NULL;

ResourceManager* ResourceManager::getInstance() {
	if (!spInstance) {
		spInstance = new ResourceManager();
	}
	return spInstance;
}

void ResourceManager::Init() {
	xml_document<> doc;
	file<> xmlFile("../Resources/Xml/resourceManager.xml");
	doc.parse<0>(xmlFile.data());

	xml_node<>* node = doc.first_node(doc.first_node()->name());

	string prefix = "../";

	for (xml_node<>* modelNode = node->first_node("models")->first_node("folder"); modelNode; modelNode = modelNode->next_sibling()) {

		for (xml_node<>* childNode = modelNode->first_node("model"); childNode; childNode = childNode->next_sibling()) {
			ModelResource* model = new ModelResource;

			model->idModel = atoi(childNode->first_attribute("id")->value());
			model->path = prefix + modelNode->first_attribute("path")->value() + childNode->first_node("file")->value();
			mrMapR[model->idModel] = model;
		}
	}

	for (xml_node<>* shaderNode = node->first_node("shaders")->first_node("folder"); shaderNode; shaderNode = shaderNode->next_sibling()) {
		
		for (xml_node<>* childNode = shaderNode->first_node("shader"); childNode; childNode = childNode->next_sibling()) {
			ShaderResource* shader = new ShaderResource;

			shader->idShader = atoi(childNode->first_attribute("id")->value());
			shader->vs = prefix + shaderNode->first_attribute("path")->value() + childNode->first_node("vs")->value();
			shader->fs = prefix + shaderNode->first_attribute("path")->value() + childNode->first_node("fs")->value();
			srMapR[shader->idShader] = shader;
		}
	}

	for (xml_node<>* textureNode = node->first_node("textures")->first_node("folder"); textureNode; textureNode = textureNode->next_sibling()) {
		string path = prefix + textureNode->first_attribute("path")->value();

		for (xml_node<>* childNode = textureNode->first_node("texture"); childNode; childNode = childNode->next_sibling()) {
			TextureResource* texture = new TextureResource;
			texture->path = path + childNode->first_node("file")->value();

			texture->idTexture = atoi(childNode->first_attribute("id")->value());
			texture->type = childNode->first_attribute("type")->value();
			texture->min_filter = childNode->first_node("min_filter")->value();
			texture->mag_filter = childNode->first_node("mag_filter")->value();
			texture->wrap_s = childNode->first_node("wrap_s")->value();
			texture->wrap_t = childNode->first_node("wrap_t")->value();
			trMapR[texture->idTexture] = texture;
		}
	}

	//sound
	/*if (FMOD::System_Create(&fmodSystem) != FMOD_OK) {
		cout << "nu avem sunete, vom afisa un mesaj de eroare";
	}
	else {
		fmodSystem->init(36, FMOD_INIT_NORMAL, NULL);//initializat la 36 de canale
		cout << "initializat la 36 de canale";
	}


	for (xml_node<>* soundNode = node->first_node("sounds")->first_node("folder"); soundNode; soundNode = soundNode->next_sibling()) {

		for (xml_node<>* childNode = soundNode->first_node("sound"); childNode; childNode = childNode->next_sibling()) {
			SoundResource* sound = new SoundResource;

			sound->id = atoi(childNode->first_attribute("id")->value());
			sound->path = prefix + soundNode->first_attribute("path")->value() + childNode->first_node("file")->value();
			soundMapR[sound->id] = sound;
		}
	}*/
}
	
Model* ResourceManager::getModel(int id) {
	if (mrMap.find(id) == mrMap.end()) {//dc nu gaseste
		Model* model = new Model;

		if (mrMapR.find(id) != mrMapR.end()) {//dc exista id-ul
			model->mr = mrMapR[id];
		}

		else {//dc nu exista id-ul
			cout << "Couldn't find model " << id << "\n";
			exit(0);
		}

		model->LoadModel();
		mrMap[id] = model;
	}

	return mrMap[id];
}

Shaders* ResourceManager::getShader(int id) {
	
	if (srMap.find(id) == srMap.end()) {//dc nu gaseste
		Shaders* shader = new Shaders;

		if (srMapR.find(id) != srMapR.end()) {//dc gaseste id-ul
			shader->sr = srMapR[id];
		}

		else {//dc nu gaseste id-ul
			cout << "Couldn't find shader " << id << "\n";
			exit(0);
		}

		shader->LoadShader();
		srMap[id] = shader;
	}

	return srMap[id];
}

Texture* ResourceManager::getTexture(int id) {
	if (trMap.find(id) == trMap.end()) {//dc nu gaseste
		Texture* texture = new Texture;
		
		if (trMapR.find(id) != trMapR.end()) {//dc gaseste id-ul
			texture->tr = trMapR[id];
		}

		else {//dc nu gaseste id-ul
			cout << "Coudln't find texture " << id << "\n";
			exit(0);
		}

		texture->LoadTexture();
		trMap[id] = texture;
	}

	return trMap[id];
}

/*Sound* ResourceManager::getSound(int id) {
	if (soundMap.find(id) == soundMap.end()) {
		Sound* sound = new Sound;

		if (soundMapR.find(id) != soundMapR.end()) {
			sound->id = soundMapR[id]->id;
			sound->path = soundMapR[id]->path;
			sound->sR = soundMapR[id];
		}

		else {
			cout << "Couldn't find sound " << id << "\n";
			exit(0);
		}

		sound->LoadSound();
		soundMap[id] = sound;
	}

	return soundMap[id];
}*/