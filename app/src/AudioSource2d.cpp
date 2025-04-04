#include "AudioSource2d.h"
#include <fstream>
#include <exception>

std::list<AudioSource2d*> AudioSource2d::createdAudio;
bool AudioSource2d::deleteingAll = false;

AudioSource2d::AudioSource2d(std::string filename, float volume, unsigned short maxNumOfChanels)
{
	m_waveData = 0;
	if (!LoadTrack(("sounds/" + filename + ".wav").c_str(), volume, maxNumOfChanels))
		throw std::runtime_error("CHeck these sounds: " + filename);

	createdAudio.push_back(this);
	i = std::prev(createdAudio.end());
}

AudioSource2d::AudioSource2d(const AudioSource2d&)
{
	m_waveSize = 0;
	m_waveData = nullptr;
}

AudioSource2d::~AudioSource2d()
{
	if (m_waveSize != 0) {
		StopTrack();

		ReleaseTrack();

		if(!deleteingAll)
			createdAudio.erase(i);
	}
}

void AudioSource2d::DeleteAllSources2d()
{
	deleteingAll = true;
	std::list<AudioSource2d*>::iterator i = createdAudio.begin();
	while (i != createdAudio.end())
	{
		delete (*i);
		i = createdAudio.erase(i);
	}
	deleteingAll = false;
}

bool AudioSource2d::LoadTrack(const char* filename, float volume, unsigned short maxChanelNum)
{
	bool result;

	//Initialize the error state through a reset.
	alGetError();

	// Generate an audio buffer.
	alGenBuffers(1, &m_audioBufferId);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	// Load the wave file for the sound.
	result = LoadWaveFile(filename, maxChanelNum);
	if (!result)
	{
		return false;
	}

	// Copy the wav data into the audio buffer.
	if(maxChanelNum == 2)
		alBufferData(m_audioBufferId, AL_FORMAT_STEREO16, m_waveData, m_waveSize, 44100);
	else if(maxChanelNum == 1)
		alBufferData(m_audioBufferId, AL_FORMAT_MONO16, m_waveData, m_waveSize, 44100);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	// Release the wave data since it was copied into the OpenAL buffer.
	ReleaseWaveFile();

	// Generate an audio source.
	alGenSources(1, &m_audioSourceId);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	// Attach the buffer to the source.
	alSourcei(m_audioSourceId, AL_BUFFER, m_audioBufferId);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	// Set the volume to max.
	alSourcef(m_audioSourceId, AL_GAIN, volume);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	return true;
}

bool AudioSource2d::setVolume(float volume)
{
	alSourcef(m_audioSourceId, AL_GAIN, volume);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	return true;
}

void AudioSource2d::ReleaseTrack()
{
	// Release the audio source.
	alDeleteSources(1, &m_audioSourceId);

	// Release the audio buffer.
	alDeleteBuffers(1, &m_audioBufferId);

	return;
}

bool AudioSource2d::PlayTrack(bool looping)
{
	// Initialize the error state through a reset.
	alGetError();

	if (looping == true)
	{
		// Set it to looping.
		alSourcei(m_audioSourceId, AL_LOOPING, AL_TRUE);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}
	}
	else
	{
		// Set it to not looping.
		alSourcei(m_audioSourceId, AL_LOOPING, AL_FALSE);
		if (alGetError() != AL_NO_ERROR)
		{
			return false;
		}
	}

	// If looping is on then play the contents of the sound buffer in a loop, otherwise just play it once.
	alSourcePlay(m_audioSourceId);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	return true;
}

bool AudioSource2d::StopTrack()
{
	// Initialize the error state through a reset.
	alGetError();

	// Stop the sound from playing.
	alSourceStop(m_audioSourceId);
	if (alGetError() != AL_NO_ERROR)
	{
		return false;
	}

	return true;
}

