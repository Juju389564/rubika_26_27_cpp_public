#pragma once

#include "IDrawable.h"
#include <SFML/Graphics/RectangleShape.hpp>

class RectangleDrawable : public IDrawable
{
public:
	RectangleDrawable();
	~RectangleDrawable();

	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	void SetSize(sf::Vector2f size);
	void SetFillColor(sf::Color color);

private:
	sf::RectangleShape Shape;
};