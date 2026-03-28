#ifndef SYSTEM_H
#define SYSTEM_H

class System
{
public:
    int drawing_z_index;
    virtual ~System() = default;
    virtual void Init() {}
    virtual void Update() {}
    virtual void Draw() {}
    virtual void DrawUI() {}
};

#endif