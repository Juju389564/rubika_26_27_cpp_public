#include "DynamicColliderComponent.h"

DynamicColliderComponent::DynamicColliderComponent(Entity& entity) : IComponent(entity)
{
}


void DynamicColliderComponent::Start()
{
}

void DynamicColliderComponent::Update(float fDeltaTime)
{
}

void DynamicColliderComponent::Destroy()
{
}


void DynamicColliderComponent::SetPosition(sf::Vector2f position)
{
	Position = position;
}

void DynamicColliderComponent::SetSize(sf::Vector2f size)
{
	Size = size;
}

void DynamicColliderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	
#endif
}