#include "LightObject.h"

UniformBuffer<LightBufferStruct>* LightObject::lightBuffer;

void LightObject::Unload()
{
	delete lightBuffer;
}
