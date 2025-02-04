#pragma once
#include "SceeneScript.h"
#include <utility>
#include <vector>
class LoadScene :
    public SceeneScript
{
private:
    GameObject* obj;
    GameObject* obj2;
    UiObject* uiObj;
public:

    static std::vector<std::pair<std::string, SceeneScript*>> scenes;

    std::shared_ptr<Scene> Init() override;

    void Update() override;

    void UnLoad() override;
};

