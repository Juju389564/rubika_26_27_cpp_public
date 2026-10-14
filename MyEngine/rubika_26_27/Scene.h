#pragma once

#include <memory>
#include <string>
#include <vector>

namespace sf 
{ 
	class RenderWindow; 
}


class Entity;

class Scene
{
public:
	Scene(const std::string& name);
	virtual ~Scene();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();


	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

	const std::string& GetName() const;

	Entity* AddEntity(std::unique_ptr<Entity> entity);
	bool RemoveEntity(Entity* entity);
	const std::vector<std::unique_ptr<Entity>>& GetEntities() const;

private:
	std::string Name;
	std::vector<std::unique_ptr<Entity>> Entities;
	bool IsStarted = false;
};