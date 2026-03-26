#include "MainScene.hpp"

#include "../Sprite/Sprite.hpp"
#include <emscripten/bind.h>
#include <raylibextra.h>

bool is_entity_selected;
Entity* selected_entity;

void Main::Init()
{
    background_color = WHITE;
}

void Main::Update()
{
    std::cout << game.GetEntitiesOfType<Sprite>().size() << '\n';

    for (Sprite* sprite : game.GetEntitiesOfType<Sprite>(this))
    {
        // is_entity_selected = false; // Resets first

        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(game.mouse_pos, 
                {(float)sprite->x - sprite->texture.width/2, (float)sprite->y - sprite->texture.height/2,
                (float)sprite->texture.width, (float)sprite->texture.height
            }
            ))
            {
                is_entity_selected = true;
                selected_entity = sprite;
                break; // Makes sure it doesn't check if the other sprites are being clicked
            }
            else
            {
                is_entity_selected = false;
            }
        }
    }

    if (is_entity_selected && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        selected_entity->x = game.mouse_pos.x;
        selected_entity->y = game.mouse_pos.y;
    }

}
void Main::Draw()
{
    if (is_entity_selected)
    {
        DrawRectangleLinesPro(
            {
                (float)selected_entity->x,
                (float)selected_entity->y,
                (float)selected_entity->texture.width,
                (float)selected_entity->texture.height
            },
            { (float)selected_entity->texture.width/2, (float)selected_entity->texture.height/2 },
            selected_entity->angle,
            2.f,
            GREEN
        );
    }
}

void Main::DrawUI()
{

}
Vector2 GetWindowSize()
{
    return { (float)GetScreenWidth(), (float)GetScreenHeight() };
}
void CreateSprite()
{
    std::unique_ptr<Sprite> new_sprite = std::make_unique<Sprite>();

    game.AddEntity(std::move(new_sprite));
}
bool IsSpriteSelected()
{
    return is_entity_selected;
}
Vector3 GetCurrentPositionAngle()
{
    extern Game game;

    return {
        (float)selected_entity->x,
        (float)selected_entity->y,
        (float)selected_entity->angle
    };
}
void SetCurrentPositionAngle(int x, int y, float angle)
{
    extern Game game;

    selected_entity->x = x;
    selected_entity->y = y;
    selected_entity->angle = angle;
}
EMSCRIPTEN_BINDINGS(main_module)
{
    emscripten::function("create_sprite", &CreateSprite);
    emscripten::function("is_sprite_selected", &IsSpriteSelected);
    emscripten::function("set_window_size", &SetWindowSize);
    emscripten::function("get_window_size", &GetWindowSize);

    emscripten::value_object<Vector2>("Vector2")
        .field("x", &Vector2::x)
        .field("y", &Vector2::y);
    emscripten::value_object<Vector3>("Vector3")
        .field("x", &Vector3::x)
        .field("y", &Vector3::y)
        .field("z", &Vector3::z);
    emscripten::function("get_current_position_angle", &GetCurrentPositionAngle);
    emscripten::function("set_current_position_angle", &SetCurrentPositionAngle);
}