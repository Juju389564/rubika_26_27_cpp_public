#pragma once

#include <SFML/Graphics.hpp>

#include <functional>
#include <string>


class InputAction
{
public:
	InputAction() = default;
	InputAction(const std::string& name);

	void update(float deltatime);

	void SetActionName(const std::string& name);
	void SetValue(float value);
	void AddActionKey(sf::Keyboard::Key key);
	void DeleteActionKey(sf::Keyboard::Key key);

	void RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner);
	void UnregisterCallbacks(void* owner);

	void DrawDebug();

	const std::string& GetActionName() const { return ActionName; };
	std::size_t GetActionNameHash() const {return ActionNameHash;};
	float GetValue() { return Value; };

	struct keyType
	{
		const char* keyString;
		sf::Keyboard::Key sfKey;
	};

private:
	struct sCallbackInfo
	{
		std::string DebugName;
		std::function<void(float)> Callback;
		void* Owner = nullptr;
	};


	std::string ActionName;
	std::size_t ActionNameHash;
	float Value = 0.f;
	std::vector<sCallbackInfo> Callbacks;
	std::vector <sf::Keyboard::Key> actionKeys;
	char NewKeyName[32] = "";
};