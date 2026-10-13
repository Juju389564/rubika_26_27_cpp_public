#include "Scene.h"
#include "Entity.h"
#include "Logger.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include <algorithm>

Scene::Scene(const std::string& name) : Name(name) {}

Scene::~Scene() { Destroy(); }

const std::string& Scene::GetName() const { return Name; }

void Scene::Start()
{
	IsStarted = true;
	for (auto& entity : Entities)
		entity->Start();
}

void Scene::Update(float fDeltaTime)
{
	for (auto& entity : Entities)
		entity->Update(fDeltaTime);
}

void Scene::Destroy()
{
	for (auto& entity : Entities)
		entity->Destroy();
	Entities.clear();   
	IsStarted = false;
}

void Scene::Draw(sf::RenderWindow& window) const
{
	for (const auto& entity : Entities)
		entity->Draw(window);
}

Entity* Scene::AddEntity(std::unique_ptr<Entity> entity)
{
	if (!entity)
	{
		Logger::Warning("Scene '" + Name + "': tried to add a null entity");
		return nullptr;
	}

	Entity* ptr = entity.get();
	Entities.push_back(std::move(entity));

	if (IsStarted)      
		ptr->Start();

	return ptr;
}

bool Scene::RemoveEntity(Entity* entity)
{
	auto it = std::find_if(Entities.begin(), Entities.end(),
		[entity](const std::unique_ptr<Entity>& e) { return e.get() == entity; });

	if (it == Entities.end())
	{
		Logger::Warning("Scene '" + Name + "': entity to remove not found");
		return false;
	}

	(*it)->Destroy();
	Entities.erase(it);
	return true;
}

const std::vector<std::unique_ptr<Entity>>& Scene::GetEntities() const
{
	return Entities;
}
