#include "raylib.h"
#include "raymath.h"
#include "Character.h"

int main()
{
    const int windowWidth{1080};
    const int windowHeight{720};
    InitWindow(windowWidth, windowHeight, "Stephen's Top Down");

    Texture2D map = LoadTexture("nature_tileset/Game_Map.png");
    Vector2 mapPos{0.0, 0.0};
    float mapScale{4.0f};

    Character knight{windowWidth,windowHeight,mapScale};
    

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        mapPos = Vector2Scale(knight.getWorldPos(), -1.0f);

        // draw the map
        DrawTextureEx(map, mapPos, 0.0, 4.0, WHITE);

        knight.tick(GetFrameTime());

        if(knight.getWorldPos().x < 0.0f ||
            knight.getWorldPos().y < 0.0f ||
            knight.getWorldPos().x + windowWidth > map.width * mapScale ||
            knight.getWorldPos().y + windowHeight > map.height * mapScale)
        {
            knight.UndoMovement();
        }

        EndDrawing();
    }
    UnloadTexture(map);

    CloseWindow();
}
