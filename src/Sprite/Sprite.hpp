#ifndef SPRITE_H
#define SPRITE_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"

extern Game game;

enum SpriteType
{
    MONKEY,
    PIG
};
struct Sprite : public Entity
{
public:
    SpriteType type;
    void Init() override;
    void Draw() override;
};

#endif