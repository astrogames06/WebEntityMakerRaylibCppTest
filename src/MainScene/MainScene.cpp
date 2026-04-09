#include "MainScene.hpp"

#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <raygui.h>
#include <raymath.h>

#include "../Utils/raylibextra.h"
#include "../Sprite/Sprite.hpp"
#include "../Systems/SpriteCreateSystem.hpp"
#include "../Systems/CameraControlSystem.hpp"

std::unique_ptr<SpriteCreateSystem> sprite_create_system;
std::unique_ptr<CameraControlSystem> camera_control_system;

void Main::Init()
{
    background_color = WHITE;

    // InitCameraControlSystem();
    camera_control_system = std::make_unique<CameraControlSystem>();
    game.AddSystem(std::move(camera_control_system));

    sprite_create_system = std::make_unique<SpriteCreateSystem>();
    game.AddSystem(std::move(sprite_create_system));
}

EM_JS(bool, js_checkbox_info_on, (), {
    return ShowInfoCheckboxOn();
});
void Main::Update()
{
    // std::cout << game.GetEntitiesOfType<Sprite>().size() << '\n';
    game.show_info = js_checkbox_info_on();
}
void Main::Draw()
{
    // Rectangle world_rect =
    // {
    //     game.GAME_MIN,
    //     game.GAME_MIN,
    //     game.GAME_MAX - game.GAME_MIN,
    //     game.GAME_MAX - game.GAME_MIN
    // };
    // DrawInverseRectInfinite(world_rect, Fade(RED, 0.6f));
}

void Main::DrawUI()
{
    if (game.show_info)
    {
        DrawFPS(20, game.HEIGHT-20);
    }
}
Vector2 GetWindowSize()
{
    return { (float)GetScreenWidth(), (float)GetScreenHeight() };
}
void SetWindowScreenSize(int width, int height)
{
    extern Game game;

    SetWindowSize(width, height);
    game.WIDTH = width;
    game.HEIGHT = height;
}
EMSCRIPTEN_BINDINGS(main_module)
{
    emscripten::function("set_window_size", &SetWindowScreenSize);
    emscripten::function("get_window_size", &GetWindowSize);

    emscripten::value_object<Vector2>("Vector2")
        .field("x", &Vector2::x)
        .field("y", &Vector2::y);
    emscripten::value_object<Vector3>("Vector3")
        .field("x", &Vector3::x)
        .field("y", &Vector3::y)
        .field("z", &Vector3::z);
}