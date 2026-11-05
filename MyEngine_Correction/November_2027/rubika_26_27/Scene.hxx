#pragma once

#include "Scene.h"

template <typename E>
inline E* Scene::AddEntity()
{
	if (!std::is_base_of<Entity, E>())
		return nullptr;

	if (GetEntity<E>())
		return nullptr;

	E* entity = new E(*this);
	Entities.push_back(entity);
	return entity;
}

template <typename E>
inline E* Scene::GetEntity() const
{
	if (!std::is_base_of<Entity, E>())
		return nullptr;

	for (auto& entity : Entities)
	{
		if (dynamic_cast<E*>(entity))
		{
			return reinterpret_cast<E*>(entity);
		}
	}

	return nullptr;
}