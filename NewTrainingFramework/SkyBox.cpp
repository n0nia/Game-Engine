#include "stdafx.h"
#include "../Utilities/utilities.h"
#include "Camera.h"
#include "SceneManager.h"
#include "SkyBox.h"

SkyBox::SkyBox() {

}

SkyBox::~SkyBox() {

}

void SkyBox::Update(GLfloat deltaTime) {
	Camera* camera = SceneManager::getInstance()->cameraMap[SceneManager::getInstance()->activeCamera];

	Matrix translation, rotx, roty, rotz, rot, scal;

	translation.SetTranslation(position);
	rotx.SetRotationX(rotation.x);
	roty.SetRotationY(rotation.y);
	rotz.SetRotationZ(rotation.z);

	rot = rotx * roty * rotz;

	scal.SetScale(scale);

	matrix = scal * rot * translation;

	position = Vector3(camera->convertToVector3(camera->position));
	position.y = camera->position.y;
}
