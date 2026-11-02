#include <InputAction.h>

InputAction::InputAction(const std::string& name) : ActionName(name) {}

void InputAction::SetActionName(const std::string& name)
{
	ActionName = name;
}

void InputAction::SetValue(float value)
{
	Value = value;
}

void InputAction::RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	Callbacks.emplace(ActionNameHash, sCallbackInfo{ debugName, callback, owner });
}

void InputAction::UnregisterCallbacks(void* owner)
{
	for (auto i = Callbacks.begin() ; i != Callbacks.end(); ++i)
	{
		auto callbackKey = i->first;
		const auto& callback = Callbacks.find(callbackKey);
		
		/*
		if ()  // trouver owner de callback pour comparer à celui envoyer ici et si c'est le même suppr callback de Callbacks
		{

		}*/
	}
}