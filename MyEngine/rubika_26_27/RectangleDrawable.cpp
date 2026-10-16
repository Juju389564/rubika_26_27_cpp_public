#include "RectangleDrawable.h"

RectangleDrawable::RectangleDrawable()
{
	Drawable = &Shape;
}

RectangleDrawable::~RectangleDrawable()
{
}

void RectangleDrawable::Start() {}
void RectangleDrawable::Update(float deltaTime) {}
void RectangleDrawable::Destroy() {}

void RectangleDrawable::SetSize(sf::Vector2f size)
{
	Shape.setSize(size);
}

void RectangleDrawable::SetFillColor(sf::Color color)
{
	Shape.setFillColor(color);
}