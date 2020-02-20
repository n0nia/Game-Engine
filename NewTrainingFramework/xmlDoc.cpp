#include "stdafx.h"
#include "xmlDoc.h"

xmlDoc::xmlDoc()
{
}

void xmlDoc::xmlLoad(char* path)
{
	ifstream file(path);
	stringstream buffer;
	buffer << file.rdbuf();
	file.close();
	content = buffer.str();
	doc.parse<0>(&content[0]);
}

xml_node<>* xmlDoc::getRootNode()
{
	return doc.first_node();
}

void xmlDoc::childrenNode(const xml_node<>* pRoot)
{
	//for - ca sa ia din fiecare obiect
	for (xml_node<>* pNode = pRoot->first_node(); pNode; pNode = pNode->next_sibling())
		//xml_node<>* pNode = pRoot->first_node();
		if (pNode->name())
			childrenNode(pNode);

	for (xml_attribute<>* pAttr = pRoot->first_attribute(); pAttr; pAttr = pAttr->next_attribute())
		cout << pAttr->name() << " = " << pAttr->value() << "\n";

}
