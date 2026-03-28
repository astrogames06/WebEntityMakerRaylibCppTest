#include "MainScene.hpp"

#include "../Sprite/Sprite.hpp"
#include <emscripten/bind.h>
#include <raygui.h>
#include <raylibextra.h>

Font inter_font;

bool is_entity_selected;
Entity* selected_entity;
bool is_moving_camera;

void Main::Init()
{
    background_color = WHITE;

    inter_font = LoadFont("assets/PixelInter.ttf");
    GuiSetFont(inter_font);
    GuiSetStyle(DEFAULT, TEXT_SIZE, 15);
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
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
    // if (IsKeyPressed(KEY_BACKSPACE))
    // {
    //     selected_entity = nullptr;
    // }

    // Makes sure that they are both set false
    if (!is_entity_selected)
    {
        selected_entity = nullptr;
    }
    if (selected_entity == nullptr)
    {
        is_entity_selected = false;
    }

    float wheel = GetMouseWheelMove() * 0.5f;
    if (wheel > 0)
    {
        game.camera.zoom -= 0.5 * GetFrameTime();
    }
    else if (wheel < 0)
    {
        game.camera.zoom += 0.5 * GetFrameTime();
    }
}
void Main::Draw()
{
    if (is_moving_camera)
    {
        const float GRID_CELL_SIZE = 50.f;
        Color grid_color = {200, 200, 200, 200};
        for (int x = 0; x < game.WIDTH/GRID_CELL_SIZE; x++)
        {
            for (int y = 0; y < game.HEIGHT/GRID_CELL_SIZE; y++)
            {
                Rectangle cell_rec = {
                    game.camera.target.x + x * GRID_CELL_SIZE,
                    game.camera.target.y + y * GRID_CELL_SIZE,
                    GRID_CELL_SIZE, GRID_CELL_SIZE
                };

                DrawRectangleLinesEx(cell_rec, 2.f, grid_color);
            }
        }
    }
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

    if (IsKeyDown(KEY_E)) game.camera.zoom -= 1 * GetFrameTime();
}

bool CameraHasBeenMoved()
{
    return (game.camera.target.x != 0 || game.camera.target.y != 0 || game.camera.zoom != 1.0f);
}
void Main::DrawUI()
{
    if (CameraHasBeenMoved())
    {
        std::string camera_str =
        "Camera X: " + std::to_string((int)game.camera.target.x) +
        ", Y: " + std::to_string((int)game.camera.target.y) +
        ", Zoom: " + std::to_string(game.camera.zoom) + "x";

        DrawTextEx(inter_font, camera_str.c_str(), {20, 20}, 20, 1.f, BLACK);
    }

    // if (CameraHasBeenMoved())
    // {
    //     DrawGrid(10, 10);
    // }
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
void DeleteCurrentEntity()
{
    selected_entity->Delete();
    selected_entity = nullptr;
}
void ResetCamera()
{
    game.camera.target = { 0, 0 };
    game.camera.offset = { 0, 0 };
    game.camera.rotation = 0.0f;
    game.camera.zoom = 1.0f;
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
    emscripten::function("delete_current_entity", &DeleteCurrentEntity);
    emscripten::function("camera_has_been_moved", &CameraHasBeenMoved);
    emscripten::function("reset_camera", &ResetCamera);
}