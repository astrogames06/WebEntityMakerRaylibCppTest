#include "MainScene.hpp"

#include "../Cat/Cat.hpp"

#include <emscripten/bind.h>

void Main::Init()
{
    background_color = WHITE;
}
void CreateEntity()
{
    std::unique_ptr<Cat> new_entity = std::make_unique<Cat>();

    game.AddEntity(std::move(new_entity));
}

void Main::Update()
{
    std::cout << game.GetEntitiesOfType<Cat>().size() << '\n';
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