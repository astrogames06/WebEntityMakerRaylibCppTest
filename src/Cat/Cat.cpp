#include "Cat.hpp"

void Cat::Init()
{
    texture = LoadTexture("assets/cat.png");
    texture.width /= 5;
    texture.height /= 5;
}

void Cat::Draw()
{
    DrawTexture(texture, x, y, WHITE);
}