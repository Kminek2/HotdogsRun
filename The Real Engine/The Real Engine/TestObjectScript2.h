#pragma once
#include "ObjectScript.h"
class TestObjectScript2 :
    public ObjectScript
{
	void Init() override;
	void Update() override;
	void OnDestroy() override;
};

