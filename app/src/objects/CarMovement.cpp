#include "objects/CarMovement.h"
#include "AppTime.h"

#include <iostream>

const CarMovement::actions CarMovement::clearedActions = {false, false, false, false, false};
const std::array<CarMovement::road_type_data,5> CarMovement::surfaces_data = {
	CarMovement::road_type_data(0.7f, 2.0f, 2.0f, 0.7f, 0.2f, 0.7f), // NONE
	CarMovement::road_type_data(1.0f, 1.5f, 1.6f, 0.9f, 1.0f, 0.9f), // ASPHALT
	CarMovement::road_type_data(0.8f, 2.0f, 2.0f, 0.7f, 0.7f, 0.8f), //	GRAVEL
	CarMovement::road_type_data(0.7f, 1.0f, 0.5f, 0.6f, 1.0f, 0.2f), // ICE
	CarMovement::road_type_data(0.2f, 0.5f, 0.2f, 1.1f, 0.3f, 0.4f)  // OIL
};
const float CarMovement::nitro_duration = 1.0f;
bool CarMovement::disabled_inputs = false;

CarMovement::CarMovement(float carWeight, float breaksStrength, float maxSpeed, float minSpeed, float accelFront, float accelBack, float gripToSpeedMult, bool expertMode, float multiplier, glm::vec3 nitro_trail_offset) :
	carWeight(carWeight), breaksStrength(breaksStrength), maxSpeed(maxSpeed), minSpeed(minSpeed), accelFront(accelFront), accelBack(accelBack), expertMode(expertMode), multiplier(multiplier), nitro_trail_offset(nitro_trail_offset), gripToSpeed(gripToSpeedMult) {
	forces = { 1, 0, 0 };
	actSpeed = 0.0f;
	axleAngle = 0.0f;
	road_type = 0;
	nitros_available = 3;
	nitro_timer = 0.0f;
	nitro_trail = nullptr;
	gripMult = glm::vec3(1);
	crashsound_timer = 0.0f;
	gassound_timer = 0.0f;
	if (gameObject == nullptr)
		gameObject = new GameObject();
	crashsound_audio = new AudioSource3d(gameObject, "crash", static_cast<float>(Settings::read("volume").value_or(100))/100.0f);
	gassound_audio = new AudioSource3d(gameObject, "gas", static_cast<float>(Settings::read("volume").value_or(100))/100.0f);
}

void CarMovement::Init() {
	gameObject->cm = this;
}

void CarMovement::Update() {
	__multiplier = multiplier * _multiplier;
	__maxSpeed   = maxSpeed   * _maxSpeed  ;
	__accelFront = accelFront * _accelFront;
	__carWeight  = carWeight  * _carWeight ;

	handleAudio();

	handleBreaks();
	handleEngBreak();
	if (nitro_timer > 0)
		handleNitroAcc();
	else
		handleGas();
	handleSteeringWheel();
	handleForces();
	if (gameObject->transform->position.z < 0) {
		gameObject->transform->MoveTo(gameObject->transform->position * glm::vec3(1, 1, 0));
		if (downSpeed < -100 * Time::deltaTime)
			downSpeed = 0.3f * std::max(0.0f, std::abs(downSpeed));
		else
			downSpeed = 0.3f * std::max(0.0f, std::abs(downSpeed)- 1000 * Time::deltaTime);
	}

	else if (gameObject->transform->position.z > 0) {
		downSpeed += -__carWeight * __multiplier * 1000 * Time::deltaTime;
	}
	auto old_rot = gameObject->transform->rotation;
	auto old_pos = gameObject->transform->position;
	gameObject->transform->Rotate(glm::vec3(0.0f, 0.0f, (axleAngle * (actSpeed < 200.0f*__multiplier ? std::min(200.0f*__multiplier, actSpeed*4) : actSpeed) / (__maxSpeed*__multiplier)) * Time::deltaTime * 5.0f * surfaces_data[road_type].steering_multiplier));
	handleGrip();
	gameObject->transform->Move((actSpeed*gripMult*Time::deltaTime)*glm::length(forces) + downSpeed * glm::vec3(0, 0, Time::deltaTime));
	bool coll = false;
	road_type = 0;

	GameObject* collidedWith = nullptr;
	for (auto const& obj : GameObject::createdGameObject) {
		if (obj == gameObject || !Collisions::checkCollision(*gameObject, *obj))
			continue;

		if (obj->surface_type < 0) {
			coll = (obj->surface_type == ALWAYS_COLLIDE);
			if (coll)
				collidedWith = obj;
		} else
			road_type = std::max(road_type, obj->surface_type);
	}
	if (coll) {
		if (crashsound_timer == 0.0f) {
			crashsound_audio->PlayTrack(false);
			crashsound_timer = 0.5f;
		}
		gameObject->transform->MoveTo(old_pos);
		gameObject->transform->RotateTo(old_rot);
		actSpeed *= -1;
		forces.x = -forces.x;
		gripMult += collidedWith->transform->position - gameObject->transform->position;
		axleAngle = 0.0f;
		//actSpeed = std::min(actSpeed/2.5f, 20.0f * (actSpeed > 0 ? 1 : -1));
		downSpeed += std::abs((1 / (__carWeight * __multiplier)) * actSpeed) * 0.03f;
	}
	actActions = clearedActions;
}

