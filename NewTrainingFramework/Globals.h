#pragma once

class Globals
{
public:
	static const int screenWidth = 960;
	static const int screenHeight = 720;

	static short int keyPressed;
	static short int effectPressed;

	enum Move {
		moveForward, moveBack, moveLeft, moveRight,
		moveUp, moveDown, rotateUp, rotateDown,
		rotateLeft, rotateRight, rotateQ, rotateE
	};

	enum Type {
		MOVE_CAMERA_NEGATIVE_X, MOVE_CAMERA_POSITIVE_X,
		MOVE_CAMERA_NEGATIVE_Y, MOVE_CAMERA_POSITIVE_Y,
		MOVE_CAMERA_NEGATIVE_Z, MOVE_CAMERA_POSITIVE_Z,

		ROTATE_CAMERA_NEGATIVE_X, ROTATE_CAMERA_POSITIVE_X,
		ROTATE_CAMERA_NEGATIVE_Y, ROTATE_CAMERA_POSITIVE_Y,
		ROTATE_CAMERA_NEGATIVE_Z, ROTATE_CAMERA_POSITIVE_Z,

		IS_WIRED_FALSE, IS_WIRED_TRUE
	};
};