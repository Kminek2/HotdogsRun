#pragma once

#include "SceeneScript.h"

class TutorialScene :
    public SceeneScript
{
private:
public:
    std::shared_ptr<Scene> Init() override;
    void Update() override;
    void UnLoad() override;
};