void CarMovement::OnDestroy() {}

void CarMovement::handleGas() {
	if (forces.x != 1.0f) return;
	if (__maxSpeed == 0) return;
	if (actActions.forward) {
		if (gassound_timer == 0.0f) {
			gassound_timer = 1.6f;
			gassound_audio->PlayTrack(false);
		}

		float speedPr = actSpeed / __maxSpeed*__multiplier*surfaces_data[road_type].max_speed_multiplier;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		if (actSpeed < __maxSpeed*__multiplier*surfaces_data[road_type].max_speed_multiplier) {
			actSpeed += Time::deltaTime * __accelFront * surfaces_data[road_type].acc_multiplier * (-1.6f * powf(speedPr, 2) + 1.6f) * __multiplier; //y = -1.6x^2+1.6
			actSpeed = std::min(actSpeed, __maxSpeed*__multiplier*surfaces_data[road_type].max_speed_multiplier);
		}
	}
	if (actActions.backwards) {
		if (gassound_timer == 0.0f) {
			gassound_timer = 1.6f;
			gassound_audio->PlayTrack(false);
		}

		float speedPr = actSpeed / minSpeed*__multiplier;
		if (speedPr >= 1.0f) return;
		if (speedPr < 0.0f) speedPr = 0.0f;
		actSpeed -= Time::deltaTime * accelBack * surfaces_data[road_type].acc_multiplier * (-1.6f * powf(speedPr, 2) + 1.6f) * __multiplier * 3.0f; //y = -1.6x^2 + 1.6
		actSpeed = std::max(actSpeed, minSpeed * __multiplier);
	}
}

void CarMovement::handleBreaks() {
	if (actActions.hand_break) {
		if (actSpeed > 0) {
			actSpeed -= surfaces_data[road_type].break_multiplier * __carWeight * breaksStrength * 200.0f * Time::deltaTime * __multiplier;
			actSpeed = std::max(actSpeed, 0.0f);
		}
		else {
			actSpeed += surfaces_data[road_type].break_multiplier * __carWeight * breaksStrength * 200.0f * Time::deltaTime * __multiplier;
			actSpeed = std::min(actSpeed, 0.0f);
		}
	}
}

