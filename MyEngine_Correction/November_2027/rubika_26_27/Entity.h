#pragma once

#include <string>
#include <vector>

class IComponent;

namespace sf
{
	class RenderWindow;
}

class Entity
{
public:
	Entity(std::string friendlyName = "");
	virtual ~Entity();

	virtual void Start();
	virtual void Update(float fDeltaTime);
	virtual void Destroy();

	template <typename C>
	C* AddComponent();

	template <typename C>
	C* GetComponent() const;

	/*
	 * This function will be removed later, when the rendering system will be more robust. But for now, we need it here.
	 * Function that will be called each frame to draw the entity
	*/
	void Draw(sf::RenderWindow& window) const;
	void DrawDebug();

	const std::string& GetName() const { return FriendlyName; }

	const __int8 GetColliderState() { return colliderState; }
	void AddNewTag(__int8 tag); 
	void RemoveTag(__int8 tag);
	bool HasTag(__int8 tag);

	void AddNewTags(std::vector<__int8> tags);
	void RemoveTags(std::vector<__int8> tags);
	bool HasTags(std::vector<__int8> tags);


protected:
	std::string FriendlyName;
	std::vector<IComponent*> Components;
	__int8 colliderState;
};

#include <Entity.hxx>
