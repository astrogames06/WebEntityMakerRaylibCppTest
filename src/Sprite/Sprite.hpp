#ifndef SPRITE_H
#define SPRITE_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"
#include <string>

extern Game game;

struct Sprite : public Entity
{
public:
    std::string name;
    std::string texture_name;

    void Init() override;
    void Update() override;
    void Draw() override;
};

#endif