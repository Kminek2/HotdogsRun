#include "UiObject.h"

UiObject::UiObject(std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) : GameObject(true, model, position, rotation, scale)
{

}

UiObject::~UiObject()
{
}
