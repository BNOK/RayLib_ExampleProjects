#include "raylib.h"
#include "raymath.h"
#include <cstdio>
#include <string>

// --------------- Window Struct ---------------
const Vector2 Win_Settings{960, 640};
class Character;
class Map;

// --------------- Forward Declarations ----------------
Vector2 FindDirection(Vector2 Position, Vector2 WindowSettings, int Scale, float Speed);
int AnimateCharacter(int Frame, int MaxFrame, float &RunningTime, float UpdateTime, float deltatime);

// =================== Main function ========================
int main()
{
    
    InitWindow(Win_Settings.x, Win_Settings.y, "Game Window");

    // ---------------------- general settings ----------------------
    int Speed{10};

    // Map Settings
    MapSettings Map{
        LoadTexture("nature_tileset/Game_Map.png"),
        {0, 0},
        0,
        4,
        WHITE};
    // ------------------------ Character Settings ----------------------------------
    Texture2D Knight_Run = LoadTexture("characters/knight_run_spritesheet.png");
    Texture2D Knight_Idle = LoadTexture("characters/knight_idle_spritesheet.png");
    Vector2 knightPos{
        (float)Win_Settings.x / 2.0f - 4.0f * (0.5f * (float)Knight_Idle.width / 6.0f),
        (float)Win_Settings.y / 2.0f - 4.0f * (0.5f * (float)Knight_Idle.height)};

    Character KnightObject;
    KnightObject.setTexture(Knight_Idle);
    KnightObject.setScreenPosition(knightPos);
    

    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);

        Vector2 FinalDirection = FindDirection(KnightObject.getScreenPosition(), Win_Settings, Map.Scale, Speed);
        if (Vector2Length(FinalDirection) != 0.0f)
        {
            Map.Position = Vector2Subtract(Map.Position, FinalDirection);
            // FinalDirection.x < 0.0f ? rightleft = -1.0f : rightleft = 1.0f;
            FinalDirection.x < 0.0f ? Flip = -1.0f : Flip = 1.0f;
            Current_Knight = Knight_Run;
        }
        else
        {
            Current_Knight = Knight_Idle;
        }
        // TICK FUNCTION CALL
        KnightObject.Tick(GetFrameTime());
        // ------- drawing map image -------
        DrawTextureEx(Map.Texture, Map.Position, Map.Rotation, Map.Scale, Map.Tint);

        // ------- Drawing the character ----------
        CharacterAnimation.Frame = AnimateCharacter(CharacterAnimation.Frame,
                                                    CharacterAnimation.MaxFrame,
                                                    CharacterAnimation.RunningTime,
                                                    CharacterAnimation.UpdateTime,
                                                    GetFrameTime());

        Rectangle Source{
            CharacterAnimation.Frame * (float)Knight_Idle.width / CharacterAnimation.MaxFrame,
            0.0f,
            KnightObject.XFlip * (float)(Knight_Idle.width / 6.0f),
            (float)Current_Knight.height};
        Rectangle Dest{
            knightPos.x, knightPos.y,
            4.0f * Current_Knight.width / 6.0f,
            4.0f * Current_Knight.height};

        DrawTexturePro(Current_Knight, Source, Dest, Vector2{}, 0.0f, WHITE);

        EndDrawing();
    }
    UnloadTexture(Map.Texture);
    UnloadTexture(Current_Knight);
    UnloadTexture(Knight_Idle);
    UnloadTexture(Knight_Run);
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

int AnimateCharacter(int Frame, int MaxFrame, float &RunningTime, float UpdateTime, float deltatime)
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

class Character
{
private:
    Texture2D Texture;
    Texture2D Idle;
    Texture2D Run;
    Vector2 ScreenPosition;
    Vector2 WorldPosition;
    // Character flip
    float XFlip = 1.0f;
    float RunningTime = 0.0f;
    const float UpdateTime = 0.0f;
    int Frame = 0;
    const int MaxFrame = 0;
    float Speed = 5.0f;

public:
    // ---------------------- Constructor ----------------------
    Character()
    {
        this->ScreenPosition = Vector2{};
        this->WorldPosition = Vector2{};
        this->MaxFrame = 0;
        this->UpdateTime = 0;
    }
    Character(const char *FilePath, Vector2 ScreenPos, Vector2 WorldPos, int MaxFrame, float UpdateTime)
    {
        this->Texture = LoadTexture(FilePath);
        this->ScreenPosition = ScreenPos;
        this->WorldPosition = WorldPos;
        this->MaxFrame = MaxFrame;
        this->UpdateTime = UpdateTime;
    }
    Character(Texture2D Texture, Vector2 ScreenPos, Vector2 WorldPos)
    {
        this->Texture = Texture;
        this->ScreenPosition = ScreenPos;
        this->WorldPosition = WorldPos;
    }
    // ---------------------- Setters ----------------------
    void setTexture(Texture2D inTexture)
    {
        this->Texture = inTexture;
    }
    void setScreenPosition(Vector2 inScreenPos)
    {
        this->ScreenPosition = inScreenPos;
    }
    void setWorldPosition(Vector2 inWorldPos)
    {
        this->WorldPosition = inWorldPos;
    }
    void setUpdateTime(float inUpdateTime)
    {
        this->UpdateTime = inUpdateTime;
    }
    void setMaxFrame(float inMaxFrame)
    {
        this->MaxFrame = inMaxFrame;
    }
    // ---------------------- Getters ----------------------
    Texture2D getTexture()
    {
        return this->Texture;
    }
    Vector2 getScreenPosition()
    {
        return this->ScreenPosition;
    }
    Vector2 getWorldPosition()
    {
        return this->WorldPosition;
    }
    float getRunningTime()
    {
        return this->RunningTime;
    }
    int getFrame()
    {
        return this->Frame;
    }
    // ---------------------- TickFunction ----------------------
    void Tick(float DeltaTime);
};

void Character::Tick(float deltatime)
{
    Vector2 FinalDirection = FindDirection(ScreenPosition, Win_Settings, Map.Scale, Speed);
    if (Vector2Length(FinalDirection) != 0.0f)
    {
        Map.Position = Vector2Subtract(Map.Position, FinalDirection);
        // FinalDirection.x < 0.0f ? rightleft = -1.0f : rightleft = 1.0f;
        FinalDirection.x < 0.0f ? Character.Flip = -1.0f : Character.Flip = 1.0f;
        Current_Knight = Knight_Run;
    }
    else
    {
        Current_Knight = Knight_Idle;
    }
}

class Map
{
};