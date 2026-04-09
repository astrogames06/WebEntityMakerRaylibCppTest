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
    float s = 1000000.0f;
    float overlap = 1.0f;   // Small overlap to guarantee the four "corner" junctions are filled
                            // (prevents any 1-pixel gaps due to floating-point rasterization)

    // Top (full width, above the rect)
    DrawRectangle(-s, -s, 2*s, r.y + s, color);

    // Bottom (full width, below the rect)
    DrawRectangle(-s, r.y + r.height, 2*s, 2*s, color);

    // Left (extends slightly into top & bottom areas so the top-left and bottom-left corners are guaranteed)
    DrawRectangle(-s, r.y - overlap, r.x + s, r.height + 2*overlap, color);

    // Right (same extension for top-right / bottom-right corners)
    DrawRectangle(r.x + r.width, r.y - overlap, 2*s, r.height + 2*overlap, color);
}