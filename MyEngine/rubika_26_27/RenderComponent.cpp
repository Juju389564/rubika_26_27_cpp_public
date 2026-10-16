#include "RenderComponent.h"
#include "IDrawable.h"
#include "Logger.h"

RenderComponent::RenderComponent(Entity& entity) : IComponent(entity)
{
}

RenderComponent::~RenderComponent()
{
}

void RenderComponent::Start()
{
	for (auto& drawable : Drawables)
		drawable->Start();
}

void RenderComponent::Update(float fDeltaTime)
{
	for (auto& drawable : Drawables)
		drawable->Update(fDeltaTime);
}

void RenderComponent::Destroy()
{
	for (auto& drawable : Drawables)
		drawable->Destroy();
	Drawables.clear();
}

void RenderComponent::Draw(sf::RenderWindow& window) const
{
	for (const auto& drawable : Drawables)
	{
		if (drawable->IsVisible())
			drawable->Draw(window);
	}
}

IDrawable* RenderComponent::AddDrawable(std::unique_ptr<IDrawable> drawable)
{
	if (!drawable)
	{
		Logger::Warning("RenderComponent: tried to add a null drawable");
		return nullptr;
	}

	IDrawable* ptr = drawable.get();
	Drawables.push_back(std::move(drawable));
	return ptr;
}