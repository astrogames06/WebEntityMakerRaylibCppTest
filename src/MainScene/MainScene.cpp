#include "MainScene.hpp"

#include <emscripten/bind.h>

void Main::Init()
{
    background_color = SKYBLUE;
}
void CreateEntity()
{
    std::unique_ptr<Entity> new_entity = std::make_unique<Entity>();
    new_entity->texture = LoadTexture("assets/cat.png");
    new_entity->x = GetRandomValue(0, game.WIDTH);
    new_entity->y = GetRandomValue(0, game.HEIGHT);

    game.AddEntity(std::move(new_entity));
}

void Main::Update()
{

}

void Main::Draw()
{
}


void Main::DrawUI()
{

}

EMSCRIPTEN_BINDINGS(main_module)
{
    emscripten::function("create_entity", &CreateEntity);
}