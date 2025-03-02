#include "SoundEngine.h"
#include "Camera.h"
#include "Transform.h"

SoundEngine::SoundEngine()
{
    Initialize();
}

SoundEngine::SoundEngine(const SoundEngine& soundEngine)
{
}

SoundEngine::~SoundEngine()
{
    Shutdown();
}

void SoundEngine::UpdatePos()
{
    float position[3];

    // Set the initial position of the listener.
    position[0] = Camera::main->cameraTransform->position.x;
    position[1] = Camera::main->cameraTransform->position.z;
    position[2] = Camera::main->cameraTransform->position.y;

    // Clear any previous unaddressed error codes.
    alGetError();

    // Set the listener position.
    alListenerfv(AL_POSITION, position);
}

bool SoundEngine::Initialize()
{
    ALCdevice* device;
    ALCcontext* context;
    float position[3];
    bool result;

    // Select the default audio device on the system.
    device = alcOpenDevice(0);
    if (!device)
    {
        return false;
    }

    // Create a context on the device.
    context = alcCreateContext(device, 0);

    // Open the context by setting it as the current context on the device.
    alcMakeContextCurrent(context);

    // Set the initial position of the listener.
    position[0] = 0.0f;
    position[1] = 0.0f;
    position[2] = 0.0f;

    // Clear any previous unaddressed error codes.
    alGetError();

    // Set the listener position.
    alListenerfv(AL_POSITION, position);
    if (alGetError() != AL_NO_ERROR)
    {
        return false;
    }

    return true;
}

void SoundEngine::Shutdown()
{
    ALCdevice* device;
    ALCcontext* context;

    // Retrieve the current context and device.
    context = alcGetCurrentContext();
    device = alcGetContextsDevice(context);

    // Unset our audio context as the current one.
    alcMakeContextCurrent(0);

    // Destroy our audio context.
    alcDestroyContext(context);

    // Close our audio device.
    alcCloseDevice(device);

    return;
}
