#pragma once

#include "AudioSource2d.h"
#include "ObjectScript.h"
#include "GameObject.h"

class Bomb : public ObjectScript {
public:
	void Init() override;
	void Update() override;
	void OnDestroy() override;

	Bomb* copy() override { return new Bomb(*this); }
	static std::vector<GameObject*> cars;

private:
	float timer;
	AudioSource3d *explosion_sound;
	GameObject *sound_obj;
};