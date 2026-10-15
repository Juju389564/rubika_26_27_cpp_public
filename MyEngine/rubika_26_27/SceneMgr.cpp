#include "SceneMgr.h"

#include "Logger.h"
#include "Entity.h"
#include "Globals.h"

#ifdef _USE_IMGUI
#include <Imgui/imgui.h>
#endif

void SceneMgr::Init()
{
    gData.DebugMgr->RegisterDebugableWindow("Scenes", this);
}

void SceneMgr::Shut()
{
    gData.DebugMgr->UnregisterDebugableWindow("Scenes");

    if (ActiveScene)
    {
        ActiveScene->Destroy();
        ActiveScene = nullptr;
    }
    Scenes.clear();
}

void SceneMgr::Update(float fDeltaTime)
{
    if (ActiveScene)
        ActiveScene->Update(fDeltaTime);
}

void SceneMgr::Draw(sf::RenderWindow& window) const
{
    if (ActiveScene)
        ActiveScene->Draw(window);
}

bool SceneMgr::AddScene(std::unique_ptr<Scene> scene)
{
    if (!scene)
    {
        Logger::Warning("SceneMgr: tried to add a null scene");
        return false;
    }

    const std::string name = scene->GetName();
    if (Scenes.find(name) != Scenes.end())
    {
        Logger::Error("SceneMgr: a scene named '" + name + "' already exists");
        return false;
    }

    Scenes.emplace(name, std::move(scene));
    return true;
}

bool SceneMgr::RemoveScene(const std::string& name)
{
    auto it = Scenes.find(name);
    if (it == Scenes.end())
    {
        Logger::Warning("SceneMgr: scene '" + name + "' not found");
        return false;
    }

    if (it->second.get() == ActiveScene)
    {
        ActiveScene->Destroy();
        ActiveScene = nullptr;
    }

    Scenes.erase(it);
    return true;
}

bool SceneMgr::SetActiveScene(const std::string& name)
{
    auto it = Scenes.find(name);
    if (it == Scenes.end())
    {
        Logger::Error("SceneMgr: cannot activate unknown scene '" + name + "'");
        return false;
    }

    Scene* newScene = it->second.get();
    if (newScene == ActiveScene)
        return true;

    if (ActiveScene) 
    {
        ActiveScene->Destroy();
        EntityCounter = 0;
    }
        

    ActiveScene = newScene;
    ActiveScene->Start();
    return true;
}

Scene* SceneMgr::GetActiveScene() const 
{ 
    return ActiveScene; 
}


void SceneMgr::DrawDebug()
{
#ifdef _USE_IMGUI

    ImGui::InputText("##NewSceneName", NewSceneName, sizeof(NewSceneName));
    ImGui::SameLine();
    if (ImGui::Button("Add scene"))
    {
        std::string name = NewSceneName;
        if (name.empty())
        {
            name = "Scene " + std::to_string(SceneCounter++);
        }

        if (AddScene(std::make_unique<Scene>(name)))
        {
            NewSceneName[0] = '\0';   
        }
    }
    ImGui::Separator();

    ImGui::Text("Active scene: %s", ActiveScene ? ActiveScene->GetName().c_str() : "none");
    ImGui::Separator();

    std::string sceneToDelete;
    bool deleteRequested = false;
    
    for (const auto& [name, scene] : Scenes)
    {
        ImGui::PushID(name.c_str());


        if (ImGui::Button("Activate"))
        {
            SetActiveScene(name);
        }
        ImGui::SameLine();
        
        if (ImGui::Button("Add entity"))
        {
            scene->AddEntity(std::make_unique<Entity>("Entity " + std::to_string(EntityCounter++)));
        }
        ImGui::SameLine();

        if (ImGui::Button("DELETE SCENE"))
        {
            sceneToDelete = name;
            deleteRequested = true;
        }
        ImGui::SameLine();

        const bool isActive = (scene.get() == ActiveScene);
        const std::string label = name + (isActive ? " (active)" : "");
        if (ImGui::TreeNode(label.c_str()))
        {
            for (const auto& entity : scene->GetEntities())
            {
                ImGui::BulletText("%s", entity->GetName().c_str());
            }
            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    if (deleteRequested)
    {
        RemoveScene(sceneToDelete);
    }

#endif
}
