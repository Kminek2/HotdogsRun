#pragma once
#include "SceeneScript.h"
#include <utility>
#include <vector>
class DebugScene :
    public SceeneScript
{
private:
public:
    std::shared_ptr<Scene> Init() override;

    void Update() override;

    void UnLoad() override;
};