bool AudioSource2d::LoadWaveFile(const char* filename, unsigned short maxChanelNum)
{
	RiffWaveHeaderType riffWaveFileHeader;
	SubChunkHeaderType subChunkHeader;
	FmtType fmtData;
	unsigned int count, seekSize;
	bool foundFormat, foundData;
	
	// Open the wave file for reading in binary.
	std::ifstream file(filename, std::ios::in | std::ios::binary);
	if (!file.is_open())
	{
		return false;
	}

	// Read in the riff wave file header.
	file.read(reinterpret_cast<char*>(& riffWaveFileHeader), sizeof(riffWaveFileHeader));
	count = file.gcount();
	if (count != sizeof(riffWaveFileHeader))
	{
		return false;
	}

	// Check that the chunk ID is the RIFF format.
	if ((riffWaveFileHeader.chunkId[0] != 'R') || (riffWaveFileHeader.chunkId[1] != 'I') || (riffWaveFileHeader.chunkId[2] != 'F') || (riffWaveFileHeader.chunkId[3] != 'F'))
	{
		return false;
	}

	// Check that the file format is the WAVE format.
	if ((riffWaveFileHeader.format[0] != 'W') || (riffWaveFileHeader.format[1] != 'A') || (riffWaveFileHeader.format[2] != 'V') || (riffWaveFileHeader.format[3] != 'E'))
	{
		return false;
	}

	// Read in the sub chunk headers until you find the format chunk.
	foundFormat = false;
	while (foundFormat == false)
	{
		// Read in the sub chunk header.
		file.read(reinterpret_cast<char*>(&subChunkHeader), sizeof(subChunkHeader));
		count = file.gcount();
		if (count != sizeof(subChunkHeader))
		{
			return false;
		}

		// Determine if it is the fmt header.  If not then move to the end of the chunk and read in the next one.
		if ((subChunkHeader.subChunkId[0] == 'f') && (subChunkHeader.subChunkId[1] == 'm') && (subChunkHeader.subChunkId[2] == 't') && (subChunkHeader.subChunkId[3] == ' '))
		{
			foundFormat = true;
		}
		else
		{
			file.seekg(subChunkHeader.subChunkSize, std::ios::cur);
		}
	}

	// Read in the format data.
	file.read(reinterpret_cast<char*>(&fmtData), sizeof(fmtData));
	count = file.gcount();
	if (count != sizeof(fmtData))
	{
		return false;
	}

	// Check that the audio format is WAVE_FORMAT_PCM (1).
	if (fmtData.audioFormat != 1)
	{
		return false;
	}

	// Check that the wave file was recorded in stereo format.
	if (fmtData.numChannels > maxChanelNum)
	{
		return false;
	}

	// Check that the wave file was recorded at a sample rate of 44.1 KHz.
	if (fmtData.sampleRate != 44100)
	{
		return false;
	}

	// Ensure that the wave file was recorded in 16 bit format.
	if (fmtData.bitsPerSample != 16)
	{
		return false;
	}





	// Seek up to the next sub chunk.
	seekSize = subChunkHeader.subChunkSize - 16;
	file.seekg(seekSize, std::ios::cur);

	// Read in the sub chunk headers until you find the data chunk.
	foundData = false;
	while (foundData == false)
	{
		file.read(reinterpret_cast<char*>(&subChunkHeader), sizeof(subChunkHeader));
		count = file.gcount();
		if (count != sizeof(subChunkHeader))
		{
			return false;
		}

		// Determine if it is the data header.  If not then move to the end of the chunk and read in the next one.
		if ((subChunkHeader.subChunkId[0] == 'd') && (subChunkHeader.subChunkId[1] == 'a') && (subChunkHeader.subChunkId[2] == 't') && (subChunkHeader.subChunkId[3] == 'a'))
		{
			foundData = true;
		}
		else
		{
			file.seekg(subChunkHeader.subChunkSize, std::ios::cur);
		}
	}

	// Store the size of the data chunk.
	m_waveSize = subChunkHeader.subChunkSize;

	// Create a temporary buffer to hold the wave file data.
	m_waveData = new unsigned char[m_waveSize];

	// Read in the wave file data into the newly created buffer.
	file.read(reinterpret_cast<char*>(m_waveData), m_waveSize);
	count = file.gcount();
	if (count != m_waveSize)
	{
		return false;
	}

	// Close the file once done reading.
	file.close();

	return true;
}

void AudioSource2d::ReleaseWaveFile()
{
	// Release the wave data.
	if (m_waveData)
	{
		delete[] m_waveData;
		m_waveData = 0;
	}

	return;
}


