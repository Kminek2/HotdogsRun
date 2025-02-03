#include "QuickCamera.h"

void qc::HandleRotate(float sx, float sy) { Camera::main->RotateCamera(Input::mouseOff.x * sx, Input::mouseOff.y * sy); }
void qc::HandleRotate(float s) { qc::HandleRotate(s, s); }

void qc::HandleMove(float s, std::array<uint16_t, 7> mappings) {
	if (Input::getKeyPressed(mappings[6])) s *= 2; // sprint

	if (Input::getKeyPressed(mappings[0])) Camera::main->MoveCamera(Time::deltaTime *  s); // front
	if (Input::getKeyPressed(mappings[1])) Camera::main->MoveCamera(Time::deltaTime * -s); // back
	
	if (Input::getKeyPressed(mappings[2])) { // left
		Camera::main->RotateCamera(-90, 0);
		Camera::main->MoveCamera(Time::deltaTime *  s);
		Camera::main->RotateCamera(90, 0);
	}
	if (Input::getKeyPressed(mappings[3])) { // right
		Camera::main->RotateCamera(-90, 0);
		Camera::main->MoveCamera(Time::deltaTime * -s);
		Camera::main->RotateCamera(90, 0);
	}

	if (Input::getKeyPressed(mappings[4])) { // up
		Camera::main->RotateCamera(0, -90);
		Camera::main->MoveCamera(Time::deltaTime *  s);
		Camera::main->RotateCamera(0, 90);
	}
	if (Input::getKeyPressed(mappings[5])) { // down
		Camera::main->RotateCamera(0, -90);
		Camera::main->MoveCamera(Time::deltaTime * -s);
		Camera::main->RotateCamera(0, 90);
	}
}
