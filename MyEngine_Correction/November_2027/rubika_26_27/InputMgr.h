#pragma once

#include <DebugMgr.h>
#include <SFML/Window/Keyboard.hpp>

namespace sf
{
	class RenderWindow;
}

class InputMgr : public IDebugable
{
public: 
	void Init();
	void Update(float deltaTime);

	void Shut();

	void Draw(sf::RenderWindow& window) const;

	virtual void DrawDebug() override;

};