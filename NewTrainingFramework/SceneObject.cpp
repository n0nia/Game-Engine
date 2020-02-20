#include "stdafx.h"
#include <iostream>
#include "../Utilities/utilities.h"
#include "SceneManager.h"
#include "ResourceManager.h"
#include "Vertex.h"
#include "Globals.h"
#include <string>
#include <map>
#include "SceneObject.h"

using namespace std;

SceneObject::SceneObject() {
	sens = 1;
	inc = 0;
	angle = 0;
	AABB = new Vector3[2];
}

SceneObject::~SceneObject() {

}

void SceneObject::Draw(bool isWired) {
	glUseProgram(shader->program);

	drawData();

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	if (!isWired) {
		glDrawElements(GL_TRIANGLES, model->nrIndici, GL_UNSIGNED_SHORT, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindTexture(GL_TEXTURE_2D, 0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
	}

	else if (isWired) {
		glDrawElements(GL_LINES, model->nrIndiciWired, GL_UNSIGNED_SHORT, 0);
		//glBindBuffer(GL_ARRAY_BUFFER, 0);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void SceneObject::drawData() {
	glBindBuffer(GL_ARRAY_BUFFER, model->vboId);

	if (!isWired) {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->iboId);
	}
	else {
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, model->wiredIboId);
	}


	if (shader->positionAttribute != -1) {
		glEnableVertexAttribArray(shader->positionAttribute);
		glVertexAttribPointer(shader->positionAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	}

	if (shader->normAttribute != -1) {
		glEnableVertexAttribArray(shader->normAttribute);
		glVertexAttribPointer(shader->normAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(2 * sizeof(Vector3)));
	}

	if (shader->binormAttribute != -1) {
		glEnableVertexAttribArray(shader->binormAttribute);
		glVertexAttribPointer(shader->binormAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(Vector3)));
	}

	if (shader->tgtAttribute != -1) {
		glEnableVertexAttribArray(shader->tgtAttribute);
		glVertexAttribPointer(shader->tgtAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(4 * sizeof(Vector3)));
	}

	if (shader->colorAttribute != -1) {
		glEnableVertexAttribArray(shader->colorAttribute);
		glVertexAttribPointer(shader->colorAttribute, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)sizeof(Vector3));
	}

	Camera* camera = SceneManager::getInstance()->cameraMap[SceneManager::getInstance()->activeCamera];
	
	Matrix View = SceneManager::getInstance()->cameraMap[SceneManager::getInstance()->activeCamera]->viewMatrix;
	GLfloat ratio = (GLfloat)Globals::screenWidth / Globals::screenHeight;
	
	Matrix Perspective;
	Perspective.SetPerspective(camera->fov, ratio, camera->Near, camera->Far);
	
	Matrix MVP = matrix * View * Perspective;

	if (shader->mvpUniform != -1) {
		glUniformMatrix4fv(shader->mvpUniform, 1, GL_FALSE, (GLfloat*)MVP.m);
	}

	if (shader->modelUniform != -1) {
		glUniformMatrix4fv(shader->modelUniform, 1, GL_FALSE, (GLfloat*)matrix.m);
	}

	if (shader->viewUniform != -1) {
		glUniformMatrix4fv(shader->viewUniform, 1, GL_FALSE, (GLfloat*)View.m);
	}

	if (shader->projectionUniform != -1) {
		glUniformMatrix4fv(shader->projectionUniform, 1, GL_FALSE, (GLfloat*)Perspective.m);
	}

	if (shader->uvAttribute != -1) {
		glEnableVertexAttribArray(shader->uvAttribute);
		glVertexAttribPointer(shader->uvAttribute, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(5 * sizeof(Vector3)));
	}

	for (int i = 0; i < texture.size(); i++) {
		if (texture[i]->tr->type == "2d") {
			if (shader->texture[i] != -1) {
				glActiveTexture(GL_TEXTURE0 + i);
				glBindTexture(GL_TEXTURE_2D, texture[i]->idTexture);
				glUniform1i(shader->texture[i], i);
			}
		}
		else if (shader->texture[i] != -1) {
			glActiveTexture(GL_TEXTURE0 + i);
			glBindTexture(GL_TEXTURE_CUBE_MAP, texture[i]->idTexture);
			glUniform1i(shader->texture[i], i);
		}
	}

	if (shader->camera != -1) {
		glUniform3f(shader->camera, camera->position.x, camera->position.y, camera->position.z);
	}


	//fog
	if (shader->camera != -1) {
		glUniform3f(shader->camera, camera->position.x, camera->position.y, camera->position.z);
	}

	if (shader->rr != -1) {
		glUniform1f(shader->rr, SceneManager::getInstance()->rFog);
	}
	if (shader->rR != -1) {
		glUniform1f(shader->rR, SceneManager::getInstance()->RFog);
	}
	if (shader->rFog != -1) {
		glUniform1f(shader->rFog, SceneManager::getInstance()->rFogFull);
	}
	if (shader->colorFog != -1) {
		Vector3 colorFog = SceneManager::getInstance()->colorFog;
		glUniform3f(shader->colorFog, colorFog.x, colorFog.y, colorFog.z);
	}

	//light
	if (shader->positionLight != -1) {
		Vector3 positionLight = SceneManager::getInstance()->lightMap[0]->position;
		glUniform3f(shader->positionLight, positionLight.x, positionLight.y, positionLight.z);
		int testtt = 0;
	}
	if (shader->ambientalColor != -1) {
		Vector3 ambiental = SceneManager::getInstance()->lightMap[0]->ambiental;
		glUniform3f(shader->ambientalColor, ambiental.x, ambiental.y, ambiental.z);
		int testtt = 0;
	}
	if (shader->diffuseColor != -1) {
		Vector3 difuseColor = SceneManager::getInstance()->lightMap[0]->diffuseColor;
		glUniform3f(shader->diffuseColor, difuseColor.x, difuseColor.y, difuseColor.z);
		int testtt = 0;
	}
	if (shader->specularColor != -1) {
		Vector3 specularColor = SceneManager::getInstance()->lightMap[0]->specularColor;
		glUniform3f(shader->specularColor, specularColor.x, specularColor.y, specularColor.z);
	}
	if (shader->specularCoef != -1) {
		glUniform1f(shader->specularCoef, SceneManager::getInstance()->lightMap[0]->specularCoef);
	}
	if (shader->diffuseCoef != -1) {
		glUniform1f(shader->diffuseCoef, SceneManager::getInstance()->lightMap[0]->diffuseCoef);
	}
	if (shader->specularPower != -1) {
		glUniform1f(shader->specularPower, SceneManager::getInstance()->lightMap[0]->specularPower);
	}
	if (shader->shininess != -1) {
		glUniform1f(shader->shininess, SceneManager::getInstance()->lightMap[0]->shininess);
	}
	if (shader->ratio != -1) {
		glUniform1f(shader->ratio, SceneManager::getInstance()->lightMap[0]->ratio);
	}
	if (shader->attenuation != -1) {
		glUniform1f(shader->attenuation, SceneManager::getInstance()->lightMap[0]->attenuation);
	}
	if (shader->type != -1) {
		glUniform1f(shader->type, SceneManager::getInstance()->lightMap[0]->type);
	}

}

