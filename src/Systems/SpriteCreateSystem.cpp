#include "SpriteCreateSystem.hpp"

#include <raylib.h>
#include <emscripten/emscripten.h>
#include <emscripten/bind.h>
#include <math.h>
#include <algorithm>

#include "../Utils/raylibextra.h"
#include "../MainScene/MainScene.hpp"
#include "../Entity/Entity.hpp"
#include "../Sprite/Sprite.hpp"

bool is_sprite_selected = false;
Sprite* selected_sprite = nullptr;

namespace Scenes
{
    extern std::unique_ptr<Main> main_scene;
}

EM_JS(bool, js_add_entity_to_list, (), {
    return AddEntityToList();
});
EM_JS(void, js_remove_entity_from_list, (int index), {
    RemoveEntityFromList(index);
});

void SpriteCreateSystem::Init()
{
    
}
bool IsSpriteSelected()
{
    return is_sprite_selected;
}
int SelectedSpriteIndex()
{
    const std::vector<Sprite*>& sprites_v = game.GetEntitiesOfType<Sprite>();
    int sprite_index = std::find(sprites_v.begin(), sprites_v.end(), selected_sprite) - sprites_v.begin();

    return sprite_index;
}
void SpriteCreateSystem::Update()
{
    std::cout << "IsSpriteSelected: " << IsSpriteSelected() << '\n';
    for (Sprite* sprite : game.GetEntitiesOfType<Sprite>(Scenes::main_scene.get()))
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
                is_sprite_selected = true;
                selected_sprite = sprite;
                break; // Makes sure it doesn't check if the other sprites are being clicked
            }
            else
            {
                is_sprite_selected = false;
            }
        }
    }

    if (is_sprite_selected && IsMouseButtonDown(MOUSE_BUTTON_LEFT))
    {
        selected_sprite->x = game.mouse_pos.x;
        selected_sprite->y = game.mouse_pos.y;
    }

    // Makes sure that they are both set false
    if (!is_sprite_selected)
    {
        selected_sprite = nullptr;
    }
    if (selected_sprite == nullptr)
    {
        is_sprite_selected = false;
    }
}
void SpriteCreateSystem::Draw()
{
    // Draws box around entity
    if (is_sprite_selected)
    {
        DrawRectangleLinesPro(
            {
                (float)selected_sprite->x,
                (float)selected_sprite->y,
                (float)selected_sprite->texture.width,
                (float)selected_sprite->texture.height
            },
            { (float)selected_sprite->texture.width/2, (float)selected_sprite->texture.height/2 },
            selected_sprite->angle,
            2.f,
            GREEN
        );
    }
}

// All Defined Functions 

Vector3 GetCurrentPositionAngle()
{
    extern Game game;

    return {
        (float)selected_sprite->x,
        (float)selected_sprite->y,
        (float)selected_sprite->angle
    };
}
void SetCurrentPositionAngle(int x, int y, float angle)
{
    extern Game game;

    selected_sprite->x = x;
    selected_sprite->y = y;
    selected_sprite->angle = angle;
}
void CreateSprite()
{
    if (js_add_entity_to_list()) // Add to visual list and checks if its valid
    {
        std::unique_ptr<Sprite> new_sprite = std::make_unique<Sprite>();
        new_sprite->type = SpriteType::MONKEY;

        game.AddEntity(std::move(new_sprite)); // Adds the actual sprite
    }
}
void DeleteCurrentEntity()
{
    // Removes from visual list
    const std::vector<Sprite*>& sprites_v = game.GetEntitiesOfType<Sprite>();
    int sprite_index = std::find(sprites_v.begin(), sprites_v.end(), selected_sprite) - sprites_v.begin();
    js_remove_entity_from_list(sprite_index);

    // Deletes the actual sprite
    selected_sprite->Delete();
    selected_sprite = nullptr;
}
void DeleteIndexedEntity(int sprite_index)
{
    if (game.GetEntitiesOfType<Sprite>()[sprite_index] == selected_sprite) // Checks if entity is the selected one
    {
        DeleteCurrentEntity();
    }
    else
    {
        game.GetEntitiesOfType<Sprite>()[sprite_index]->Delete(); // Otherwise it removes it as normal
        js_remove_entity_from_list(sprite_index);
    }
}
void OnBlur()
{
    is_sprite_selected = false;
}

EMSCRIPTEN_BINDINGS(sprite_creator_module)
{
    emscripten::function("create_sprite", &CreateSprite);
    emscripten::function("is_sprite_selected", &IsSpriteSelected);
    emscripten::function("selected_sprite_index", &SelectedSpriteIndex);

    emscripten::function("get_current_position_angle", &GetCurrentPositionAngle);
    emscripten::function("set_current_position_angle", &SetCurrentPositionAngle);

    emscripten::function("on_blur", &OnBlur);
    emscripten::function("delete_current_entity", &DeleteCurrentEntity);
    emscripten::function("delete_indexed_entity", &DeleteIndexedEntity);
}