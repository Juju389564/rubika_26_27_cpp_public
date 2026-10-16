#include "IComponent.h"

#include "Entity.h"


IComponent::IComponent(Entity& entity)
	: ParentEntity{entity}
{ }


IComponent::~IComponent()
{ }