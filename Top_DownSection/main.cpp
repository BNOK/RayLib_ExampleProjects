#include "raylib.h"
#include "raymath.h"
#include <cstdio>
#include <string>

// --------------- Window Struct ---------------
const Vector2 Win_Settings{960, 640};

class Character{
private:
    Vector2 ScreenPos{
        (float)Win_Settings.x / 2.0f - 4.0f * (0.5f * (float)this->Current.width / 6.0f),
        (float)Win_Settings.y / 2.0f - 4.0f * (0.5f * (float)this->Current.height)
    };
    Vector2 WorldPos{0.0f,0.0f};

    // -------- Animation Settings -------------
    int Frame{};
    int MaxFrame{};
    float RunningTime{};
    float UpdateTime{};
    float Flip{};

public:
    Texture2D Idle;
    Texture2D Run;
    Texture2D Current;

    Vector2 getWorldPos() { return WorldPos; }
    Vector2 getScreenPos(){return ScreenPos;}
    float getFlip(){return Flip;}

    void setScreenPos(float inWidth, float inHeight);
    void setFlip(float inFlip){this->Flip = inFlip;}
    void setUpdateTime(float inUpdateTime){ this->UpdateTime = inUpdateTime;}
    void setMaxFrame(int inMaxFrame){this->MaxFrame = inMaxFrame;}
    void tick(float deltaTime);

};

// --------------- Forward Declarations ----------------
Vector2 FindDirection(Vector2 Position, Vector2 WindowSettings, int Scale, float Speed);
int FindAnimationFrame(int Frame, int MaxFrame, float &RunningTime, float UpdateTime, float deltatime);

// =================== Main function ========================
int main()
{
    InitWindow(Win_Settings.x, Win_Settings.y, "Game Window");

    // ---------------------- general settings ----------------------
    int Speed{5};

    // Map Settings
    Texture2D Map = LoadTexture("nature_tileset/Game_Map.png");
    int MapScale = 4.0f;
    Vector2 MapPosition{0.0f,0.0f};
     // ------------------------- Animation Settings ----------------------------------
    int Frame = 0;
    int MaxFrame = 6;
    float RunningTime = 0;
    float UpdateTime = 12;

    // ====== Character instance =======
    Character Knight;
    Knight.Idle = LoadTexture("characters/knight_idle_spritesheet.png");
    Knight.Run = LoadTexture("characters/knight_run_spritesheet.png");
    Knight.Current = Knight.Idle;
    Knight.setFlip(1.0f);
    Knight.setUpdateTime(UpdateTime);
    Knight.setMaxFrame(MaxFrame);
    // ======================================
    Vector2 Screen_Pos{
        (float)Win_Settings.x / 2.0f - 4.0f * (0.5f * (float)Knight.Idle.width / 6.0f),
        (float)Win_Settings.y / 2.0f - 4.0f * (0.5f * (float)Knight.Idle.height)
        };
    Knight.setScreenPos(Screen_Pos.x,Screen_Pos.y);

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        // ------- drawing map image -------
        MapPosition = Knight.getWorldPos();
        DrawTextureEx(Map, MapPosition, 0, MapScale, WHITE);

        // ------- Drawing the character ----------
        Knight.tick(GetFrameTime());

        EndDrawing();
    }
    UnloadTexture(Map);
    UnloadTexture(Knight.Idle);
    UnloadTexture(Knight.Run);
    UnloadTexture(Knight.Current);
    CloseWindow();
}

Vector2 FindDirection(Vector2 Position, Vector2 WindowSettings, int Scale, float Speed)
{
    Vector2 Direction{0, 0};
    float Pad = 50;
    if (IsKeyDown(KEY_A) && Position.x < 0)
    {
        // DrawText("Left",100,100,50,RED);
        Direction.x -= 1.0;
    }
    if (IsKeyDown(KEY_D) && Position.x - Pad >= -(WindowSettings.x * (Scale - 1)))
    {
        // DrawText("Right",20,20,20,RED);
        Direction.x += 1.0;
    }
    if (IsKeyDown(KEY_Z) && Position.y < 0)
    {
        // DrawText("up",20,20,20,RED);
        Direction.y -= 1.0;
    }
    if (IsKeyDown(KEY_S) && Position.y - Pad > -(WindowSettings.y * (Scale - 1)))
    {
        // DrawText("down",20,20,20,RED);
        Direction.y += 1.0;
    }
    Direction = Vector2Normalize(Direction);
    Direction = Vector2Scale(Direction, Speed);
    return Direction;
}

int FindAnimationFrame(int Frame, int MaxFrame, float &RunningTime, float UpdateTime, float deltatime)
{

    RunningTime += deltatime;
    if (RunningTime > 1 / UpdateTime)
    {
        RunningTime = 0;
        Frame++;
        if (Frame > MaxFrame)
        {
            Frame = 0;
        }
    }
    return Frame;
}

void Character::setScreenPos(float winWidth, float winHeight)
{
    ScreenPos = {(float)winWidth / 2.0f - 4.0f * (0.5f * (float)Current.width / 6.0f),
                 (float)winHeight / 2.0f - 4.0f * (0.5f * (float)Current.height)
    };
}

void Character::tick(float deltaTime){


    Vector2 FinalDirection = FindDirection(WorldPos,Win_Settings,4.0,5.0);
    if (Vector2Length(FinalDirection) != 0.0f)
    {
            WorldPos = Vector2Add(WorldPos, FinalDirection);
            // FinalDirection.x < 0.0f ? rightleft = -1.0f : rightleft = 1.0f;
            WorldPos.x < 0.0f ? Flip = -1.0f : Flip = 1.0f;
            Current = Run;
    }
    else
    {
            Current = Idle;
    }

    Frame = FindAnimationFrame(Frame,MaxFrame,RunningTime,UpdateTime,deltaTime);
     Rectangle Source{
            Frame * (float)Current.width / MaxFrame,
            0.0f,
            getFlip() * (float)(Current.width / 6.0f),
            (float)Current.height};
        Rectangle Dest{
            getScreenPos().x, getScreenPos().y,
            4.0f * Current.width / 6.0f,
            4.0f * Current.height};

        DrawTexturePro(Current, Source, Dest, Vector2{}, 0.0f, WHITE);
}







