#include "Entity.h"
#include "Logger.h"
#include "TransformComponent.h"
#include "RectangleDrawable.h"

#include <SFML/Graphics/RenderWindow.hpp>
#include "RenderComponent.h"



Entity::Entity(std::string friendlyName)
	: FriendlyName(std::move(friendlyName))
	, Transform (new::TransformComponent(*this))
	, Render(new RenderComponent(*this))
{
	Logger::Info("Entity created: " + FriendlyName);
	auto rectangle = std::make_unique<RectangleDrawable>();
	rectangle->SetSize({ 100.f, 100.f });
	rectangle->SetFillColor(sf::Color::Red);

	sf::Transform transform;
	transform.translate({ 800.f, 500.f });
	rectangle->SetWorldTransform(transform);

	Render->AddDrawable(std::move(rectangle));
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