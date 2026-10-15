#include "Entity.h"
#include "Logger.h"
#include "TransformComponent.h"

#include <SFML/Graphics/RenderWindow.hpp>

Entity::Entity(std::string friendlyName)
	: FriendlyName(std::move(friendlyName))
	, Transform (new::TransformComponent())
{
	Logger::Info("Entity created: " + FriendlyName);
}

Entity::~Entity()
{
	Logger::Info("Entity destroyed: " + FriendlyName);
	delete Transform;
	Transform = nullptr;
}

void Entity::Start()
{
	Transform->Start();
}

void Entity::Update(float fDeltaTime)
{
	Transform->Update(fDeltaTime);
}

void Entity::Destroy()
{
	Transform->Destroy();
}

void Entity::Draw(sf::RenderWindow& window) const
{
}

const std::string& Entity::GetName() const 
{ 
	return FriendlyName; 
}