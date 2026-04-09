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

    // Keeps it in the world bounding box
    x = Clamp(x, game.GAME_MIN + texture.width/2, game.GAME_MAX - texture.width/2);
    y = Clamp(y, game.GAME_MIN + texture.height/2, game.GAME_MAX - texture.height/2);
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