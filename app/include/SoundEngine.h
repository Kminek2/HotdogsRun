#pragma once
#include <AL/al.h>
#include <AL/alc.h>
class SoundEngine
{
public:
	SoundEngine();
	SoundEngine(const SoundEngine& soundEngine);
	~SoundEngine();

	void UpdatePos();
private:
	bool Initialize();
	void Shutdown();
};