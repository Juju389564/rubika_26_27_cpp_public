#include "InputAction.h"

#include <Logger.h>

#include <Imgui/imgui.h>

#define IM_COUNTOF(_ARR) ((int)(sizeof(_ARR) / sizeof(*(_ARR))))

int const ArraySize = 30;

InputAction::keyType keyArray[ArraySize] =
{
	   {"Right Arrow", sf::Keyboard::Key::Right},
	   {"Left Arrow", sf::Keyboard::Key::Left},
	   {"Up Arrow", sf::Keyboard::Key::Up},
	   {"Down Arrow", sf::Keyboard::Key::Down},
	   {"A", sf::Keyboard::Key::A},
	   {"B", sf::Keyboard::Key::B},
	   {"C", sf::Keyboard::Key::C},
	   {"D", sf::Keyboard::Key::D},
	   {"E", sf::Keyboard::Key::E},
	   {"F", sf::Keyboard::Key::F},
	   {"G", sf::Keyboard::Key::G},
	   {"H", sf::Keyboard::Key::H},
	   {"I", sf::Keyboard::Key::I},
	   {"J", sf::Keyboard::Key::J},
	   {"K", sf::Keyboard::Key::K},
	   {"L", sf::Keyboard::Key::L},
	   {"M", sf::Keyboard::Key::M},
	   {"N", sf::Keyboard::Key::N},
	   {"O", sf::Keyboard::Key::O},
	   {"P", sf::Keyboard::Key::P},
	   {"Q", sf::Keyboard::Key::Q},
	   {"R", sf::Keyboard::Key::R},
	   {"S", sf::Keyboard::Key::S},
	   {"T", sf::Keyboard::Key::T},
	   {"U", sf::Keyboard::Key::U},
	   {"V", sf::Keyboard::Key::V},
	   {"W", sf::Keyboard::Key::W},
	   {"X", sf::Keyboard::Key::X},
	   {"Y", sf::Keyboard::Key::Y},
	   {"Z", sf::Keyboard::Key::Z},
};




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

	const char* items[ArraySize];
	for (int i = 0; i < ArraySize; ++i)
	{
		const char* keyName = keyArray[i].keyString;
		items[i] = keyName;
	}

	static int item_current = -1;
	ImGui::Combo("##", &item_current, items, IM_COUNTOF(items));
	ImGui::SameLine();
	if (ImGui::Button("ADD KEY BINDING"))
	{
		if (item_current > -1)
		{
			auto it = std::find(actionKeys.begin(), actionKeys.end(), keyArray[item_current].sfKey);
			if (it != actionKeys.end())
			{
				Logger::Warning("The key you try to bind is already bind to this Input Action");
			}
			else
			{
				AddActionKey(keyArray[item_current].sfKey);
			}
		}

	}


	for (sf::Keyboard::Key &key : actionKeys)
	{	
		ImGui::PushID(&key);
		sf::Keyboard::Scancode scancode = sf::Keyboard::delocalize(key);
		std::string keyName = sf::Keyboard::getDescription(scancode).toAnsiString();
		ImGui::Text(keyName.c_str());

		ImGui::SameLine();
		if (ImGui::Button("DELETE BINDING"))
		{
			DeleteActionKey(key);
		}
		ImGui::PopID();

	}

#endif
}
