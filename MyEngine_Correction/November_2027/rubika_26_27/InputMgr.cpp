#include "InputMgr.h"

#include <Globals.h>

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

void InputMgr::DrawDebug()
{
#ifdef _USE_IMGUI


#endif
}