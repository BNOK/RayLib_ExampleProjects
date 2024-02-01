#include "raylib.h"
#include "raymath.h"
#include <cstdio>

#pragma region declarations
// --------------- Window Struct ---------------
struct MapSettings
{
    Texture2D Texture;
    Vector2 Position;
    float Rotation;
    int Scale;
    Color Tint;
};

struct CharacterSettings
{
    Texture2D Texture;
    float Scale;
    float Flip;
    Vector2 Position;
};

struct AnimData
{
    float RunningTime;
    float UpdateTime;
    int Frame;
    int MaxFrame;
};


#pragma endregion

Vector2 FindDirection(Vector2 Position,Vector2 WindowSettings,int Scale,float Speed);




int main(){
    const Vector2 Win_Settings{960,640};
    InitWindow(Win_Settings.x,Win_Settings.y,"Game Window");
    


    // ---------------------- general settings ----------------------
    float delta_time = GetFrameTime();
    int Speed{10};

    // Map Settings
    MapSettings Map {
        LoadTexture("nature_tileset/Game_Map.png"),
        {0,0},
        0,
        4,
        WHITE
    };
    // ------------------------ Character Settings ----------------------------------
    CharacterSettings OUH{
        LoadTexture("characters/knight_run_spritesheet.png"),
        4.0f,
        1.0f,
        {
        (float)Win_Settings.x/2.0f - 4.0f * (0.5f * (float)OUH.Texture.width/6.0f),
        (float)Win_Settings.y/2.0f - 4.0f * (0.5f * (float)OUH.Texture.height)
        },
    };


    Texture2D Knight_Run = LoadTexture("characters/knight_run_spritesheet.png");
    Texture2D Knight_Idle = LoadTexture("characters/knight_idle_spritesheet.png");
    Texture2D Current_Knight = Knight_Idle;
    Vector2 knightPos{
        (float)Win_Settings.x/2.0f - 4.0f * (0.5f * (float)Current_Knight.width/6.0f),
        (float)Win_Settings.y/2.0f - 4.0f * (0.5f * (float)Current_Knight.height)
    };
    float rightleft = 1.0f;
    

    SetTargetFPS(60);
    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        
        Vector2 FinalDirection= FindDirection(Map.Position,Win_Settings,Map.Scale,Speed);
        if(Vector2Length(FinalDirection) != 0.0f){
            Map.Position = Vector2Subtract(Map.Position,FinalDirection);
            //FinalDirection.x < 0.0f ? rightleft = -1.0f : rightleft = 1.0f;
            FinalDirection.x < 0.0f ? OUH.Flip = -1.0f : OUH.Flip = 1.0f;
        }
        // ------- drawing map image -------
        DrawTextureEx(Map.Texture,Map.Position,Map.Rotation,Map.Scale,Map.Tint);

        // ------- Drawing the character ----------
        Rectangle Source{
            0.0f,0.0f,
            OUH.Flip * (Current_Knight.width/6.0f),
            Current_Knight.height
        };
        Rectangle Dest{
            knightPos.x, knightPos.y,
            4.0f * Current_Knight.width/6.0f,
            4.0f * Current_Knight.height
        };
        
        DrawTexturePro(Current_Knight,Source,Dest,Vector2{},0.0f,WHITE);


        
        EndDrawing();
    }
    UnloadTexture(Map.Texture); 
    //UnloadTexture(Knight.Texture);
    CloseWindow();


}

Vector2 FindDirection(Vector2 Position,Vector2 WindowSettings,int Scale,float Speed){
    Vector2 Direction{0,0};
    float Pad = 50;
    if(IsKeyDown(KEY_A) && Position.x < 0){
        //DrawText("Left",100,100,50,RED);
        Direction.x-= 1.0 ;
    }
    if(IsKeyDown(KEY_D) && Position.x - Pad >= -(WindowSettings.x * (Scale-1)) ){
        //DrawText("Right",20,20,20,RED);
        Direction.x+= 1.0 ;
    }
    if(IsKeyDown(KEY_Z) && Position.y < 0){
        //DrawText("up",20,20,20,RED);
        Direction.y -= 1.0 ;
    }
    if(IsKeyDown(KEY_S) && Position.y - Pad> -(WindowSettings.y * (Scale-1))){
        //DrawText("down",20,20,20,RED);
        Direction.y += 1.0 ;
    }
    Direction = Vector2Normalize(Direction);
    Direction = Vector2Scale(Direction,Speed);
    return Direction;
}