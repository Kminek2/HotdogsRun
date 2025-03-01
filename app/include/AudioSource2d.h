#pragma once
#include "SoundEngine.h"

#include <list>
#include <string>
#include <cstdio>
class AudioSource2d
{
private:
    struct RiffWaveHeaderType
    {
        char chunkId[4];
        unsigned int chunkSize;
        char format[4];
    };

    struct SubChunkHeaderType
    {
        char subChunkId[4];
        unsigned int subChunkSize;
    };

    struct FmtType
    {
        unsigned short audioFormat;
        unsigned short numChannels;
        unsigned int sampleRate;
        unsigned int bytesPerSecond;
        unsigned short blockAlign;
        unsigned short bitsPerSample;
    };
public:
    AudioSource2d(std::string filename, float volume);
    AudioSource2d(const AudioSource2d&);
    ~AudioSource2d();

    static void DeleteAllSources2d();

    bool PlayTrack(bool looping);
    bool StopTrack();
private:
    static bool deleteingAll;
    bool LoadTrack(const char* filename, float volume);

    bool LoadStereoWaveFile(const char* filename);
    void ReleaseWaveFile();

    void ReleaseTrack();

    static std::list<AudioSource2d*> createdAudio;
    std::list<AudioSource2d*>::iterator i;

    unsigned int m_audioBufferId, m_audioSourceId;
    unsigned char* m_waveData;
    unsigned int m_waveSize;
};

