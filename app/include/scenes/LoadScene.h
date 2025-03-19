#pragma once

#include "SceeneScript.h"
#include "QuickCamera.h"
#include <utility>
#include <vector>

class LoadScene :
    public SceeneScript
{
private:
    Text* amountOfLoaded;
    unsigned count;
    GameObject* loadingCircle;

public:
    static std::vector<std::pair<std::string, SceeneScript*>> scenes;
    static std::vector<std::string> preLoadModels;
    static std::vector<std::string> preLoadSprites;

    std::shared_ptr<Scene> Init() override;
    void Update() override;
    void UnLoad() override;
};