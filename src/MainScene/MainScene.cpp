#include "MainScene.hpp"

#include "../Sprite/Sprite.hpp"
#include <emscripten/bind.h>
#include <raygui.h>
#include <raylibextra.h>

bool is_entity_selected;
Entity* selected_entity;
bool is_moving_camera;

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

    // Moving camera
    is_moving_camera = false;
    if (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP))
    {
        game.camera.target.y -= 500 * GetFrameTime();
        is_moving_camera = true;
    }
    if (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT))
    {
        game.camera.target.x -= 500 * GetFrameTime();
        is_moving_camera = true;
    }
    if (IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN))
    {
        game.camera.target.y += 500 * GetFrameTime();
        is_moving_camera = true;
    }
    if (IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT))
    {
        game.camera.target.x += 500 * GetFrameTime();
        is_moving_camera = true;
    }

    // Deleting entites
    if (IsKeyPressed(KEY_BACKSPACE))
    {
        selected_entity->Delete();
    }

    // Makes sure that they are both set false
    if (!is_entity_selected)
    {
        selected_entity = nullptr;
    }
    if (selected_entity == nullptr)
    {
        is_entity_selected = false;
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
    if (game.camera.target.x != 0 || game.camera.target.y != 0)
    {
        if (GuiButton({20, 20, 100, 40}, "Reset Camera"))
        {
            game.camera.target = {0,0};
        }
    }
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
void OnBlur()
{
    is_entity_selected = false;
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
    emscripten::function("on_blur", &OnBlur);
}