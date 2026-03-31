#include "Entity.hpp"
#include <emscripten/bind.h>

void Entity::Init()
{
    ;
}

void Entity::Update()
{
    ;
}

void Entity::Draw()
{
   ;
}

// Entity emscripten bindings module
EMSCRIPTEN_BINDINGS(entity_module)
{
    emscripten::class_<Entity>("Entity")
        .property("x", &Entity::x)
        .property("y", &Entity::y)
        .property("angle", &Entity::angle);
}