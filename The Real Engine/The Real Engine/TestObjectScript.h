#pragma once
#include "ObjectScript.h"
class TestObjectScript :
    public ObjectScript
{
public:
    void Init() override;
    void Update() override;
    void OnDestroy() override;
};

