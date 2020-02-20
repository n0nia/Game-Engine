#pragma once
#include "stdafx.h"
#include <fstream>
#include <sstream>
#include <iostream>
#include "../Resources/Xml/rapidxml-1.13/rapidxml.hpp"

using namespace std;
using namespace rapidxml;

class xmlDoc
{
	xml_document<> doc;//character type defaults to char
	string content;

public:
	xmlDoc();
	void xmlLoad(char*);
	xml_node<>* getRootNode();
	void childrenNode(const xml_node<>*);
};

