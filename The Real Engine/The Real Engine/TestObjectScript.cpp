#include "TestObjectScript.h"
#include "GameObject.h"
#include "Time.h"

void TestObjectScript::Init()
{
}

void TestObjectScript::Update()
{
	gameObject->transform->Move(glm::vec3(0, 0, -Time::deltaTime));
}

void TestObjectScript::OnDestroy()
{
}
