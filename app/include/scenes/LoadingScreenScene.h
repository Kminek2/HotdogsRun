#pragma once
#include "SceeneScript.h"
class LoadingScreenScene :
    public SceeneScript
{
private:
    Text* text;
public:
    std::shared_ptr<Scene> Init() override;

    void Update() override;

    void UnLoad() override;
};

