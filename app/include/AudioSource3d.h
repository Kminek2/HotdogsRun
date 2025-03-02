#pragma once
#include "AudioSource2d.h"
#include "Transform.h"

class GameObject;

class AudioSource3d :
    public AudioSource2d
{
public:
    AudioSource3d(GameObject* gameObject, std::string filename, float volume);
    AudioSource3d(const AudioSource3d&);
    ~AudioSource3d();

    Transform* transform;

private:
    static void DeleteAllSources3d();
    static void UpdateAllPosition();

    static std::list<AudioSource3d*> createdAudio;
    std::list<AudioSource3d*>::iterator i;
    bool UpdatePos();

    friend class Application;
};

