#pragma once
#include "AudioSource2d.h"
#include "SceeneScript.h"
#include "UiObject.h"
#include "objects/AnimationManager.h"
#include "QuickCamera.h"
#include "objects/ColorPicker.hpp"
#include <utility>
#include <vector>
class MainMenuScene :
    public SceeneScript
{
private:
    CarMovement* cm;
    // QuickCamera* qc;
    AnimationManager* am;
    std::vector<GameObject*> objs;
    static bool first_load;
    static int model_choosen;
    std::vector<std::pair<Text*, Text*>> menu_options;
    std::vector<std::pair<Text*, Text*>> appearance_options;
    std::vector<std::pair<Sprite*, std::pair<Text*, Text*>>> maps_options;
    std::pair<Text*, Text*> tutorial_text;
    Sprite* settings_bg;
    std::vector<std::pair<std::pair<Text*, Text*>, std::pair<Text*, Text*>>> settings_options;

    Sprite* volume_bar;
    Sprite* bar_thumb;
    float volume_delay;

    UiObject* logo;
    GameObject* user_car;

    float music_timer;
    AudioSource2d* music_first;
    AudioSource2d* music_cont;
    AudioSource2d* sound_select;

    ColorPicker cp;
    
    int menu_choosen_option;
    void ShowMenu();
    void HideMenu();
    void UpdateMenu();
    void UpdateMenuHighlight();
    void first_animation();

    void ShowMaps();
    void UpdateMaps();
    void UpdateMapsHighlight();
    void HideMaps();

    void ShowAppearance();
    void UpdateAppearance();
    void HideAppearance();

    void ShowSettings();
    void UpdateSettings();
    void UpdateSettingsHighlight();
    void HideSettings();

    void to_maps_animation();
    void from_maps_animation();
    void to_appearance_animation();
    void from_appearance_animation();
public:
    static std::vector<std::pair<std::string, SceeneScript*>> scenes;
    
    std::shared_ptr<Scene> Init() override;
    
    void Update() override;
    
    void UnLoad() override;
};