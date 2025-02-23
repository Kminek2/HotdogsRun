#include "GameObject.h"

bool GameObject::deletingAll = false;
std::list<GameObject*> GameObject::createdGameObject;

GameObject::GameObject(int, std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	std::pair<Model*, uint32_t> newModel = Model::CreateUI(model);
	this->model = newModel.first;

	transform = new Transform(position, rotation, scale);

	createdGameObject.insert(std::next(createdGameObject.begin(), newModel.second), this);

	i = std::next(createdGameObject.begin(), newModel.second);
}

GameObject::GameObject(std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale) {
	std::pair<Model*, uint32_t> newModel = Model::Create(model);
	this->model = newModel.first;

	transform = new Transform(position, rotation, scale);

	createdGameObject.insert(std::next(createdGameObject.begin(), newModel.second), this);

	i = std::next(createdGameObject.begin(), newModel.second);
}

GameObject::~GameObject()
{
	for (int i = 0; i < objectScripts.size(); i++)
	{
		objectScripts[i]->OnDestroy();
		delete objectScripts[i];
	}
	if(!deletingAll)
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
	deletingAll = true;
	std::list<GameObject*>::iterator it = createdGameObject.begin();
	
	while (it != createdGameObject.end())
	{
		delete *it;
		it = createdGameObject.erase(it);
	}

	deletingAll = false;
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

void GameObject::LateUpdateAllObjectScripts()
{
	std::list<GameObject*>::iterator it = createdGameObject.begin();

	while (it != createdGameObject.end())
	{
		for (int i = 0; i < (*it)->objectScripts.size(); i++)
		{
			(*it)->objectScripts[i]->LateUpdate();
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

	if (transforms.size() == 0)
		return;

	Transform::TransformToMemory(transforms);
}

void GameObject::addOBB(OBB obb) {
	obbs.push_back(obb);
}