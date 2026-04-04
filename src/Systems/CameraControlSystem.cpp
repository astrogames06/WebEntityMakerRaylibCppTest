#include "CameraControlSystem.hpp"

#include <emscripten/emscripten.h>
#include <raylib.h>
#include <raymath.h>
#include <emscripten/bind.h>
#include <format>

#include "../Game/Game.hpp"
#include "SpriteCreateSystem.hpp"

extern Game game;

bool is_moving_camera;

Font inter_font;
void CameraControlSystem::Init()
{
    inter_font = LoadFont("assets/PixelInter.ttf");
}

void CameraControlSystem::Update()
{
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

    float wheel = GetMouseWheelMove();
    if (wheel != 0.0f)
    {
        game.camera.zoom += wheel * 0.1f; // adjust sensitivity
    }

    game.camera.zoom = std::round(game.camera.zoom * 10.0f) / 10.0f; // Rounds the zoom to one decimal place
    game.camera.zoom = Clamp(game.camera.zoom, 0.5f, 2.0f); // Makes sure you can't zoom too in/out

    // Clamps it to world position
    game.camera.target.x = Clamp(game.camera.target.x, game.GAME_MIN, game.GAME_MAX);
    game.camera.target.y = Clamp(game.camera.target.y, game.GAME_MIN, game.GAME_MAX);
}

EM_JS(bool, js_show_grid_checkbox_on, (), {
    return ShowGridCheckboxOn();
});
void CameraControlSystem::Draw()
{
    // // Checks if sprite is being moved
    // bool is_editing_sprite = (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && is_sprite_selected && did_click_on_sprite);
    // // Draws grid
    if (js_show_grid_checkbox_on())
    {
        const float GRID_CELL_SIZE = 25.f;
        Color grid_color = {212, 212, 212, 255};

        float worldWidth  = game.WIDTH  / game.camera.zoom;
        float worldHeight = game.HEIGHT / game.camera.zoom;

        int startX = (int)((game.camera.target.x - worldWidth) / GRID_CELL_SIZE);
        int startY = (int)((game.camera.target.y - worldHeight) / GRID_CELL_SIZE);

        int endX = (int)((game.camera.target.x + worldWidth) / GRID_CELL_SIZE) + 1;
        int endY = (int)((game.camera.target.y + worldHeight) / GRID_CELL_SIZE) + 1;

        for (int x = startX; x < endX; x++)
        {
            for (int y = startY; y < endY; y++)
            {
                Rectangle cell_rec = {
                    x * GRID_CELL_SIZE,
                    y * GRID_CELL_SIZE,
                    GRID_CELL_SIZE,
                    GRID_CELL_SIZE
                };

                DrawRectangleLinesEx(cell_rec, 2.f / game.camera.zoom, grid_color);
            }
        }
    }
}

bool CameraHasBeenMoved();
void CameraControlSystem::DrawUI()
{
    if (CameraHasBeenMoved())
    {
        std::string camera_str =
        "Camera X: " + std::to_string((int)game.camera.target.x) +
        ", Y: " + std::to_string((int)game.camera.target.y) +
        ", Zoom: " + std::format("{:.1f}", game.camera.zoom) + "x";

        DrawTextEx(inter_font, camera_str.c_str(), {20, 20}, 20, 1.f, BLACK);
    }
}

// All Defined Functions
void ResetCamera()
{
    game.camera.target = { 0, 0 };
    game.camera.offset = { 0, 0 };
    game.camera.rotation = 0.0f;
    game.camera.zoom = 1.0f;
}
bool CameraHasBeenMoved()
{
    return (game.camera.target.x != 0 || game.camera.target.y != 0 || game.camera.zoom != 1.0f);
}
void MoveCameraToEntity(Entity* entity)
{
    game.camera.target = {
        (float)entity->x - game.WIDTH / 2,
        (float)entity->y - game.HEIGHT / 2
    };
}

EMSCRIPTEN_BINDINGS(camera_control_module)
{
    emscripten::function("reset_camera", &ResetCamera);
    emscripten::function("camera_has_been_moved", &CameraHasBeenMoved);
    emscripten::function("move_camera_to_entity", &MoveCameraToEntity, emscripten::allow_raw_pointers());
}