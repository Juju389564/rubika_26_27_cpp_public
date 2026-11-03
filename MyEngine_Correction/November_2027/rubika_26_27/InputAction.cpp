#include "InputAction.h"

#include <Imgui/imgui.h>

InputAction::InputAction(const std::string& name) : ActionName(name) 
{
	ActionNameHash = std::hash<std::string>{} (ActionName);
}

void InputAction::update(float deltatime)
{
	for (sf::Keyboard::Key key : actionKeys)
	{
		if (sf::Keyboard::isKeyPressed(key))
		{
			SetValue(1.f);
			break;
		}
		else
		{
			SetValue(0.f);
		}
	}
}

void InputAction::SetActionName(const std::string& name)
{
	ActionName = name;
	ActionNameHash = std::hash<std::string>{} (ActionName);
}

void InputAction::SetValue(float value)
{
	Value = value;
}

void InputAction::AddActionKey(sf::Keyboard::Key key)
{
	actionKeys.emplace_back(key);
}

void InputAction::DeleteActionKey(sf::Keyboard::Key key) 
{
	auto it = std::find(actionKeys.begin(), actionKeys.end(), key);
	if (it != actionKeys.end())
	{
		actionKeys.erase(it);
	}
}

void InputAction::RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	Callbacks.emplace_back(sCallbackInfo{ debugName, callback, owner });
}

void InputAction::UnregisterCallbacks(void* owner)
{

	for (auto i = begin(Callbacks); i != end(Callbacks); ++i)
	{
		if (i->Owner == owner)
		{
			//delete(&i);
			Callbacks.erase(i);
		}
	}
	
}

void InputAction::DrawDebug()
{
#ifdef _USE_IMGUI
	for (sCallbackInfo c : Callbacks)
	{
		ImGui::Text(c.DebugName.c_str());
	}

	for (sf::Keyboard::Key key : actionKeys)
	{
		sf::Keyboard::Scancode scancode = sf::Keyboard::delocalize(key);
		std::string keyName = sf::Keyboard::getDescription(scancode).toAnsiString();
		ImGui::Text(keyName.c_str());
	}

#endif
}
