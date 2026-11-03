#pragma once

#include <DebugMgr.h>
#include <SFML/Window/Keyboard.hpp>

class InputAction;

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

	InputAction* CreateInputAction(const std::string& ActionName);
	void DeleteInputAction(InputAction* ia);

	virtual void DrawDebug() override;

protected:

	std::unordered_map<std::size_t, InputAction*> InputActions;

};