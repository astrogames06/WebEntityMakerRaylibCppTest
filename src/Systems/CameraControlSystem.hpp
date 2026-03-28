#ifndef CAMERACONTROLSYSTEM_H
#define CAMERACONTROLSYSTEM_H

#include "../System/System.hpp"

class CameraControlSystem : public System
{
public:
    void Init() override;
    void Update() override;
    void Draw() override;
    void DrawUI() override;
};

#endif