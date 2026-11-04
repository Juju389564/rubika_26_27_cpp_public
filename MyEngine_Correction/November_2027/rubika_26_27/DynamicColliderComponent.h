#pragma once

#include <IComponent.h>
#include <SFML/Graphics/Rect.hpp>

class DynamicColliderComponent : public IComponent
{
public:
	DynamicColliderComponent(Entity& entity);
	void Start();
	void Update(float fDeltaTime);
	void Destroy();


	void SetPosition(sf::Vector2f position);
	sf::Vector2f GetPosition() const{ return Position; }

	void SetSize(sf::Vector2f size);
	sf::Vector2f GetSize() const { return Size; }

	const char* GetName() const override { return "DynamicColliderComponent"; }
	void DrawDebug() override;

private :
	sf::FloatRect Collider;

	sf::Vector2f Position;
	sf::Vector2f Size;
};