#include "MainScene.hpp"

#include "../Sprite/Sprite.hpp"
#include <emscripten/bind.h>
#include <raygui.h>
#include <raymath.h>
#include "../Utils/raylibextra.h"

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

void Main::Update()
{
    // std::cout << game.GetEntitiesOfType<Sprite>().size() << '\n';
}
void Main::Draw()
{
}

void Main::DrawUI()
{
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