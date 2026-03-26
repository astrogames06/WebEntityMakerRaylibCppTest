#ifndef SPRITE_H
#define SPRITE_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"

extern Game game;

struct Sprite : public Entity
{
public:
    void Init() override;
    void Draw() override;
};

#endif