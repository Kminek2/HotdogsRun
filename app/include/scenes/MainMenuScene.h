#pragma once
#include "SceeneScript.h"
#include "QuickCamera.h"
#include <utility>
#include <vector>
class MainMenuScene :
    public SceeneScript
{
private:
    QuickCamera* qc;
    std::vector<GameObject*> objs;
    static bool first_load;
    std::vector<Text*> menu_options;
public:
    static std::vector<std::pair<std::string, SceeneScript*>> scenes;
    
    std::shared_ptr<Scene> Init() override;
    
    void Update() override;
    
    void UnLoad() override;
    
    void ShowMenu();
};