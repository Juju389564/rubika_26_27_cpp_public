#pragma once

#include "IDrawable.h"
#include <SFML/Graphics/Sprite.hpp>

class SpriteDrawable : public IDrawable
{
public:
	SpriteDrawable(const sf::Texture& texture);
	~SpriteDrawable();

	SpriteDrawable(const SpriteDrawable&) = delete;
	SpriteDrawable& operator=(const SpriteDrawable&) = delete;

	void Start() override;
	void Update(float deltaTime) override;
	void Destroy() override;

	void SetTexture(const sf::Texture& texture);
	void SetTextureRect(const sf::IntRect& rect);

private:
	sf::Sprite SfSprite;
};