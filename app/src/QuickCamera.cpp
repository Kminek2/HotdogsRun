#include "QuickCamera.h"

void qc::HandleRotate(float sx, float sy) { Camera::main->cameraTransform->Rotate(Input::mouseOff.x * sx, Input::mouseOff.y * sy); }
void qc::HandleRotate(float s) { qc::HandleRotate(s, s); }

void qc::HandleMove(float s, std::array<uint16_t, 7> mappings) {
	glm::vec3 movement = glm::vec3(0);
	if (Input::getKeyPressed(mappings[6])) s *= 2; // sprint

	if (Input::getKeyPressed(mappings[0])) movement += glm::vec3(Time::deltaTime * s, 0, 0); // front
	if (Input::getKeyPressed(mappings[1])) movement += glm::vec3(Time::deltaTime * -s, 0, 0); // back
	
	if (Input::getKeyPressed(mappings[2])) { // left
		movement += glm::vec3(0, Time::deltaTime * -s, 0);
	}
	if (Input::getKeyPressed(mappings[3])) { // right
		movement += glm::vec3(0, Time::deltaTime * s, 0);
	}

	if (Input::getKeyPressed(mappings[4])) { // up
		movement += glm::vec3(0, 0, Time::deltaTime * s);
	}
	if (Input::getKeyPressed(mappings[5])) { // down
		movement += glm::vec3(0, 0, Time::deltaTime * -s);
	}

	Camera::main->cameraTransform->Translate(movement);
}
