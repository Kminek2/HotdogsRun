#include "AudioSource3d.h"
#include "GameObject.h"

std::list<AudioSource3d*> AudioSource3d::created3dAudio;
bool AudioSource3d::deleteingAll3d = false;

AudioSource3d::AudioSource3d(GameObject* gameObject, std::string filename, float volume) : AudioSource2d(*this)
{
    transform = gameObject->transform;

    m_waveData = 0;
    if (!LoadTrack(("sounds/" + filename + ".wav").c_str(), volume, 1))
        throw std::runtime_error("CHeck these sounds: " + filename);

    created3dAudio.push_back(this);
    i = std::prev(created3dAudio.end());
}

AudioSource3d::AudioSource3d(const AudioSource3d& s) : AudioSource2d(s)
{
}

AudioSource3d::~AudioSource3d()
{
    StopTrack();

    ReleaseTrack();

    if (!deleteingAll3d)
        created3dAudio.erase(i);

    m_waveSize = 0;
}

void AudioSource3d::DeleteAllSources3d()
{
    deleteingAll3d = true;
    std::list<AudioSource3d*>::iterator i = created3dAudio.begin();
    while (i != created3dAudio.end())
    {
        delete (*i);
        i = created3dAudio.erase(i);
    }
    deleteingAll3d = false;
}

void AudioSource3d::UpdateAllPosition()
{
    std::list<AudioSource3d*>::iterator i = created3dAudio.begin();
    while (i != created3dAudio.end())
    {
        (*i)->UpdatePos();
        i = std::next(i);
    }
}

bool AudioSource3d::UpdatePos()
{
    float position[3];


    // Set the 3D position of the sound.
    position[0] = transform->position.x/10.0f;
    position[1] = transform->position.z/10.0f;
    position[2] = transform->position.y/10.0f;

    alSourcefv(m_audioSourceId, AL_POSITION, position);
    if (alGetError() != AL_NO_ERROR)
    {
        return false;
    }

    return true;

}
