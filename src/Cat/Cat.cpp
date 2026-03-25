#include "Cat.hpp"

void Cat::Init()
{
    texture = LoadTexture("assets/cat.png");
    texture.width /= 5;
    texture.height /= 5;
}

void Cat::Draw()
{
    DrawTexturePro(texture,
        {0, 0, (float)texture.width, (float)texture.height},
        {(float)x, (float)y, (float)texture.width, (float)texture.height},
        {(float)texture.width / 2, (float)texture.height / 2},
        angle,
        WHITE
    );
}