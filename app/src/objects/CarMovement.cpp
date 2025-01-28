#include "objects/CarMovement.h"
#include "Input.h"
#include "AppTime.h"
#include <iostream>

CarMovement::CarMovement(float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, bool expertMode) :
	carWeight(carWeight), breaksStrength(breaksStrength), maxSpeed(maxSpeed), minSpeed(minSpeed), accelFront(accelFront), accelBack(accelBack), expertMode(expertMode) {
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
	gameObject->transform->Rotate(glm::vec3(0.0f, 0.0f, axleAngle * (actSpeed / maxSpeed) * Time::deltaTime * 3.0f));
	gameObject->transform->Translate((actSpeed*Time::deltaTime)*forces);
	//std::cout << actSpeed << '\n';
	std::cout << forces.x << ' ' << forces.y << ' ' << forces.z << '\n';
}

void CarMovement::OnDestroy() {

}

void CarMovement::handleGas() {
	if (maxSpeed == 0) return;
	if (Input::getKeyPressed(GLFW_KEY_W)) {
		if (actSpeed < -10.0f) {
			std::cout << "GEARBOX DESTROYED!" << '\n';
			//destroyGearbox();
			return;
		}
		float speedPr = actSpeed / maxSpeed;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		actSpeed += Time::deltaTime * accelFront * (-1.6f * powf(speedPr, 2) + 1.6f); //y = -1.6x^2+1.6
		actSpeed = std::min(actSpeed, maxSpeed);
	}
	if (Input::getKeyPressed(GLFW_KEY_S)) {
		if (actSpeed > 10.0f) {
			std::cout << "GEARBOX DESTROYED!" << '\n';
			//destroyGearbox();
			return;
		}
		float speedPr = actSpeed / minSpeed;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		actSpeed -= Time::deltaTime * accelBack * (-1.6f * powf(speedPr, 2) + 1.6f); //y = -1.6x^2 + 1.6
		actSpeed = std::max(actSpeed, minSpeed);
	}
}

void CarMovement::handleBreaks() {
	if (Input::getKeyPressed(GLFW_KEY_SPACE)) {
		if (actSpeed > 0) {
			actSpeed -= carWeight * breaksStrength * 200.0f * Time::deltaTime;
			actSpeed = std::max(actSpeed, 0.0f);
		}
		else {
			actSpeed += carWeight * breaksStrength * 200.0f * Time::deltaTime;
			actSpeed = std::min(actSpeed, 0.0f);
		}
	}
}

void CarMovement::handleEngBreak() {
	if (actSpeed > 0) {
		actSpeed -= carWeight * 10.0f * Time::deltaTime;
		actSpeed = std::max(actSpeed, 0.0f);
	} else {
		actSpeed += carWeight * 10.0f * Time::deltaTime;
		actSpeed = std::min(actSpeed, 0.0f);
	}
}

void CarMovement::handleSteeringWheel() {
	bool left = Input::getKeyPressed(GLFW_KEY_A);
	bool right = Input::getKeyPressed(GLFW_KEY_D);
	if (expertMode) {

	} else {
		if (left == right)
			axleAngle = 0.0f;
		else if (left)
			axleAngle = 30.0f;
		else if (right)
			axleAngle = -30.0f;
	}
}

//Please don't read this part. It would make you feel a lot of pain.
void CarMovement::handleForces() {
	bool yNeg = (forces.y < 0 ? true : false);

	if (axleAngle < 0.0f+EPSILON && axleAngle > 0.0f-EPSILON) {
		forces.y -= (yNeg ? -0.4 * Time::deltaTime : 0.4 * Time::deltaTime);
		forces.y = (yNeg ? std::min(forces.y, 0.0f) : std::max(forces.y, 0.0f));
		if (actSpeed > 0) {
			actSpeed -= carWeight * 20.0f * Time::deltaTime;
			actSpeed = std::max(actSpeed, 0.0f);
		} else {
			actSpeed += carWeight * 20.0f * Time::deltaTime;
			actSpeed = std::min(actSpeed, 0.0f);
		}
	}
	
	if (abs(forces.y * (axleAngle/15.0f)) < 0.5f) {
		if (axleAngle < 0.0f-EPSILON) {
			forces.y += 0.1*(axleAngle/45.0f)*Time::deltaTime;
		} else if (axleAngle > 0.0f+EPSILON) {
			forces.y -= 0.1f*(axleAngle/45.0f)*Time::deltaTime;
		}
	}
}