#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include <Globals.h>
#include <Profiler.h>
#include <Logger.h>
#include <DebugMgr.h>
#include <Scene.h>
#include <SceneMgr.h>
#include <TextureMgr.h>
#include <Entity.h>
#include <memory>

int main()
{
    gData.Init();

    Logger::Info("Working dir: " + std::filesystem::current_path().string());
    gData.TextureMgr->LoadTexture("../Ressources/enemy_texture_1.png");
    gData.TextureMgr->LoadTexture("../Ressources/enemy_texture_2.png");
    gData.TextureMgr->LoadTexture("../Ressources/enemy_texture_3.png");
    gData.TextureMgr->LoadTexture("../Ressources/enemy_texture_4.png");
    gData.TextureMgr->LoadTexture("../Ressources/enemy_texture_5.png");
    gData.TextureMgr->LoadTexture("../Ressources/Antonio_Belpaese.png");
    gData.TextureMgr->LoadTexture("../Ressources/IsaacSprite.png");
    gData.TextureMgr->LoadTexture("../Ressources/Basement.png");

    gData.SceneMgr->AddScene(std::make_unique<Scene>("Menu"));
    gData.SceneMgr->AddScene(std::make_unique<Scene>("Game"));
    gData.SceneMgr->SetActiveScene("Menu");

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    auto scene = std::make_unique<Scene>("Test");
    scene->AddEntity(std::make_unique<Entity>("Rectangle test"));
    gData.SceneMgr->AddScene(std::move(scene));
    gData.SceneMgr->SetActiveScene("Test");

    ImGui::SFML::Init(window);


    Logger::Info("This is an info message. That is useful");
    Logger::Warning("This is a warning message. Should I worry?");
    Logger::Error("This is an error message. Oh sh*t!!");

    sf::Clock clock;
    clock.restart();
    
    while (window.isOpen())
    {
        gData.IncrementFrameCount();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLACK, "Frame %llu", gData.FrameCount);

        int deltaTimeMicroS = clock.getElapsedTime().asMicroseconds();
        float fDeltaTimeMS = (float)deltaTimeMicroS / 1000.f;
        float fDeltaTimeS = fDeltaTimeMS / 1000.f;
        sf::Time imGuiTime = clock.restart();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_BLUE, "Event & Input");

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }

#ifdef _USE_IMGUI
            ImGui::SFML::ProcessEvent(window, event.value());
#endif
        }

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
#ifdef _USE_IMGUI
        ImGui::SFML::Update(window, imGuiTime);
#endif

        // sample
        //ImGui::ShowDemoWindow();
        //Logger::DrawLogger();
        gData.DebugMgr->Draw();

        //fenetre de test des textures : 
        /*
        ImGui::Begin("Test texture");
        ImGui::Image(gData.TextureMgr->GetTextureData("enemy_texture_1").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("enemy_texture_2").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("enemy_texture_3").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("enemy_texture_4").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("enemy_texture_5").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("Antonio_Belpaese").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("IsaacSprite").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("None").Texture);
        ImGui::Image(gData.TextureMgr->GetTextureData("Basement").Texture);
        ImGui::End();
        */

        gData.SceneMgr->Update(fDeltaTimeS);

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();
        gData.SceneMgr->Draw(window);

#ifdef _USE_IMGUI
        ImGui::SFML::Render(window);
#endif
        window.display();

        PROFILER_EVENT_END();

        PROFILER_EVENT_END();
    }

    ImGui::SFML::Shutdown();

    gData.Shut();
    gData.Destroy();

    return 0;
}