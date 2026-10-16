#pragma once

#include "IComponent.h"

#include <SFML/Graphics/Transform.hpp>

class TransformComponent : public IComponent
{
public:
	struct DecomposedData
	{
		sf::Vector2f Position;
		float Rotation;
		sf::Vector2f Scale;
	};

	TransformComponent( Entity& ParentEntity);
	~TransformComponent();


	const sf::Transform& GetMatrix() const;

	void SetWorldPosition(sf::Vector2f newPosition);
	sf::Vector2f GetWorldPosition() const;

	void SetWorldPositionQuery(sf::Vector2f newPosition);
	sf::Vector2f GetWorldPositionQuery() const;

	void SetRotation(float newRotation);
	float GetRotation() const;

	void SetScale(sf::Vector2f newScale);
	sf::Vector2f GetScale() const;

	void Start() override;
	void Update(float fDeltaTime) override;
	void Destroy() override;

private:
	sf::Transform Matrix;

	sf::Vector2f WorldPosition;
	float Rotation;
	sf::Vector2f Scale;

	sf::Vector2f WorldPositionQuery;

	void UpdateMatrix();
};
