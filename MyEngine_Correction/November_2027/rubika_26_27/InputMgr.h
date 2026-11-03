#pragma once

#include <DebugMgr.h>
#include <SFML/Graphics.hpp>

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

	InputAction* CreateInputAction(const std::string& ActionName);
	void DeleteInputAction(InputAction* ia);

	void BindActionToKey(InputAction* ia, sf::Keyboard::Key key);
	void UnbindActionToKey(InputAction* ia, sf::Keyboard::Key key);

	void registerCallbackToAction(InputAction* ia, const std::string& debugName, std::function<void(float)> callback, void* owner);
	void unregisterCallbacksToAction(InputAction* ia, void* owner);



	virtual void DrawDebug() override;

protected:

	std::unordered_map<std::size_t, InputAction*> InputActions;

};

