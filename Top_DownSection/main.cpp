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
    Rectangle Source;
    Rectangle Dest;
    Vector2 Origin;
    float Rotation;
    Color Tint;
};

#pragma endregion

Vector2 FindDirection(Vector2 Position,Vector2 WindowSettings,int Scale,float Speed);



int main(){
    const Vector2 Win_Settings{960,640};
    InitWindow(Win_Settings.x,Win_Settings.y,"Game Window");
    SetTargetFPS(60);


    // general settings
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

    // Character Settings
    CharacterSettings Knight{
        LoadTexture("characters/knight_run_spritesheet.png"),
        4.0f,
        { 0.0f, 0.0f, (float)Knight.Texture.width/6, (float)Knight.Texture.height},
        {   
            (float) Win_Settings.x/2 - (Knight.Scale * 0.5f * Knight.Texture.width/6.0f), 
            (float) Win_Settings.y/2.0f - (Knight.Scale * 0.5f * Knight.Texture.height),
            (float) Knight.Scale * (Knight.Texture.width/6.0f),
            (float) Knight.Scale * (Knight.Texture.height)
        },
        {0.5f,0.5f},
        0.0f,
        WHITE
    };


    while(!WindowShouldClose()){
        BeginDrawing();
        ClearBackground(WHITE);
        
        Vector2 FinalDirection= FindDirection(Map.Position,Win_Settings,Map.Scale,Speed);
        if(Vector2Length(FinalDirection) != 0.0){
            Map.Position = Vector2Subtract(Map.Position,FinalDirection);
        }
        
        
        
        
        // drawing map image
        DrawTextureEx(Map.Texture,Map.Position,Map.Rotation,Map.Scale,Map.Tint);
        DrawTexturePro(Knight.Texture,Knight.Source,Knight.Dest,Knight.Origin,Knight.Rotation,Knight.Tint);
        
        
        //DrawText(TextFormat("Direction : %.3f,%.3f",FinalDirection.x,FinalDirection.y),100,100,50,RED);
        EndDrawing();
    }
    UnloadTexture(Map.Texture); 
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