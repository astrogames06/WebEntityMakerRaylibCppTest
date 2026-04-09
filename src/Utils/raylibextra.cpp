#include "raylibextra.h"

#include <math.h>

void DrawRectangleLinesPro(Rectangle rect, Vector2 origin, float rotation, float thickness, Color color)
{
    // Convert rotation to radians
    float rad = rotation * DEG2RAD;

    // Local corners relative to origin
    Vector2 corners[4] = {
        { -origin.x,              -origin.y },
        { rect.width - origin.x,  -origin.y },
        { rect.width - origin.x,  rect.height - origin.y },
        { -origin.x,              rect.height - origin.y }
    };

    // Rotate + translate corners
    for (int i = 0; i < 4; i++)
    {
        float x = corners[i].x;
        float y = corners[i].y;

        float rx = x * cosf(rad) - y * sinf(rad);
        float ry = x * sinf(rad) + y * cosf(rad);

        corners[i].x = rect.x + rx;
        corners[i].y = rect.y + ry;
    }

    // Draw edges
    for (int i = 0; i < 4; i++)
    {
        Vector2 start = corners[i];
        Vector2 end   = corners[(i + 1) % 4];

        DrawLineEx(start, end, thickness, color);
    }
}

void DrawInverseRectInfinite(Rectangle r, Color color)
{
    float size = 1000000.0f; // "infinite"

    // top
    DrawRectangle(-size, -size, size * 2, r.y + size, color);

    // bottom
    DrawRectangle(-size, r.y + r.height, size * 2, size * 2, color);

    // left
    DrawRectangle(-size, r.y, r.x + size, r.height, color);

    // right
    DrawRectangle(r.x + r.width, r.y, size * 2, r.height, color);
}