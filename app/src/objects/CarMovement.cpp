#include "objects/CarMovement.h"
#include "Input.h"
#include "AppTime.h"
#include <iostream>

CarMovement::CarMovement(float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, bool expertMode, float multiplier) :
	carWeight(carWeight), breaksStrength(breaksStrength), maxSpeed(maxSpeed), minSpeed(minSpeed), accelFront(accelFront), accelBack(accelBack), expertMode(expertMode), multiplier(multiplier) {
	forces = { 1, 0, 0 };
	actSpeed = 0.0f;
	axleAngle = 0.0f;
}

void CarMovement::Init() {

}

void CarMovement::Update() {
	handleBreaks();
	handleEngBreak();
	handleGas();
	handleSteeringWheel();
	handleForces();
	if (actSpeed > 10.0f*multiplier || actSpeed < -10.0f*multiplier)
		gameObject->transform->Rotate(glm::vec3(0.0f, 0.0f, (axleAngle * (std::max(actSpeed,200.0f*multiplier)) / (maxSpeed*multiplier)) * Time::deltaTime * 3.0f));
	gameObject->transform->Translate((actSpeed*Time::deltaTime)*forces);
}

void CarMovement::OnDestroy() {

}

void CarMovement::handleGas() {
	if (maxSpeed == 0) return;
	if (Input::getKeyPressed(GLFW_KEY_W)) {
		if (actSpeed < -40.0f*multiplier) {
			std::cout << "GEARBOX DESTROYED!" << '\n';
			//destroyGearbox();
			return;
		}
		float speedPr = actSpeed / maxSpeed*multiplier;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		actSpeed += Time::deltaTime * accelFront * (-1.6f * powf(speedPr, 2) + 1.6f) * multiplier; //y = -1.6x^2+1.6
		actSpeed = std::min(actSpeed, maxSpeed*multiplier);
	}
	if (Input::getKeyPressed(GLFW_KEY_S)) {
		if (actSpeed > 40.0f*multiplier) {
			std::cout << "GEARBOX DESTROYED!" << '\n';
			//destroyGearbox();
			return;
		}
		float speedPr = actSpeed / minSpeed*multiplier;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		actSpeed -= Time::deltaTime * accelBack * (-1.6f * powf(speedPr, 2) + 1.6f) * multiplier; //y = -1.6x^2 + 1.6
		actSpeed = std::max(actSpeed, minSpeed * multiplier);
	}
}

void CarMovement::handleBreaks() {
	if (Input::getKeyPressed(GLFW_KEY_SPACE)) {
		if (actSpeed > 0) {
			actSpeed -= carWeight * breaksStrength * 200.0f * Time::deltaTime * multiplier;
			actSpeed = std::max(actSpeed, 0.0f);
		}
		else {
			actSpeed += carWeight * breaksStrength * 200.0f * Time::deltaTime * multiplier;
			actSpeed = std::min(actSpeed, 0.0f);
		}
	}
}

void CarMovement::handleEngBreak() {
	if (actSpeed > 0) {
		actSpeed -= carWeight * 10.0f * Time::deltaTime * multiplier;
		actSpeed = std::max(actSpeed, 0.0f);
	} else {
		actSpeed += carWeight * 10.0f * Time::deltaTime * multiplier;
		actSpeed = std::min(actSpeed, 0.0f);
	}
}

void CarMovement::handleSteeringWheel() {
	bool left = Input::getKeyPressed(GLFW_KEY_A);
	bool right = Input::getKeyPressed(GLFW_KEY_D);
	if (expertMode) {
		return;
	}

	if (left == right) {
		if (axleAngle > 0) {
			axleAngle -= 100.0f * Time::deltaTime;
			axleAngle = std::max(axleAngle, 0.0f);
		} else {
			axleAngle += 100.0f * Time::deltaTime;
			axleAngle = std::min(axleAngle, 0.0f);
		}
	} else if (left) {
		axleAngle += 100.0f * Time::deltaTime;
		axleAngle = std::min(axleAngle, 30.0f);
	} else if (right) {
		axleAngle -= 100.0f * Time::deltaTime;
		axleAngle = std::max(axleAngle, -30.0f);
	}
}

//Please don't read this part. It would make you feel a lot of pain.
void CarMovement::handleForces() {
	bool yNeg = (forces.y < 0 ? true : false);

	if (axleAngle < 0.0f+EPSILON && axleAngle > 0.0f-EPSILON) {
		forces.y -= (yNeg ? -0.4 * Time::deltaTime : 0.4 * Time::deltaTime);
		forces.y = (yNeg ? std::min(forces.y, 0.0f) : std::max(forces.y, 0.0f));
	} else if (abs(forces.y * (axleAngle/15.0f)) < 0.5f) {
		if (axleAngle < 0.0f-EPSILON) {
			forces.y += 0.1*(axleAngle/45.0f)*Time::deltaTime;
		} else if (axleAngle > 0.0f+EPSILON) {
			forces.y -= 0.1f*(axleAngle/45.0f)*Time::deltaTime;
		}
		if (actSpeed > 40.0f * multiplier) {
			actSpeed -= carWeight * Time::deltaTime * accelFront * multiplier;
			actSpeed = std::max(actSpeed, 0.0f);
		} else if (actSpeed < -40.0f * multiplier) {
			actSpeed += carWeight * Time::deltaTime * accelBack * multiplier;
			actSpeed = std::min(actSpeed, 0.0f);
		}
	}
}