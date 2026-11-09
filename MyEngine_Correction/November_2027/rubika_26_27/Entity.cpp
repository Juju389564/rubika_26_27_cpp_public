#include "Entity.h"

#include <IComponent.h>
#include <RenderComponent.h>
#include <Imgui/imgui.h>

#include <ConfigFlag.h>


Entity::Entity(std::string friendlyName) : FriendlyName(friendlyName)
{}

Entity::~Entity()
{
	for (const IComponent* component : Components)
	{
		delete component;
	}

	Components.clear();
}

void Entity::Start()
{
	for (IComponent* c : Components)
	{
		c->Start();
	}
}

void Entity::Update(float fDeltaTime)
{
	for (IComponent* c : Components)
	{
		c->Update(fDeltaTime);
	}
}

void Entity::Destroy()
{
	for (IComponent* c : Components)
	{
		c->Destroy();
	}
}

void Entity::Draw(sf::RenderWindow& window) const
{
	RenderComponent* r = GetComponent<RenderComponent>();
	if (r)
	{
		r->Draw(window);
	}
}

void Entity::DrawDebug()
{
#ifdef _USE_IMGUI
	for (IComponent* c : Components)
	{
		if (ImGui::TreeNode(c->GetName()))
		{
			c->DrawDebug();
			ImGui::TreePop();
		}
	}
#endif
}



void Entity::AddNewTag(__int8 tag)
{
	colliderState |= tag;
}

void Entity::RemoveTag(__int8 tag)
{
	colliderState &= ~tag;
}

bool Entity::HasTag(__int8 tag)
{
	if (colliderState &= tag )
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Entity::AddNewTags(std::vector<__int8> tags)
{
	for (__int8 tag : tags)
	{
		AddNewTag(tag);
	}
}

void Entity::RemoveTags(std::vector<__int8> tags)
{
	for (__int8 tag : tags)
	{
		RemoveTag(tag);
	}
}