#ifndef SPRITECREATESYSTEM_H
#define SPRITECREATESYSTEM_H

#include "../System/System.hpp"

class SpriteCreateSystem : public System
{
public:
    void Init() override;
    void Update() override;
    void Draw() override;
    // void DrawUI() override;
};

#endif