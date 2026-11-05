#include <SFML/Graphics.hpp>
#include <Imgui/imgui.h>
#include <Imgui/imgui-SFML.h>

#include <Globals.h>
#include <TextureMgr.h>
#include <SceneMgr.h>
#include <Scene.h>
#include <Profiler.h>
#include <Logger.h>
#include <Entity.h>
#include <TransformComponent.h>
#include <RenderComponent.h>
#include <DynamicColliderComponent.h>
#include <SpriteDrawable.h>
#include <InputMgr.h>
#include <InputAction.h>


sf::Vector2f startPostion = sf::Vector2f(100.f, 100.f);

struct directions
{
    InputAction* directionAction;
    sf::Vector2f directionVector;
};


void moveEntity(std::vector<directions> directionVector, sf::Vector2f currentPosition, TransformComponent* tc, DynamicColliderComponent* dc)
{
    for (auto& [action, direction] : directionVector)
    {
        float valueAction = action->GetValue();
        if (valueAction == 1)
        {
            currentPosition += sf::Vector2f(direction);
            tc->SetWorldPosition(currentPosition);

        }
    }
    
}

int main()
{
    gData.Init();

    sf::RenderWindow window(sf::VideoMode({ 1080, 720 }), "SFML works!", sf::State::Windowed);

    ImGui::SFML::Init(window);

    Logger::Info("This is an info message. That is useful");
    Logger::Warning("This is a warning message. Should I worry?");
    Logger::Error("This is an error message. Oh sh*t!!");
    

    sf::Vector2f currentPosition = startPostion;

    float speed = 0.01f;

    sf::Vector2f up = sf::Vector2f(0.f, -speed);
    sf::Vector2f down = sf::Vector2f(0.f, speed);
    sf::Vector2f left = sf::Vector2f(-speed, 0.f);
    sf::Vector2f right = sf::Vector2f(speed, 0.f);

    std::vector<directions> directionVector;

    InputAction* upAction = gData.InputMgr->CreateInputAction("Up action");
    gData.InputMgr->BindActionToKey(upAction, sf::Keyboard::Key::Up);
    directionVector.push_back(directions{ upAction, up });
    
    InputAction* downAction = gData.InputMgr->CreateInputAction("Down action");
    gData.InputMgr->BindActionToKey(downAction, sf::Keyboard::Key::Down);
    directionVector.push_back(directions{ downAction, down });

    InputAction* leftAction = gData.InputMgr->CreateInputAction("Left action");
    gData.InputMgr->BindActionToKey(leftAction, sf::Keyboard::Key::Left);
    directionVector.push_back(directions{ leftAction, left });

    InputAction* rightAction = gData.InputMgr->CreateInputAction("Right action");
    gData.InputMgr->BindActionToKey(rightAction, sf::Keyboard::Key::Right);
    directionVector.push_back(directions{ rightAction, right });


    const char* texturePath = "../../../MyEngine/Ressources/IsaacSprite.png";
    gData.TextureMgr->LoadTexture(texturePath);

    Scene* scene = gData.SceneMgr->CreateScene("Main");
    {
        Entity* entity = scene->CreateEntity("MainEntity");
        TransformComponent* tc = entity->AddComponent<TransformComponent>();
        {
            tc->SetWorldPosition(startPostion);
        }

        RenderComponent* rc = entity->AddComponent<RenderComponent>();
        {
            SpriteDrawable* bodySprite = rc->AddNewDrawable<SpriteDrawable>("Body", sf::Vector2f(2, 0), 0, sf::Vector2f(1, 1));
            bodySprite->SetVisibility(true);
            bodySprite->SetTexture(texturePath);
            bodySprite->SetAnimation("Body_Vertical");

            SpriteDrawable* headSprite = rc->AddNewDrawable<SpriteDrawable>("Head", sf::Vector2f(-2.5f, -20), 0, sf::Vector2f(1, 1));
            headSprite->SetVisibility(true);
            headSprite->SetTexture(texturePath);
            headSprite->SetAnimation("Head_Down");
        }        

        DynamicColliderComponent* dc = entity->AddComponent<DynamicColliderComponent>();
        {
            dc->SetPosition(tc);
            dc->SetSize(sf::Vector2f(25.f, 33.f), tc);
            dc->SetOffset(sf::Vector2f(0.f, -20.f), tc);
        }

    }
    gData.SceneMgr->SetActiveScene("Main");
    gData.SceneMgr->Start();

    sf::Clock clock;
    clock.restart();

    
    Entity* currentEntity = scene->GetEntity(0);


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

        gData.Update(fDeltaTimeS);

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_RED, "Update");
#ifdef _USE_IMGUI
        ImGui::SFML::Update(window, imGuiTime);
#endif

        // sample
        //ImGui::ShowDemoWindow();
        //Logger::DrawLogger();
        gData.DebugMgr->Draw();
         
        TransformComponent* tc = currentEntity->GetComponent<TransformComponent>();
        {
            sf::Vector2f CurrentPos = tc->GetWorldPosition();
            if (currentPosition != CurrentPos)
            {
                currentPosition = CurrentPos;
            }
        }
        DynamicColliderComponent* dc = currentEntity->GetComponent<DynamicColliderComponent>();
        
        moveEntity(directionVector, currentPosition, tc, dc);

        PROFILER_EVENT_END();

        PROFILER_EVENT_BEGIN(PROFILER_COLOR_GREEN, "Draw");
        window.clear();

        gData.SceneMgr->Draw(window);
        dc->draw(window);

        


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