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
#include "../Utils/Utils.hpp"

bool is_sprite_selected = false;
Sprite* selected_sprite = nullptr;

namespace Scenes
{
    extern std::unique_ptr<Main> main_scene;
};

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
void DeleteCurrentEntity();

bool did_click_on_sprite = false;
void SpriteCreateSystem::Update()
{
    // std::cout << "IsSpriteSelected: " << IsSpriteSelected() << '\n';
    if (IsKeyPressed(KEY_O))
    {
        js_alert(selected_sprite->name.c_str());
    }
    for (Sprite* sprite : game.GetEntitiesOfType<Sprite>(Scenes::main_scene.get()))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            if (CheckCollisionPointRec(game.mouse_pos, 
                {(float)sprite->x - sprite->texture.width/2, (float)sprite->y - sprite->texture.height/2,
                (float)sprite->texture.width, (float)sprite->texture.height
            }
            ))
            {
                did_click_on_sprite = true; // Checks if any sprite was clicked
                is_sprite_selected = true;
                selected_sprite = sprite;
                break; // Makes sure it doesn't check if the other sprites are being clicked
            }
            else
            {
                did_click_on_sprite = false; // This only happens if NO sprites where clicked because of the 'break'
            }
        }
    }
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && !did_click_on_sprite) // Makes sure to de-select the sprite if you click not on it
    {
        selected_sprite = nullptr;
        is_sprite_selected = false;
    }

    if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) && is_sprite_selected && did_click_on_sprite) // Checks if you are dragging it
    {
        selected_sprite->x = game.mouse_pos.x;
        selected_sprite->y = game.mouse_pos.y;
    }

    // Deletes entity on when click backspace/delete
    if (IsKeyPressed(KEY_BACKSPACE) || IsKeyPressed(KEY_DELETE))
    {
        DeleteCurrentEntity();
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
void SetSelectedEntity(Sprite* sprite)
{
    if (!sprite)
    {
        std::cerr << "Trying To Selected NULL Entity\n"; // If sprite doesn't exist we will print error
        return;
    }
    // Otherwise we set the selected sprite
    selected_sprite = sprite;
    is_sprite_selected = true;
}
Sprite* CreateSprite(std::string texture_name)
{
    const auto& sprites_v = game.GetEntitiesOfType<Sprite>();

    int name_num_addition = 0;

    std::string new_sprite_name = "Sprite " + std::to_string(name_num_addition); // Sets Default name
    bool sprite_name_exists = std::find_if(sprites_v.begin(), sprites_v.end(),
        [&](Sprite* s)
        {
            return remove_whitespace_str(s->name) == remove_whitespace_str(new_sprite_name);
        }) != sprites_v.end();

    // Checks if the sprite name already exists. If so it keeps updating till its name is unique
    while (sprite_name_exists)
    {
        name_num_addition++;
        new_sprite_name = "Sprite " + std::to_string(name_num_addition);

        // Rechecks
        sprite_name_exists = std::find_if(sprites_v.begin(), sprites_v.end(),
            [&](Sprite* s)
            {
                return remove_whitespace_str(s->name) == remove_whitespace_str(new_sprite_name);
            }) != sprites_v.end();
    }

    std::unique_ptr<Sprite> new_sprite = std::make_unique<Sprite>();
    new_sprite->texture_name = texture_name;
    new_sprite->name = new_sprite_name;

    Sprite* raw_sprite_ptr = new_sprite.get(); // Saves the sprites raw pointer

    js_add_entity_to_list(new_sprite->name.c_str()); // Uploads the name to the entity list
    game.AddEntity(std::move(new_sprite)); // Adds the actual sprite

    return raw_sprite_ptr;
}
std::string CreateSpriteDefaultName()
{
    const auto& sprites_v = game.GetEntitiesOfType<Sprite>();

    int name_num_addition = 0;

    std::string new_sprite_name = "Sprite " + std::to_string(name_num_addition); // Sets Default name
    bool sprite_name_exists = std::find_if(sprites_v.begin(), sprites_v.end(),
        [&](Sprite* s)
        {
            return remove_whitespace_str(s->name) == remove_whitespace_str(new_sprite_name);
        }) != sprites_v.end();

    // Checks if the sprite name already exists. If so it keeps updating till its name is unique
    while (sprite_name_exists)
    {
        name_num_addition++;
        new_sprite_name = "Sprite " + std::to_string(name_num_addition);

        // Rechecks
        sprite_name_exists = std::find_if(sprites_v.begin(), sprites_v.end(),
            [&](Sprite* s)
            {
                return remove_whitespace_str(s->name) == remove_whitespace_str(new_sprite_name);
            }) != sprites_v.end();
    }

    return new_sprite_name;
}
Entity* GetSpriteByIndex(int index)
{
    const auto& sprites_v = game.GetEntitiesOfType<Sprite>();
    if (index >= 0 && index < sprites_v.size() && sprites_v[index] != nullptr)
    {
        return sprites_v[index];
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
// Gets the sprite name
std::string GetSpriteName(Sprite* sprite)
{
    return sprite->name;
}
// Sets sprite name correctly for the javascript
std::string SetSpriteName(Sprite* sprite, std::string name)
{
    const auto sprites_v = game.GetEntitiesOfType<Sprite>();
    std::string original_name = name;
    int suffix = 1;

    auto exists = [&](const std::string& n)
    {
        return std::find_if(sprites_v.begin(), sprites_v.end(),
            [&](Sprite* s)
            {
                return s != sprite && remove_whitespace_str(s->name) == remove_whitespace_str(n);
            }) != sprites_v.end();
    };

    if (remove_whitespace_str(name).empty())
    {
        name = CreateSpriteDefaultName();
    }

    while (exists(name))
    {
        name = remove_whitespace_str_from_back(original_name) + " " + std::to_string(suffix++);
    }

    sprite->name = name;
    return name;
}
void DuplicateSprite(Sprite* sprite)
{
    Sprite* new_sprite = CreateSprite(sprite->texture_name); // Makes new sprite
    
    // Copies all sprite info
    new_sprite->texture = sprite->texture;
    new_sprite->angle = sprite->angle;

    // Slightly offsets its x and y position
    new_sprite->x = sprite->x - sprite->texture.width / 2; 
    new_sprite->y = sprite->y + sprite->texture.height / 2;

    // This makes sure its name is a variation of the its duplicating from
    new_sprite->name = SetSpriteName(new_sprite, sprite->name);
}
void OnBlur()
{
    is_sprite_selected = false;
}

EMSCRIPTEN_BINDINGS(sprite_creator_module)
{
    emscripten::function("create_sprite", &CreateSprite, emscripten::allow_raw_pointers());
    emscripten::function("set_selected_entity", &SetSelectedEntity, emscripten::allow_raw_pointers());
    emscripten::function("duplicate_sprite", &DuplicateSprite, emscripten::allow_raw_pointers());
    emscripten::function("is_sprite_selected", &IsSpriteSelected);
    emscripten::function("selected_sprite_index", &SelectedSpriteIndex);

    emscripten::function("get_sprite_name", &GetSpriteName, emscripten::allow_raw_pointers());
    emscripten::function("set_sprite_name", &SetSpriteName, emscripten::allow_raw_pointers());

    emscripten::function("get_entity_index", &GetEntityIndex, emscripten::allow_raw_pointers());
    emscripten::function("get_sprite_by_index", &GetSpriteByIndex, emscripten::allow_raw_pointers());


    emscripten::function("on_blur", &OnBlur);
    emscripten::function("delete_current_entity", &DeleteCurrentEntity);
    emscripten::function("delete_indexed_entity", &DeleteIndexedEntity);

    emscripten::function("get_current_entity", &GetCurrentEntity, emscripten::allow_raw_pointers());

    emscripten::class_<Sprite, emscripten::base<Entity>>("Sprite");
}