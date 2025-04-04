#pragma once
#include "SoundEngine.h"

#include <list>
#include <string>
#include <fstream>
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
    AudioSource2d(std::string filename, float volume, unsigned short maxNumOfChanels = 2);
    AudioSource2d(const AudioSource2d&);
    ~AudioSource2d();

    static void DeleteAllSources2d();

    bool PlayTrack(bool looping);
    bool StopTrack();

    bool setVolume(float volume);
protected:
    static bool deleteingAll;
    bool LoadTrack(const char* filename, float volume, unsigned short maxChanelNum);

    bool LoadWaveFile(const char* filename, unsigned short maxChanelNum);
    void ReleaseWaveFile();

    void ReleaseTrack();

    static std::list<AudioSource2d*> createdAudio;
    std::list<AudioSource2d*>::iterator i;

    unsigned int m_audioBufferId, m_audioSourceId;
    unsigned char* m_waveData;
    unsigned int m_waveSize;
};

