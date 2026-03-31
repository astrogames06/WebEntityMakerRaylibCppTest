#ifndef SPRITE_H
#define SPRITE_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"
#include <string>

extern Game game;

enum SpriteType
{
    MONKEY,
    PIG
};
struct Sprite : public Entity
{
public:
    std::string name;
    SpriteType type;
    void Init() override;
    void Draw() override;
};

#endif