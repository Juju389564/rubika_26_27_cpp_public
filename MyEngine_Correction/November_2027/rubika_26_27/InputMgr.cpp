#include "InputMgr.h"

#include <Globals.h>
#include <InputAction.h>
#include <Logger.h>

#include <Imgui/imgui.h>

void InputMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("InputMgr", this);
}

void InputMgr::Update(float deltaTime)
{
	for (auto& [key, action] : InputActions)
	{
		if (action != nullptr)
		{
			action->update(deltaTime);
		}
	}
}


void InputMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("InputMgr");
}


InputAction* InputMgr::CreateInputAction(const std::string& ActionName)
{	
	std::size_t hash = std::hash<std::string>{}(ActionName);
	const auto& it = InputActions.find(hash);
	if (it != InputActions.end())
	{
		return nullptr;
	}
	InputAction* ia = new InputAction(ActionName);
	InputActions.emplace(hash, ia);
	return ia;

}

void InputMgr::DeleteInputAction(InputAction* ia)
{
	const auto& it = InputActions.find(ia->GetActionNameHash());
	if (it != InputActions.end())
	{
		InputActions.erase(it);
	}
}


void InputMgr::BindActionToKey(InputAction* ia, sf::Keyboard::Key key)
{
	const auto& it = InputActions.find(ia->GetActionNameHash());
	if (it != InputActions.end())
	{
		ia->AddActionKey(key);
	}
	
}

void InputMgr::UnbindActionToKey(InputAction* ia, sf::Keyboard::Key key)
{
	const auto& it = InputActions.find(ia->GetActionNameHash());
	if (it != InputActions.end())
	{
		ia->DeleteActionKey(key);
	}

}

void InputMgr::registerCallbackToAction(InputAction* ia, const std::string& debugName, std::function<void(float)> callback, void* owner)
{
	const auto& it = InputActions.find(ia->GetActionNameHash());
	if (it != InputActions.end())
	{
		ia->RegisterCallback(debugName, callback, owner);
	}
	else
	{
		Logger::Error("You try to register a callback to an action that doesn't exist");
	}
	
}

void InputMgr::unregisterCallbacksToAction(InputAction* ia, void* owner)
{
	const auto& it = InputActions.find(ia->GetActionNameHash());
	if (it != InputActions.end())
	{
		ia->UnregisterCallbacks(owner);
	}
	else
	{
		Logger::Error("You try to unregister callbacks to an action that doesn't exist");
	}
}


void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI

	for (auto it : InputActions)
	{
		ImGui::PushID(it.second);

		bool bTree = ImGui::TreeNode("%s (%lld)", it.second->GetActionName().c_str(), it.second->GetActionNameHash());

		if (bTree)
		{
			it.second->DrawDebug();
			ImGui::TreePop();
		}

		ImGui::PopID();
	}

#endif
}