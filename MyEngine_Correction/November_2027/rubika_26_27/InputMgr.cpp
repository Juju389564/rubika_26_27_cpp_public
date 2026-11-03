#include "InputMgr.h"

#include <Globals.h>
#include <InputAction.h>

void InputMgr::Init()
{
	gData.DebugMgr->RegisterDebugableWindow("InputMgr", this);
}

void InputMgr::Update(float deltaTime)
{

}


void InputMgr::Shut()
{
	gData.DebugMgr->UnregisterDebugableWindow("InputMgr");
}

void InputMgr::Draw(sf::RenderWindow& window) const
{

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


void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI


#endif
}