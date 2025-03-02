#include "AudioSource3d.h"
#include "GameObject.h"

std::list<AudioSource3d*> AudioSource3d::createdAudio;

AudioSource3d::AudioSource3d(GameObject* gameObject, std::string filename, float volume) : AudioSource2d(filename, volume, 1)
{
    transform = gameObject->transform;

    createdAudio.push_back(this);
    i = std::prev(createdAudio.end());
}

AudioSource3d::AudioSource3d(const AudioSource3d& s) : AudioSource2d(s)
{
}

AudioSource3d::~AudioSource3d()
{
    StopTrack();

    ReleaseTrack();

    if (!deleteingAll)
        createdAudio.erase(i);
}

void AudioSource3d::DeleteAllSources3d()
{
    deleteingAll = true;
    std::list<AudioSource3d*>::iterator i = createdAudio.begin();
    while (i != createdAudio.end())
    {
        delete (*i);
        i = createdAudio.erase(i);
    }
    deleteingAll = false;
}

void AudioSource3d::UpdateAllPosition()
{
    std::list<AudioSource3d*>::iterator i = createdAudio.begin();
    while (i != createdAudio.end())
    {
        (*i)->UpdatePos();
        i = std::next(i);
    }
}

bool AudioSource3d::UpdatePos()
{
    float position[3];


    // Set the 3D position of the sound.
    position[0] = transform->position.x;
    position[1] = transform->position.z;
    position[2] = transform->position.y;

    alSourcefv(m_audioSourceId, AL_POSITION, position);
    if (alGetError() != AL_NO_ERROR)
    {
        return false;
    }

    return true;

}
