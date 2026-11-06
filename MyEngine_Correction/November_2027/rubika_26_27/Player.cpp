#include "Player.h"

#include <TransformComponent.h>
#include <DynamicColliderComponent.h>
#include <RenderComponent.h>
#include <InputAction.h>
#include <SpriteDrawable.h>

#include <Globals.h>
#include <InputMgr.h>
#include <TextureMgr.h>


Player::Player() : Entity("Player")
{
    
}

Player::~Player()
{
}

void Player::Start()
{
    TransformComponent* tc = this->AddComponent<TransformComponent>();
    {
        tc->SetWorldPosition(startPostion);
    }
    const char* texturePath = "../../../MyEngine/Ressources/IsaacSprite.png";
    gData.TextureMgr->LoadTexture(texturePath);


    sf::Vector2f currentPosition = startPostion;

    float speed = 0.01f;

    sf::Vector2f up = sf::Vector2f(0.f, -speed);
    sf::Vector2f down = sf::Vector2f(0.f, speed);
    sf::Vector2f left = sf::Vector2f(-speed, 0.f);
    sf::Vector2f right = sf::Vector2f(speed, 0.f);

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

    RenderComponent* rc = this->AddComponent<RenderComponent>();
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

    DynamicColliderComponent* dc = this->AddComponent<DynamicColliderComponent>();
    {
        dc->SetPosition(tc);
        dc->SetSize(sf::Vector2f(25.f, 33.f), tc);
        dc->SetOffset(sf::Vector2f(0.f, -20.f), tc);
    }

    Entity::Start();
}

void Player::Update(float deltaTime)
{
    TransformComponent* tc = this->GetComponent<TransformComponent>();
    {
        sf::Vector2f CurrentPos = tc->GetWorldPosition();
        if (currentPosition != CurrentPos)
        {
            currentPosition = CurrentPos;
        }
    }
    DynamicColliderComponent* dc = this->GetComponent<DynamicColliderComponent>();

    moveEntity(directionVector, currentPosition, tc, dc);

    Entity::Update(deltaTime);
}

void Player::Destroy()
{
    Entity::Destroy();
}

void Player::moveEntity(std::vector<Player::directions> directionVector, sf::Vector2f currentPosition, TransformComponent* tc, DynamicColliderComponent* dc)
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