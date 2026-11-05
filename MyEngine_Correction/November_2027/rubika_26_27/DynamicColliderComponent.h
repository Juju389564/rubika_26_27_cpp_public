#pragma once

#include <IComponent.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Transform.hpp>

class TransformComponent;

class DynamicColliderComponent : public IComponent
{
public:
	DynamicColliderComponent(Entity& entity);
	void Start();
	void Update(float fDeltaTime);
	void Destroy();


	void SetPosition(TransformComponent* tc);
	sf::Vector2f GetPosition() const{ return Position; }

	void SetSize(sf::Vector2f size, TransformComponent* tc);
	sf::Vector2f GetSize() const { return Size; }

	void SetOffset(sf::Vector2f offset, TransformComponent* tc);
	sf::Vector2f GetOffset() const { return Offset; }

	void SetVisible(bool visibilityState);

	const char* GetName() const override { return "DynamicColliderComponent"; }
	void DrawDebug() override;

	void draw(sf::RenderWindow& window) const;


private :
	sf::FloatRect Collider;

	bool isVisible;
	sf::RectangleShape ColliderRectangle;

	sf::Vector2f Position;
	sf::Vector2f Size;
	sf::Vector2f Offset;
};