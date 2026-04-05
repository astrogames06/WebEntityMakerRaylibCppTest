#include "Sprite.hpp"

#include <raymath.h>
#include <limits>

extern Game game;

void Sprite::Init()
{
    texture = LoadTexture(texture_name.c_str());
    SetTextureFilter(texture, TEXTURE_FILTER_BILINEAR);
}

void Sprite::Update()
{
    angle = Clamp(angle, 0.f, 360.f);

    x = Clamp(x, game.GAME_MIN, game.GAME_MAX);
    y = Clamp(y, game.GAME_MIN, game.GAME_MAX);
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