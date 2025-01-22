#include "GameObject.h"

std::list<GameObject*> GameObject::createdGameObject;

GameObject::GameObject(std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	this->model = Model::Create(model);

	transform = new Transform(position, rotation, scale);

	createdGameObject.push_back(this);

	i = std::prev(createdGameObject.end());
}

GameObject::~GameObject()
{
	for (int i = 0; i < objectScripts.size(); i++)
	{
		objectScripts[i]->OnDestroy();
		delete objectScripts[i];
	}
	createdGameObject.erase(i);
	delete transform;
	delete model;
}

void GameObject::AddScript(ObjectScript* script)
{
	script->gameObject = this;
	objectScripts.push_back(script);
	script->Init();
}

void GameObject::DeleteAll()
{
	std::list<GameObject*>::iterator it = createdGameObject.begin();
	
	while (it != createdGameObject.end())
	{
		delete *it;
		it = std::next(it);
	}
}

void GameObject::UpdateAllObjectScripts()
{
	std::list<GameObject*>::iterator it = createdGameObject.begin();

	while (it != createdGameObject.end())
	{
		for (int i = 0; i < (*it)->objectScripts.size(); i++)
		{
			(*it)->objectScripts[i]->Update();
		}
		it = std::next(it);
	}
}

void GameObject::TransformTransformsToMemory()
{
	std::vector<glm::mat4> transforms;
	std::list<GameObject*>::iterator it = createdGameObject.begin();
	for (int i = 0; i < createdGameObject.size(); i++) {
		transforms.push_back((*it)->transform->modelTransform);
		it = std::next(it);
	}

	Transform::TransformToMemory(transforms);
}
