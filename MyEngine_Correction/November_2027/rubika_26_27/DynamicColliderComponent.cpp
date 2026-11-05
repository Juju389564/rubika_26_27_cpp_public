#include "DynamicColliderComponent.h"

#include <TransformComponent.h>
#include <Entity.h>
#include <Logger.h>

#include <Imgui/imgui.h>

DynamicColliderComponent::DynamicColliderComponent(Entity& entity) : IComponent(entity)
{
	Collider = sf::FloatRect(Position, Size);

	ColliderRectangle.setOutlineColor(sf::Color::Green);
	ColliderRectangle.setOutlineThickness(1.f);
	ColliderRectangle.setFillColor(sf::Color::Transparent);
}


void DynamicColliderComponent::Start()
{
}

void DynamicColliderComponent::Update(float fDeltaTime)
{

	TransformComponent* tc = GetEntity().GetComponent<TransformComponent>();
	SetPosition(tc);
	Collider.position.x = Position.x;
	Collider.position.y = Position.y;
}

void DynamicColliderComponent::Destroy()
{
}


void DynamicColliderComponent::SetPosition(TransformComponent* tc)
{
	Position = tc->GetWorldPosition() + Offset;
	ColliderRectangle.setPosition(Offset);
}

void DynamicColliderComponent::SetSize(sf::Vector2f size, TransformComponent* tc)
{
	sf::Vector2f scale = tc->GetScale();
	Size = size.componentWiseMul(scale);
	ColliderRectangle.setSize(Size);
}

void DynamicColliderComponent::SetOffset(sf::Vector2f offset, TransformComponent* tc)
{
	Offset = offset;
	SetPosition(tc);
}

void DynamicColliderComponent::SetVisible(bool visibilityState)
{
	isVisible = visibilityState;
}

void DynamicColliderComponent::draw(sf::RenderWindow& window) const
{
	if(isVisible)
	{
		sf::RenderStates states = sf::RenderStates::Default;
		TransformComponent* tc = GetEntity().GetComponent<TransformComponent>();
		states.transform = tc->GetMatrix();

		window.draw(ColliderRectangle, states);
	}
}

void DynamicColliderComponent::DrawDebug()
{
#ifdef _USE_IMGUI
	static float vec2fSize[2] = {Size.x, Size.y};
	static float vec2fOffset[2] = { Offset.x, Offset.y };
	TransformComponent* tc = GetEntity().GetComponent<TransformComponent>();

	if (ImGui::InputFloat2("Collider size", vec2fSize))
	{		
		SetSize(sf::Vector2f(vec2fSize[0], vec2fSize[1]),tc);
	}

	if (ImGui::InputFloat2("Collider Offset", vec2fOffset))
	{
		SetOffset(sf::Vector2f(vec2fOffset[0], vec2fOffset[1]), tc);
	}

	static bool check = false;
	ImGui::Checkbox("Collider is Visible", &check);
	{
		SetVisible(check);
	}
		
#endif
}