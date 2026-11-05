#pragma once

#include <SFML/Graphics/Transform.hpp>

#include <Entity.h>


class TransformComponent;
class DynamicColliderComponent;
class InputAction;

class Player : public Entity
{
public:
	Player();
	~Player();

	void Start();
	void Update(float fDeltaTime);
	void Destroy();

	struct directions
	{
		InputAction* directionAction;
		sf::Vector2f directionVector;
	};

	void moveEntity(std::vector<directions> directionVector, sf::Vector2f currentPosition, TransformComponent* tc, DynamicColliderComponent* dc);

private:
	sf::Vector2f startPostion = sf::Vector2f(100.f, 100.f);
	std::vector<directions> directionVector;
	sf::Vector2f currentPosition;
};