void SceneObject::Update(GLfloat deltaTime)
{
	Matrix translation, scal;
	Matrix rotx, roty, rotz, rot;

	translation.SetTranslation(position);
	scal.SetScale(scale);

	rotx.SetRotationX(rotation.x);
	roty.SetRotationY(rotation.y);
	rotz.SetRotationZ(rotation.z);
	rot = rotx * roty * rotz;

	matrix = scal * rot * translation;

	if (typeTrajectory == T_LINEAR) {

		endPoint = pointPosition[inc + sens];
		if ((endPoint - startPoint).Dot(endPoint - position) > 0) {
			position += (endPoint - startPoint).Normalize() * deltaTime * speed;
		}
		else if (iterationTrajectory != 0) {
			if (directionTrajectory == "normal") {
				position = pointPosition[0];
				if (iterationTrajectory != -1) {
					iterationTrajectory -= 1;
				}
			}
			else if (directionTrajectory == "alternate") {
				sens = -sens;
				if (sens == -1) {
					position = pointPosition[1];
					startPoint = pointPosition[1];
					inc = 1;
				}
				else if (sens == 1) {
					startPoint = pointPosition[0];
					position = pointPosition[0];
					inc = 0;
				}
				rotation.y += PI;
				if (iterationTrajectory != -1) {
					iterationTrajectory -= 1;
				}
			}
		}
	}
	else if (typeTrajectory == T_LINE_STRIP || typeTrajectory == T_LINE_LOOP) {


		if (pointSize > 0) {

			endPoint = pointPosition[inc + sens];

			if ((endPoint - startPoint).Dot(endPoint - position) > 0) {
				position += (endPoint - startPoint).Normalize() * deltaTime * speed;
			}
			else {
				pointSize -= 1;
				position = endPoint;
				if (pointSize > 0) {
					startPoint = endPoint;
					inc += sens;
				}
			}

		}
		if ((pointSize == 0) && (iterationTrajectory != 0)) {

			if (directionTrajectory == "normal") {
				inc = 0;
				pointSize = pointPosition.size() - 1;
				startPoint = pointPosition[inc];
				position = startPoint;
				if (iterationTrajectory != -1) {
					iterationTrajectory -= 1;
				}
			}
			else if (directionTrajectory == "alternate") {
				if (sens == 1) {
					inc = pointPosition.size() - 1;
				}
				else if (sens == -1) {
					inc = 0;
				}
				sens = -sens;

				pointSize = pointPosition.size() - 1;
				startPoint = pointPosition[inc];
				if (iterationTrajectory != -1) {
					iterationTrajectory -= 1;
				}
				rotation.y += PI;
			}
		}
	}
	else if (typeTrajectory == T_CIRCLE) {
		if (angle > (2 * PI)) {
			angle = angle - (2 * PI);
		}
		angle += speed * deltaTime;

		Matrix scaleM, RinitX, RinitY, RinitZ, Traza, Ry, RplaneX, RplaneZ, T;


		scaleM.SetScale(scale);
		RinitX.SetRotationX(rotation.x);
		RinitY.SetRotationY(rotation.y);
		RinitZ.SetRotationZ(rotation.z);
		Traza.SetTranslation(0, 0, radius);

		Ry.SetRotationY(angle);

		RplaneX.SetRotationX(rotationPlane.x);
		RplaneZ.SetRotationZ(rotationPlane.z);

		T.SetTranslation(center);

		matrix = scaleM * RinitX * RinitY * RinitZ * Traza * Ry * RplaneX * RplaneZ * T;

		int test = 1;

	}

	/*AABB*/
	vertexData = new Vertex[model->nrVertex];
	for (int i = 0; i < model->nrVertex; i++) {
		Vector4 vertexDataUpdate = Vector4(model->vertexData[i].position.x, model->vertexData[i].position.y, model->vertexData[i].position.z, 1.0) * matrix;
		vertexData[i].position = Vector3(vertexDataUpdate.x, vertexDataUpdate.y, vertexDataUpdate.z);
	}

	Min.x = vertexData[0].position.x, Min.y = vertexData[0].position.y, Min.z = vertexData[0].position.z;
	Max.x = vertexData[0].position.x, Max.y = vertexData[0].position.y, Max.z = vertexData[0].position.z;

	for (int i = 1; i < model->nrVertex; i++) {
		if (vertexData[i].position.x < Min.x) {
			Min.x = vertexData[i].position.x;
		}
		if (vertexData[i].position.y < Min.y) {
			Min.y = vertexData[i].position.y;
		}
		if (vertexData[i].position.z < Min.z) {
			Min.z = vertexData[i].position.z;
		}

		if (vertexData[i].position.x > Max.x) {
			Max.x = vertexData[i].position.x;
		}
		if (vertexData[i].position.y > Max.y) {
			Max.y = vertexData[i].position.y;
		}
		if (vertexData[i].position.z > Max.z) {
			Max.z = vertexData[i].position.z;
		}
	}
	delete[] vertexData;


	AABB[0] = Vector3((GLfloat)Min.x, (GLfloat)Min.y, (GLfloat)Min.z);
	AABB[1] = Vector3((GLfloat)Max.x, (GLfloat)Max.y, (GLfloat)Max.z);
}

