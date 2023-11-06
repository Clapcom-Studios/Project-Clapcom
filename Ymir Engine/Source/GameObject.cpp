#include "GameObject.h"

#include "Application.h"
#include "ModuleScene.h"

GameObject::GameObject(std::string name, GameObject* parent)
{
	this->name = name;
	mParent = parent;

	active = true;
	selected = false;

	AddComponent(new CTransform(this));

	UID = Random::Generate();

}

GameObject::~GameObject()
{
	
}

void GameObject::Update()
{
	// Check if any of the UIDs is repeated (it's not gonna happen)
	for (auto it = External->scene->gameObjects.begin(); it != External->scene->gameObjects.end(); ++it) {

		if ((*it)->UID == this->UID && (*it) != this) { // If it is repeated, regenerate

			this->UID = Random::Generate();

		}

	}

}

void GameObject::Enable()
{
	if (!active) {
		active = true;
	}
}

void GameObject::Disable()
{
	if (active) {
		active = false;
	}
}

void GameObject::SetParent(GameObject* newParent)
{
	mParent = newParent;
}

void GameObject::AddChild(GameObject* child)
{
	mChildren.push_back(child);
}

void GameObject::AddComponent(Component* component)
{
	mComponents.push_back(component);
}

Component* GameObject::GetComponent(ComponentType ctype)
{
	for (auto it = mComponents.begin(); it != mComponents.end(); ++it) {

		if ((*it)->ctype == ctype) {

			return (*it);

		}

	}

	return nullptr;
}

void GameObject::DestroyGameObject()
{
	mTransform = nullptr;

	if (this->mParent)
	{
		auto it = std::find(this->mParent->mChildren.begin(), this->mParent->mChildren.end(), this);
		if (it != this->mParent->mChildren.end())
		{
			this->mParent->mChildren.erase(it);
		}
	}

	for (std::vector<GameObject*>::reverse_iterator it = mChildren.rbegin(); it != mChildren.rend(); ++it)
	{
		delete (*it);
		(*it) = nullptr;
	}

}
