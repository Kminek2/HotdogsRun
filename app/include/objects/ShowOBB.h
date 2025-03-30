#pragma once

#include "ObjectScript.h"
#include "GameObject.h"
#include "DebugPoints.h"
#include "Collisions.h"

class ShowOBB :
	public ObjectScript
{
public:
	void Init() override;
	void Update() override;
	void LateUpdate() override;
	void OnDestroy() override;


	ShowOBB* copy() override { return new ShowOBB(*this); }
};