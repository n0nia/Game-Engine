#include "stdafx.h"
#include "Events.h"
#include "SceneManager.h"

#include <algorithm>

using namespace std;

Events::Events() {
	firstEvC = NullEvent;
	firstEvN = NullEvent;
	ev1 = NullEvent;
	ev2 = NullEvent;
}

string terrainS = "Terrain"; string skyBoxS = "skyBox";
void Events::Collisionn(int id) {
	//idSound = id;
	map<GLint, SceneObject*> object = SceneManager::getInstance()->objectMap;
	for (map<GLint, SceneObject*>::iterator firstObject = object.begin(); firstObject != object.end(); firstObject++) {
		for (map<GLint, SceneObject*>::iterator secondObject = object.begin(); secondObject != object.end(); secondObject++) {

			if (firstObject->second->id != secondObject->second->id && (firstObject->second->type != terrainS) && (secondObject->second->type != terrainS) &&
				(firstObject->second->type != skyBoxS) && (secondObject->second->type != skyBoxS)) {



				if (firstObject->second->AABB[0].x <= secondObject->second->AABB[1].x && firstObject->second->AABB[1].x >= secondObject->second->AABB[0].x &&
					firstObject->second->AABB[0].y <= secondObject->second->AABB[1].y && firstObject->second->AABB[1].y >= secondObject->second->AABB[0].y &&
					firstObject->second->AABB[0].z <= secondObject->second->AABB[1].z && firstObject->second->AABB[1].z >= secondObject->second->AABB[0].z) {


					pair<int, int> obj = pair<int, int>(firstObject->second->id, secondObject->second->id);
					if (std::find(objectCollision.begin(), objectCollision.end(), obj) == objectCollision.end()) {
						objectCollision.push_back(obj);
						//SceneManager::getInstance()->soundMap[idSound]->play();
						cout << firstObject->second->name << ", " << secondObject->second->name << endl;
					}




				}
				else {

					pair<int, int> obj = pair<int, int>(firstObject->second->id, secondObject->second->id);
					if (std::find(objectCollision.begin(), objectCollision.end(), obj) != objectCollision.end()) {
						objectCollision.erase(std::remove(objectCollision.begin(), objectCollision.end(), obj), objectCollision.end());
					}

				}

			}
		}
	}
}
/*void Events::playSounds(int n) {
	if (ver != 1) {
		//SceneManager::getInstance()->soundMap[n]->play();
		ver = 1;
	}
}*/
