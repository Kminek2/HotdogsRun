#pragma once

#include "SceeneScript.h"
#include "objects/MapManager.h"

#include "QuickCamera.h"
#include "objects/CameraLockScript.h"

class TutorialScene :
	public SceeneScript
{
private:
	bool first_music;
	float music_timer;
	AudioSource2d* music_first;
	AudioSource2d* music_cont;
public:
	std::shared_ptr<Scene> Init() override;
	void Update() override;
	void UnLoad() override;

	int camera_view = 0;
	int text_id = -1;
	
	static bool visited;

	float cp_color_timer = 5.0f;
	GameObject* checkpoint = nullptr;

	std::array<CameraLockScript*, 2> view_scripts;

	Sprite* main_text_bg = nullptr;
	std::vector<Text*> main_text;
	void setText(std::vector<std::string> txt);
};