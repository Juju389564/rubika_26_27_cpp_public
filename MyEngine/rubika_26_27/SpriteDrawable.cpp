#include "SpriteDrawable.h"

#include <SFML/Graphics/Texture.hpp>

SpriteDrawable::SpriteDrawable(const sf::Texture& texture) : SfSprite(texture)
{
	Drawable = &SfSprite;
}

SpriteDrawable::~SpriteDrawable()
{
}

void SpriteDrawable::Start() {}
void SpriteDrawable::Update(float deltaTime) {}
void SpriteDrawable::Destroy() {}

void SpriteDrawable::SetTexture(const sf::Texture& texture)
{
	SfSprite.setTexture(texture, true);
}

void SpriteDrawable::SetTextureRect(const sf::IntRect& rect)
{
	SfSprite.setTextureRect(rect);
}