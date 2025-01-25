#pragma once
#include "SceeneScript.h"
#include <utility>
#include <vector>
class DebugScene :
    public SceeneScript
{
private:
    std::vector<GameObject*> objs;
public:
    std::shared_ptr<Scene> Init() override;

    void Update() override;

    void UnLoad() override;
};

