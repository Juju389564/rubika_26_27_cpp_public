#include <InputAction.h>

InputAction::InputAction(const std::string& name) : ActionName(name) 
{
	ActionNameHash = std::hash<std::string>{} (ActionName);
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

void InputAction::RegisterCallback(const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	Callbacks.emplace_back(sCallbackInfo{ debugName, callback, owner });
}

void InputAction::UnregisterCallbacks(void* owner)
{
	/*
	std::size_t vectorSize = Callbacks.size();
	for (std::size_t i = vectorSize; i > 0; --i)
	{
		sCallbackInfo callback = Callbacks.at(i);
		if (callback.Owner == owner)
		{
			Callbacks.erase(i);
		}
	}*/

	for (auto i = begin(Callbacks); i != end(Callbacks); ++i)
	{
		if (i->Owner == owner)
		{
			delete(&i);
			Callbacks.erase(i);
		}
	}
	
}
