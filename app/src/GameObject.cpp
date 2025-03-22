#include "GameObject.h"
#include "default_obb_data.h"

bool GameObject::deletingAll = false;
std::list<GameObject*> GameObject::createdGameObject;
Buffer<ThisColorChanges>* GameObject::colorChangesPrObject = nullptr;
UniformBuffer<ColorChangeStruct>* GameObject::allColorChanges = nullptr;
std::list<ColorChangeStruct> GameObject::changeColor;

GameObject::GameObject(int, std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int surface_type) 
: surface_type(surface_type)
{
	std::pair<Model*, uint32_t> newModel = Model::CreateUI(model);
	this->model = newModel.first;

	transform = new Transform(position, rotation, scale);

	createdGameObject.insert(std::next(createdGameObject.begin(), newModel.second), this);

	i = std::next(createdGameObject.begin(), newModel.second);
	colorChangesIndex = changeColor.end();
	amountOfColorChanges = 0;
}

uint32_t GameObject::SendColorData(uint32_t frame)
{
	std::list<GameObject*>::iterator it = createdGameObject.begin();
	std::vector<ThisColorChanges> colorChanges;
	for (int i = 0; i < createdGameObject.size(); i++) {
		ThisColorChanges colorChange;
		colorChange.amount = (*it)->amountOfColorChanges;
		colorChange.index = std::distance(changeColor.begin(), (*it)->colorChangesIndex);
		colorChanges.push_back(colorChange);

		it = std::next(it);
	}

	colorChangesPrObject->ClearBuffer();
	colorChangesPrObject->AddToBuffer(colorChanges);
	colorChangesPrObject->SendBufferToMemory();

	std::list<ColorChangeStruct>::iterator Cit = changeColor.begin();
	std::vector<ColorChangeStruct> colors;
	while (Cit != changeColor.end())
	{
		colors.push_back(*Cit);
		Cit = std::next(Cit);
	}

	allColorChanges->UpdateBuffer(frame, *colors.data(), colors.size() * sizeof(ColorChangeStruct));
	return colors.size() * sizeof(ColorChangeStruct);
}

GameObject::GameObject(std::string model, glm::vec3 position, glm::vec3 rotation, glm::vec3 scale, int surface_type)
: surface_type(surface_type)
{
	std::pair<Model*, uint32_t> newModel = Model::Create(model);
	this->model = newModel.first;

	transform = new Transform(position, rotation, scale);

	createdGameObject.insert(std::next(createdGameObject.begin(), newModel.second), this);

	i = std::next(createdGameObject.begin(), newModel.second);
	colorChangesIndex = changeColor.end();
	amountOfColorChanges = 0;
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

	auto it = colorChangesIndex;
	for (int i = 0; i < amountOfColorChanges; i++) {
		it = changeColor.erase(it);
	}
}

GameObject* GameObject::AddScript(ObjectScript* script)
{
	script->gameObject = this;
	objectScripts.push_back(script);
	script->Init();
	return this;
}

std::vector<ObjectScript*>& GameObject::GetObjectScripts() {
	return objectScripts;
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
	std::list<GameObject*>::iterator it2 = createdGameObject.begin();
	
	while (it2 != createdGameObject.end()) {
		for (auto& x : (*it2)->GetObjectScripts()) {
			delete x;
		}
		(*it2)->GetObjectScripts().clear();
		++it2;
	}

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
	if (colorChangesPrObject == nullptr) {
		GameObject::colorChangesPrObject = new Buffer<ThisColorChanges>(VK_BUFFER_USAGE_VERTEX_BUFFER_BIT);
		GameObject::allColorChanges = new UniformBuffer<ColorChangeStruct>(FRAMES_IN_FLIGHT, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT);
		return;
	}
	std::vector<glm::mat4> transforms;
	std::list<GameObject*>::iterator it = createdGameObject.begin();
	for (int i = 0; i < createdGameObject.size(); i++) {
		transforms.push_back((*it)->transform->modelTransform);

		it = std::next(it);
	}

	if (transforms.size() == 0)
		return;

	Transform::AddToMemory(transforms);
}

GameObject* GameObject::addOBB(OBB obb) {
	obbs.push_back(obb);
	return this;
}

GameObject* GameObject::AddDefaultOBB(glm::vec3 offset, bool two_dim)
{
	std::array<glm::vec2, 3> maxD = GetModelMaxDistVert();
	if (this->GetModelName() == "")
		return this;

	auto found = default_obb_data::data.find(this->GetModelName());
	if (found != default_obb_data::data.end()) {
		for (const auto& x : found->second)
			addOBB(x);
		return this;
	}

	for (int i = 0; i < 3; i++)
		maxD[i] += glm::vec2(offset[i], -offset[i]);
	glm::vec3 c = { (maxD[0].x + maxD[0].y) / 2, (maxD[1].x + maxD[1].y) / 2, (maxD[2].x + maxD[2].y) / 2 };
	glm::vec3 s = { (maxD[0].x + abs(maxD[0].y)) / 2, (maxD[1].x + abs(maxD[1].y)) / 2, (maxD[2].x + abs(maxD[2].y)) / 2 };
	if (two_dim)
		s.z = 100000;

	return addOBB(OBB(c, s));
}

int GameObject::GetOBBsCount()
{
	return obbs.size();
}

void GameObject::AddColorChange(glm::vec3 from, glm::vec3 to)
{
	ColorChangeStruct colorChange;
	colorChange.from = from;
	colorChange.to = to;

	amountOfColorChanges++;
	if(amountOfColorChanges > 1)
		changeColor.insert(colorChangesIndex, colorChange);
	else {
		changeColor.push_back(colorChange);
		colorChangesIndex = std::prev(changeColor.end());
	}
}

GameObject* GameObject::ChangeModel(std::string model)
{
	createdGameObject.erase(i);
	delete this->model;

	std::pair<Model*, uint32_t> newModel = Model::Create(model);
	this->model = newModel.first;

	createdGameObject.insert(std::next(createdGameObject.begin(), newModel.second), this);

	i = std::next(createdGameObject.begin(), newModel.second);
	return this;
}
