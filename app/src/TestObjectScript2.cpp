#include "TestObjectScript2.h"
#include "Input.h"
#include "GameObject.h"

void TestObjectScript2::Init()
{
}

void TestObjectScript2::Update()
{
	if (Input::getKeyPressed(GLFW_KEY_E))
		gameObject->transform->Move(glm::vec3(0, 0, Time::deltaTime));
}

void TestObjectScript2::OnDestroy()
{
}
