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

EM_JS(void, js_add_entity_to_list, (const char* name), {
    AddEntityToList(UTF8ToString(name)); // converts the name to js string
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

Sprite* GetCurrentEntity()
{
    return selected_sprite;
}
Sprite* CreateSprite()
{
    auto sprites_v = game.GetEntitiesOfType<Sprite>();

    int name_num_addition = 0;

    std::string new_sprite_name = "Sprite " + std::to_string(name_num_addition); // Default name
    bool sprite_name_exists = std::find_if(sprites_v.begin(), sprites_v.end(),
        [&](Sprite* s)
        {
            return s->name == new_sprite_name;
        }) != sprites_v.end();

    while (sprite_name_exists)
    {
        name_num_addition++;
        new_sprite_name = "Sprite " + std::to_string(name_num_addition);

        // Rechecks
        sprite_name_exists = std::find_if(sprites_v.begin(), sprites_v.end(),
            [&](Sprite* s)
            {
                return s->name == new_sprite_name;
            }) != sprites_v.end();
    }

    std::unique_ptr<Sprite> new_sprite = std::make_unique<Sprite>();
    new_sprite->name = new_sprite_name;

    js_add_entity_to_list(new_sprite->name.c_str()); // Uploads the name to the entity list
    game.AddEntity(std::move(new_sprite)); // Adds the actual sprite

    return new_sprite.get();
}
Entity* GetSpriteByIndex(int index)
{
    if (game.GetEntitiesOfType<Sprite>()[index])
    {
        return game.GetEntitiesOfType<Sprite>()[index];
    }
    return nullptr; // Returns nothing if it doesn't exist
}
int GetEntityIndex(Entity* entity)
{
    auto sprites_v = game.GetEntitiesOfType<Sprite>();
    int sprite_i = std::find(sprites_v.begin(), sprites_v.end(), entity) - sprites_v.begin();

    return sprite_i;
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
    emscripten::function("create_sprite", &CreateSprite, emscripten::allow_raw_pointers());
    emscripten::function("is_sprite_selected", &IsSpriteSelected);
    emscripten::function("selected_sprite_index", &SelectedSpriteIndex);
    emscripten::function("get_entity_index", &GetEntityIndex, emscripten::allow_raw_pointers());
    emscripten::function("get_sprite_by_index", &GetSpriteByIndex, emscripten::allow_raw_pointers());

    emscripten::function("on_blur", &OnBlur);
    emscripten::function("delete_current_entity", &DeleteCurrentEntity);
    emscripten::function("delete_indexed_entity", &DeleteIndexedEntity);

    emscripten::function("get_current_entity", &GetCurrentEntity, emscripten::allow_raw_pointers());

    emscripten::class_<Sprite, emscripten::base<Entity>>("Sprite")
        .property("name", &Sprite::name);
}