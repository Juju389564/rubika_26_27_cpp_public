#pragma once

#include <string>

//#include "RenderComponent.h"

namespace sf
{
	class RenderWindow;
}

class TransformComponent;
class RenderComponent;

class Entity //: public RenderComponent
{
public:
	Entity(std::string friendlyName = "");
	virtual ~Entity();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	Entity(const Entity&) = delete;
	Entity& operator=(const Entity&) = delete;

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;

	const std::string& GetName() const;

protected:
	std::string FriendlyName;
	TransformComponent* Transform;
	RenderComponent* Render;
};
