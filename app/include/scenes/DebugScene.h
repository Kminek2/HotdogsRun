#pragma once
#include "SceeneScript.h"
#include <utility>
#include <vector>
#include "QuickCamera.h"
class DebugScene :
    public SceeneScript
{
private:
    std::vector<GameObject*> objs;
    QuickCamera* qc;
public:
    std::shared_ptr<Scene> Init() override;

    void Update() override;

    void UnLoad() override;
};

