#include "Sprite.hpp"

#include <raymath.h>

void Sprite::Init()
{
    texture = LoadTexture("assets/cat.png");
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
}

void Sprite::Update()
{
    angle = Clamp(angle, 0.f, 360.f);
}

void Sprite::Draw()
{
    DrawTexturePro(texture,
        {0, 0, (float)texture.width, (float)texture.height},
        {(float)x, (float)y, (float)texture.width, (float)texture.height},
        {(float)texture.width / 2, (float)texture.height / 2},
        angle,
        WHITE
    );
}