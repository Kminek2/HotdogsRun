#pragma once

#include "ObjectScript.h"
#include "GameObject.h"

class Bomba : public ObjectScript {
public:
	void Init() override;
	void Update() override;
	void OnDestroy() override;

	Bomba* copy() override { return new Bomba(*this); }

private:
	// ...
};