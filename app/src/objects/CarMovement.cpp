#include "objects/CarMovement.h"
#include "AppTime.h"
#include <iostream>

const CarMovement::actions CarMovement::clearedActions = {false, false, false, false, false};

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
	auto old_rot = gameObject->transform->rotation;
	auto old_pos = gameObject->transform->position;
	gameObject->transform->Rotate(glm::vec3(0.0f, 0.0f, (axleAngle * (actSpeed < 200.0f*multiplier ? std::min(200.0f*multiplier, actSpeed*4) : actSpeed) / (maxSpeed*multiplier)) * Time::deltaTime * 3.0f));
	gameObject->transform->Translate((actSpeed*Time::deltaTime)*forces);
	bool coll = false;
	for (auto const& obj : GameObject::createdGameObject) {
		if (obj == gameObject)
			continue;

		if (Collisions::checkCollision(*gameObject, *obj)) {
			coll = true;
			break;
		}
	}
	if (coll) {
		gameObject->transform->MoveTo(old_pos);
		gameObject->transform->RotateTo(old_rot);
		forces.x = -forces.x;
		axleAngle = 0.0f;
		if (actSpeed > 0) {
			actSpeed = std::min(actSpeed/2.5f, 20.0f);
		} else {
			actSpeed = std::max(actSpeed/2.5f, -20.0f);
		}
	}
	actActions = clearedActions;
}

void CarMovement::OnDestroy() {

}

void CarMovement::handleGas() {
	if (forces.x != 1.0f) return;
	if (maxSpeed == 0) return;
	if (actActions.forward) {
		float speedPr = actSpeed / maxSpeed*multiplier;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		actSpeed += Time::deltaTime * accelFront * (-1.6f * powf(speedPr, 2) + 1.6f) * multiplier; //y = -1.6x^2+1.6
		actSpeed = std::min(actSpeed, maxSpeed*multiplier);
	}
	if (actActions.backwards) {
		float speedPr = actSpeed / minSpeed*multiplier;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		actSpeed -= Time::deltaTime * accelBack * (-1.6f * powf(speedPr, 2) + 1.6f) * multiplier; //y = -1.6x^2 + 1.6
		actSpeed = std::max(actSpeed, minSpeed * multiplier);
	}
}

void CarMovement::handleBreaks() {
	if (actActions.hand_break) {
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
	if (forces.x != 1.0)
		return;

	if (expertMode) {
		return;
	}

	if (actActions.left_turn == actActions.right_turn) {
		if (axleAngle > 0) {
			axleAngle -= 100.0f * Time::deltaTime;
			axleAngle = std::max(axleAngle, 0.0f);
		} else {
			axleAngle += 100.0f * Time::deltaTime;
			axleAngle = std::min(axleAngle, 0.0f);
		}
	} else if (actActions.left_turn) {
		axleAngle += 100.0f * Time::deltaTime;
		axleAngle = std::min(axleAngle, 30.0f);
	} else if (actActions.right_turn) {
		axleAngle -= 100.0f * Time::deltaTime;
		axleAngle = std::max(axleAngle, -30.0f);
	}
}

//Please don't read this part. It would make you feel a lot of pain.
void CarMovement::handleForces() {
	if (forces.x < 1.0f) {
		forces.x += 0.7f*Time::deltaTime;
		forces.x = std::min(forces.x, 1.0f);
		if (forces.x > 0.0f) {
			forces.x = 1.0f;
			actSpeed = 0.0f;
		}
	}
	
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
	//std::cout << forces.x << '\n';
}

void CarMovement::goForward() {
	actActions.forward = true;
}

void CarMovement::goBackwards() {
	actActions.backwards = true;
}

void CarMovement::makeLeftTurn() {
	actActions.left_turn = true;
}

void CarMovement::makeRightTurn() {
	actActions.right_turn = true;
}

void CarMovement::useHandBreak() {
	actActions.hand_break = true;
}