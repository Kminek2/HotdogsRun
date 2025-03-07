#include "GameObject.h"

bool GameObject::deletingAll = false;
std::list<GameObject*> GameObject::createdGameObject;

GameObject::GameObject(int, std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int surface_type) 
: surface_type(surface_type)
{
	std::pair<Model*, uint32_t> newModel = Model::CreateUI(model);
	this->model = newModel.first;

	transform = new Transform(position, rotation, scale);

	createdGameObject.insert(std::next(createdGameObject.begin(), newModel.second), this);

	i = std::next(createdGameObject.begin(), newModel.second);
}

GameObject::GameObject(std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int surface_type) 
: surface_type(surface_type)
{
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

std::string GameObject::GetModelName()
{
	return model->GetName();
}

std::array<glm::vec2, 3> GameObject::GetModelMaxDistVert()
{
	return model->GetMaxDistVert();
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

void GameObject::EarlyUpdateAllObjectScripts() {
	std::list<GameObject*>::iterator it = createdGameObject.begin();

	while (it != createdGameObject.end())
	{
		for (int i = 0; i < (*it)->objectScripts.size(); i++)
		{
			(*it)->objectScripts[i]->EarlyUpdate();
		}
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

void GameObject::AddDefaultOBB(glm::vec3 offset, bool two_dim)
{
	std::array<glm::vec2, 3> maxD = GetModelMaxDistVert();
	for (int i = 0; i < 3; i++)
		maxD[i] += glm::vec2(offset[i], -offset[i]);
	glm::vec3 c = { (maxD[0].x + maxD[0].y) / 2, (maxD[1].x + maxD[1].y) / 2, (maxD[2].x + maxD[2].y) / 2 };
	glm::vec3 s = { (maxD[0].x + abs(maxD[0].y)) / 2, (maxD[1].x + abs(maxD[1].y)) / 2, (maxD[2].x + abs(maxD[2].y)) / 2 };
	if (two_dim)
		s.z = 100000;

	addOBB(OBB(c, s));
}

void GameObject::AddColorChange(glm::vec3 from, glm::vec3 to)
{
	ColorChangeBuffer colorChange;
	colorChange.fromCol = from;
	colorChange.toCol = to;

	changeColor.push_back(colorChange);
}
