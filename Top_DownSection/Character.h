#include "raylib.h"

class Character
{
public:
    Character(float inWinwidth, float inWinHeight,float Scale);
    Vector2 getWorldPos() { return worldPos; }
    void setScreenPos(int winWidth, int winHeight);
    void tick(float deltaTime);
    void UndoMovement();

private:
    Texture2D texture{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D idle{LoadTexture("characters/knight_idle_spritesheet.png")};
    Texture2D run{LoadTexture("characters/knight_run_spritesheet.png")};
    Vector2 Size;
    Vector2 screenPos;
    Vector2 worldPos;
    Vector2 LastFramePos;
    // 1 : facing right, -1 : facing left
    float rightLeft{1.f};
    // animation variables
    float runningTime{};
    int frame{};
    const int maxFrames{6};
    const float updateTime{1.f / 12.f};
    const float speed{10.f};
};