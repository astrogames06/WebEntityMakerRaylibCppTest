#ifndef MAIN_H
#define MAIN_H

#include "../Scene/Scene.hpp"
#include "../Game/Game.hpp"

#include <iostream>

extern Game game;

class Main : public Scene
{
public:
    Entity* selected_entity;

    void Init() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;

    void DrawStats();
};

#endif