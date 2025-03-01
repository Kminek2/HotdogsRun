#pragma once
#include <OpenAl/al.h>
#include <OpenAl/alc.h>
class SoundEngine
{
public:
	SoundEngine();
	SoundEngine(const SoundEngine& soundEngine);
	~SoundEngine();
private:
	bool Initialize();
	void Shutdown();
};