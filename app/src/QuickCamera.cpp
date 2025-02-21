#include "QuickCamera.h"

void QuickCamera::HandleRotate() { Camera::main->cameraTransform->Rotate(Input::mouseOff.x * srx, Input::mouseOff.y * sry); }

void QuickCamera::HandleMove() {
	glm::vec3 movement = glm::vec3(0);
	if (Input::getKeyPressed(mappings[6])) sm *= 2; // sprint

	if (Input::getKeyPressed(mappings[0])) movement += glm::vec3(Time::deltaTime * sm, 0, 0);  // front
	if (Input::getKeyPressed(mappings[1])) movement += glm::vec3(Time::deltaTime * -sm, 0, 0); // back
	
	if (Input::getKeyPressed(mappings[2])) movement += glm::vec3(0, Time::deltaTime * -sm, 0); // left
	if (Input::getKeyPressed(mappings[3])) movement += glm::vec3(0, Time::deltaTime * sm, 0);  // right

	if (Input::getKeyPressed(mappings[4])) movement += glm::vec3(0, 0, Time::deltaTime * sm);  // up
	if (Input::getKeyPressed(mappings[5])) movement += glm::vec3(0, 0, Time::deltaTime * -sm); // down

	Camera::main->cameraTransform->Translate(movement);
}

float QuickCamera::_srx(float srx) {
	if (srx == -1) return this->srx;
	return this->srx = srx;
}
float QuickCamera::_sry(float sry) {
	if (sry == -1) return this->sry;
	return this->sry = sry;
}
float QuickCamera::_sm(float sm) {
	if (sm == -1) return this->sm;
	return this->sm = sm;
}
bool QuickCamera::_sr(float sr) {
	if (sr == -1) return srx == sry;
	srx = sry = sr;
	return true;
}

__keybinds QuickCamera::_mappings(__keybinds mappings) {
	if (mappings == __keybinds({ 0,0,0,0,0,0,0 })) return this->mappings;
	return this->mappings = mappings;
}