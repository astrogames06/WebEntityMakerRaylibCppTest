#include "MainScene.hpp"

#include "../Cat/Cat.hpp"
#include <emscripten/bind.h>

bool is_entity_selected;

void Main::Init()
{
    background_color = WHITE;
}


void Main::Update()
{
    std::cout << game.GetEntitiesOfType<Cat>().size() << '\n';

    for (Entity* entity : game.GetEntitiesOfType<Entity>(this))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(game.mouse_pos, 
                {(float)entity->x, (float)entity->y, (float)entity->texture.width, (float)entity->texture.height}
            ))
            {
                is_entity_selected = true;
                selected_entity = entity;
            }
            else
            {
                is_entity_selected = false;
            }
        }
    }

    if (is_entity_selected && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        selected_entity->x = game.mouse_pos.x - selected_entity->texture.width/2;
        selected_entity->y = game.mouse_pos.y - selected_entity->texture.height/2;
    }
}
void Main::Draw()
{
    if (is_entity_selected)
    {
        DrawRectangleLines(selected_entity->x, selected_entity->y, selected_entity->texture.width, selected_entity->texture.height, GREEN);
    }
}

void Main::DrawUI()
{

}

void CreateEntity()
{
    std::unique_ptr<Cat> new_entity = std::make_unique<Cat>();

    game.AddEntity(std::move(new_entity));
}
bool IsEntitySelected()
{
    return is_entity_selected;
}
EMSCRIPTEN_BINDINGS(main_module)
{
    emscripten::function("create_entity", &CreateEntity);
    emscripten::function("is_entity_selected", &IsEntitySelected);
}