#pragma once

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
};