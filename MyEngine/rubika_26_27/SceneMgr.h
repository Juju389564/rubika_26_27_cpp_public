#pragma once

#include "Scene.h"
#include "DebugMgr.h"

#include <memory>
#include <string>
#include <unordered_map>

class SceneMgr final : public IDebugable
{
public:
    void Init();
    void Shut();

    void Update(float fDeltaTime);
    void Draw(sf::RenderWindow& window) const;

    bool AddScene(std::unique_ptr<Scene> scene);   
    bool RemoveScene(const std::string& name);     
    bool SetActiveScene(const std::string& name);  

    Scene* GetActiveScene() const;

    virtual void DrawDebug() override;

private:
    std::unordered_map<std::string, std::unique_ptr<Scene>> Scenes;
    Scene* ActiveScene = nullptr;
    int EntityCounter = 0;
    char NewSceneName[64] = "";
    int SceneCounter = 0;
};