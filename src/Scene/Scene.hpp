#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include <raylib.h>

#include "../Entity/Entity.hpp"
#include "../System/System.hpp"

class Scene
{
public:
    Color background_color = WHITE;
    std::vector<std::unique_ptr<Entity>> entities;
    std::vector<std::unique_ptr<System>> systems;

    virtual ~Scene() = default;
    virtual void Init() {}
    virtual void Update() {}
    virtual void Draw() {}
    virtual void DrawUI() {}
};

#endif