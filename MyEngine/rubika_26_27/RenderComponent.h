#pragma once

#include "IComponent.h"

#include <memory>
#include <vector>

namespace sf { class RenderWindow; }
class IDrawable;

class RenderComponent : public IComponent
{
public:
	RenderComponent(Entity& entity);
	~RenderComponent();

	void Start() override;
	void Update(float fDeltaTime) override;
	void Destroy() override;

	void Draw(sf::RenderWindow& window) const;

	IDrawable* AddDrawable(std::unique_ptr<IDrawable> drawable);

private:
	std::vector<std::unique_ptr<IDrawable>> Drawables;
};