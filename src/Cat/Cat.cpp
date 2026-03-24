#include "Cat.hpp"

void Cat::Init()
{
    texture = LoadTexture("assets/cat.png");
    texture.width /= 5;
    texture.height /= 5;

    x = GetRandomValue(0, game.WIDTH);
    y = GetRandomValue(0, game.HEIGHT);
}

void Cat::Draw()
{
    DrawTexture(texture, x, y, WHITE);
}