void CarMovement::handleEngBreak() {
	if (actSpeed > 0) {
		actSpeed -= surfaces_data[road_type].eng_break_multiplier * __carWeight * 10.0f * Time::deltaTime * __multiplier * std::max(((actSpeed - __maxSpeed*__multiplier*surfaces_data[road_type].max_speed_multiplier)*3.0f), 1.0f);
		actSpeed = std::max(actSpeed, 0.0f);
	} else {
		actSpeed += surfaces_data[road_type].eng_break_multiplier * __carWeight * 10.0f * Time::deltaTime * __multiplier;
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
	
	bool yNeg = (forces.y < 0);
	
	if (axleAngle < 0.0f+EPSILON && axleAngle > 0.0f-EPSILON) {
		forces.y -= (yNeg ? -0.4 * Time::deltaTime : 0.4 * Time::deltaTime);
		forces.y = (yNeg ? std::min(forces.y, 0.0f) : std::max(forces.y, 0.0f));
	} else if (abs(forces.y * (axleAngle/15.0f)) < 0.5f) {
		if (axleAngle < 0.0f-EPSILON) {
			forces.y += 0.1*(axleAngle/45.0f)*Time::deltaTime;
		} else if (axleAngle > 0.0f+EPSILON) {
			forces.y -= 0.1f*(axleAngle/45.0f)*Time::deltaTime;
		}
		if (actSpeed > 40.0f * __multiplier) {
			actSpeed -= __carWeight * Time::deltaTime * __accelFront * __multiplier;
			actSpeed = std::max(actSpeed, 0.0f);
		} else if (actSpeed < -40.0f * __multiplier) {
			actSpeed += __carWeight * Time::deltaTime * accelBack * __multiplier;
			actSpeed = std::min(actSpeed, 0.0f);
		}
	}
}

void CarMovement::goForward() {
	if (disabled_inputs)
		return;
	actActions.forward = true;
}

void CarMovement::goBackwards() {
	if (disabled_inputs)
		return;
	actActions.backwards = true;
}

void CarMovement::makeLeftTurn() {
	if (disabled_inputs)
		return;
	actActions.left_turn = true;
}

void CarMovement::makeRightTurn() {
	if (disabled_inputs)
		return;
	actActions.right_turn = true;
}

void CarMovement::useHandBreak() {
	if (disabled_inputs)
		return;
	actActions.hand_break = true;
}

CarMovement::road_type_data::road_type_data(float acc_multiplier, float eng_break_multiplier, float break_multiplier, float steering_multiplier, float max_speed_multiplier, float grip) {
	this->acc_multiplier = acc_multiplier;
	this->eng_break_multiplier = eng_break_multiplier;
	this->break_multiplier = break_multiplier;
	this->steering_multiplier = steering_multiplier;
	this->max_speed_multiplier = max_speed_multiplier;
	this->grip = grip;
}

void CarMovement::useNitro() {
	if (disabled_inputs || nitros_available < 1)
		return;
	--nitros_available;
	before_nitro_mem = actSpeed;
	nitro_timer = nitro_duration;
	nitro_trail = new GameObject("nitroOgien");
	nitro_trail->AddScript(new LockPosition(gameObject->transform, nitro_trail_offset))->AddScript(new LockRotation(gameObject->transform));
}

void CarMovement::addNitros(int count)
{
	nitros_available += count;
}

void CarMovement::handleNitroAcc() {
	nitro_timer -= Time::deltaTime;
	std::cout << nitro_timer << '\n';
	nitro_timer = std::max(0.0f, nitro_timer);
	actSpeed += Time::deltaTime * __accelFront * surfaces_data[road_type].acc_multiplier * 0.1f * __multiplier * std::max(before_nitro_mem, 50.0f);
	if (nitro_timer == 0.0f) {
		actSpeed = before_nitro_mem;
		if (nitro_trail) {
			delete nitro_trail;
			nitro_trail = nullptr;
		}
	}
}

void CarMovement::handleGrip() {
	gripMult = glm::normalize(surfaces_data[road_type].grip * gameObject->transform->front * Time::deltaTime * (gameObject->transform->position.z > 0 ? 0.01f : 1.0f) + (1 - surfaces_data[road_type].grip) * gripMult * std::abs(actSpeed / (gripToSpeed * __maxSpeed)) * (std::pow(axleAngle * (actSpeed / (__maxSpeed * __multiplier)) / 30, 2.0f) + 0.5f));
}

void CarMovement::handleAudio() {
	crashsound_timer -= Time::deltaTime;
	gassound_timer -= Time::deltaTime;
	crashsound_timer = std::max(0.0f, crashsound_timer);
	gassound_timer = std::max(0.0f, gassound_timer);
}