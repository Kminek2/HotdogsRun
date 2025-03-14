#pragma once

#include "ObjectScript.h"
#include "GameObject.h"

class PowerUp : public ObjectScript {
	virtual void OnActivate() = 0;

	void Init() override;
};