#include "Entity.h"
#include <Logger.h>
#include <TransformComponent.h>


Entity::Entity(std::string friendlyName) 
{}

Entity::~Entity()
{}

void Entity::Start() 
{
	Logger::Info("A new entity has been instantiate");
}

void Entity::Update(float fDeltaTime)
{}

void Entity::Destroy() 
{}

void Entity::Draw(sf::RenderWindow& window) const 
{}

