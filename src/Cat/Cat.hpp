#ifndef CAT_H
#define CAT_H

#include "../Game/Game.hpp"
#include "../Entity/Entity.hpp"

extern Game game;

struct Cat : public Entity
{
public:
    void Init() override;
    void Draw() override;
};

#endif