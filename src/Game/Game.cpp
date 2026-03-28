#include "Game.hpp"

#include <iostream>
#include <algorithm>
#include <raygui.h>
#include <emscripten/bind.h>

#include "../Utils/null_img.h"
Image null_img;
Texture2D null_tex;

void Game::Init()
{
    camera.target = { 0, 0 };
    camera.offset = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    null_img.data = NULL_IMG_DATA;
    null_img.format = NULL_IMG_FORMAT;
    null_img.width = NULL_IMG_WIDTH;
    null_img.height = NULL_IMG_HEIGHT;
    null_img.mipmaps = 1;
    null_tex = LoadTextureFromImage(null_img);
}
    
void Game::Update()
{
    mouse_pos = GetScreenToWorld2D(GetMousePosition(), camera);

    // Sets the next scene in queue ! MUST DO FIRST !
    if (next_scene)
    {
        current_scene = next_scene;
        std::cout << "SET Current Scene: " << current_scene << '\n';
        std::cout << "SET Next Scene: " << next_scene << '\n';
        if (current_scene)
            current_scene->Init();
        next_scene = nullptr;
    }

    // Adds queued entities first
    while (!pending_entities.empty())
    {
        pending_entities.front()->Init();
        current_scene->entities.push_back(std::move(pending_entities.front()));
        pending_entities.pop();
    }

    if (current_scene != nullptr)
    {
        current_scene->Update();
        for (std::unique_ptr<Entity>& entity : current_scene->entities)
        {
            entity->Update();
        }
        for (std::unique_ptr<System>& system : current_scene->systems)
        {
            system->Update();
        }

        // Remove it if Entity->remove = true;
        current_scene->entities.erase(
            std::remove_if(
                current_scene->entities.begin(),
                current_scene->entities.end(),
                [](const std::unique_ptr<Entity>& e) { return e->remove; }
            ),
            current_scene->entities.end()
        );
    }
}

void Game::Draw()
{
    BeginDrawing();
    if (current_scene != nullptr)
    {
        ClearBackground(current_scene->background_color);
    }
    BeginMode2D(camera);

    if (current_scene != nullptr)
    {
        current_scene->Draw();
        for (std::unique_ptr<Entity>& entity : current_scene->entities)
        {
            entity->Draw();
        }
        for (std::unique_ptr<System>& system : current_scene->systems)
        {
            system->Draw();
        }
    }

    EndMode2D();

    if (current_scene != nullptr)
    {
        current_scene->DrawUI();

        for (std::unique_ptr<System>& system : current_scene->systems)
        {
            system->DrawUI();
        }
    }
    EndDrawing();
}

void Game::Reset()
{

}

void SetupEntity(Entity* entity)
{
    entity->texture = null_tex; // Sets the entities texture temporary until its actual texture is loaded
}
void Game::AddEntity(std::unique_ptr<Entity> entity)
{
    if (current_scene != nullptr)
    {
        SetupEntity(entity.get());
        entity->Init();
        pending_entities.push(std::move(entity));
    }
    else
        std::cout << "! WARNING CURRENT SCENE IS NULLPTR !\n";
}
void Game::AddSystem(std::unique_ptr<System> system)
{
    if (current_scene != nullptr)
    {
        // Adds the system to the systems vector
        current_scene->systems.push_back(std::move(system));

        // Sets the systems drawing z index
        system->drawing_z_index = (int)current_scene->systems.size(); // Sets the default z index

        // Initialises the system 
        for (std::unique_ptr<System>& system : current_scene->systems)
        {
            system->Init();
        }
        // Sorts the systems based on their z index
        std::sort(current_scene->systems.begin(), current_scene->systems.end(),
            [](const std::unique_ptr<System>& a, const std::unique_ptr<System>& b)
            {
                return a->drawing_z_index > b->drawing_z_index;
            });
    }
}

void Game::SetScene(Scene* scene)
{
    if (next_scene != scene)
        next_scene = scene;
}
void Game::SetStartScene(Scene* scene)
{
    current_scene = scene;
    current_scene->Init();
}