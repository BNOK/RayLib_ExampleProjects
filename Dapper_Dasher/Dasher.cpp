#include <cstdio>
#include "raylib.h"

struct AnimData
{
    Rectangle AnimRec;
    Vector2 ObjectPosition;

    float RunningTime;
    float UpdateTime;
    int Frame;
};

void DrawBackground(Texture Texture, Vector2& background1,Vector2& background2, float Scale, float Speed, float deltatime,Color Tint){
    //Background Movement
        

        DrawTextureEx(Texture,background1,0,Scale,Tint);
        DrawTextureEx(Texture,background2,0,Scale,Tint);

        background1.x += Speed * deltatime;
        background2.x += Speed * deltatime;
        
        if(background1.x + (Texture.width*Scale) < 0 ){
            //DrawText("RESET",20,20,20,BLACK);
            background1.x = background2.x+(Texture.width*Scale) ;
        } 

        if(background2.x + (Texture.width*Scale) < 0 ){
            //DrawText("RESET",20,20,20,BLACK);
            background2.x = background1.x+(Texture.width*Scale) ;
        } 
}

bool isGrounded(AnimData ObjectToCheck, float Height )
{
    return ObjectToCheck.ObjectPosition.y >= Height - ObjectToCheck.AnimRec.height;
}

// move on the x axis alone
void CalculateMovement(AnimData& Object, float velocity, float deltatime)
{
    Object.ObjectPosition.x += velocity * deltatime;
}

void Jump(float& Object_Velocity,float Jump_Velocity)
{
    Object_Velocity += Jump_Velocity; 
}

AnimData UpdateAnimation(AnimData Object, float delta_time, int FrameLength)
{
    Object.RunningTime += delta_time;
            if(Object.RunningTime >= 1/Object.UpdateTime)
            {
                Object.RunningTime = 0;
                Object.Frame++;
                Object.AnimRec.x = Object.Frame * Object.AnimRec.width;
                if(Object.Frame > FrameLength-1)
                {
                    Object.Frame = 0;
                }
            }
    return Object;
}

float ResizeTexture(float texture_width,const int Window_width){
    
    return Window_width/texture_width;
}

Color getNewColor(){
    Color tempColor{
        GetRandomValue(0,255),
        GetRandomValue(0,255),
        GetRandomValue(0,255),
        1
    };

    return tempColor;
}

void ResetLevel(bool& collision,bool& finishlevel,AnimData* Obstacles,int ObstacleSize,const int* WindowSize ){
    collision = false;
    finishlevel = false;    
    for(int i=0;i<ObstacleSize;i++){
        Obstacles[i].ObjectPosition.x = WindowSize[0] + (500*i);
        Obstacles[i].ObjectPosition.y = WindowSize[1] - Obstacles[i].AnimRec.height;
    }

}

// --------------------------------- MAIN FUNCTION ------------------------------


int main()
{
    // ----------- WINDOW SETTINGS -----------
    const int Win_Settings[2] = {1280,720};
    
    const int Target_FPS{60};
    InitWindow(Win_Settings[0], Win_Settings[1], "DapperDasher");
    SetTargetFPS(Target_FPS);
    bool collision = false;

    // ----------- Gravity Settings -----------
    int g_acceleration = 10; // earth gravity (TAKING INTO CONSIDERATION THE DELTA TIME)  
    float velocity = 0;
    int Gravity_Scale = 150; // without this the gravity effect is very low 
    
    // ----------- Background Settings -----------
    Texture2D Background = LoadTexture("textures/far-buildings.png");
    Texture2D Midground = LoadTexture("textures/back-buildings.png");
    Texture2D Foreground =  LoadTexture("textures/foreground.png");

    int Bg_Velocity{-100};
    int mg_velocity{Bg_Velocity*2};
    int fg_velocity{mg_velocity * 2};
    //background
    float bg_Scale = ResizeTexture(Background.width,Win_Settings[0]); 
    Vector2 Bg_Pos{0.0 , 0.0};
    Vector2 Bg_Pos1{Background.width*bg_Scale , 0.0};
    //midground
    float mg_Scale = ResizeTexture(Midground.width , Win_Settings[0]); 
    Vector2 Mg_Pos{0.0 , 0.0};
    Vector2 Mg_Pos1{Midground.width*mg_Scale , 0.0};
    //foreground
    float fg_Scale = ResizeTexture(Foreground.width,Win_Settings[0]); 
    Vector2 Fg_Pos{0.0 , Win_Settings[1]- (Foreground.height*fg_Scale)};
    Vector2 Fg_Pos1{Foreground.width * fg_Scale, Win_Settings[1] - (Foreground.height*fg_Scale) };

    // ----------- Obstacle Initialization -----------    
    Texture2D Obstacle = LoadTexture("textures/12_nebula_spritesheet.png");

    int Obstacle_Number = 10;
    AnimData Obstacle_Array[Obstacle_Number];

    for(int i=0;i<Obstacle_Number;i++){
        // Animation Update Settings
        Obstacle_Array[i].Frame = 0;
        Obstacle_Array[i].RunningTime = 0;
        Obstacle_Array[i].UpdateTime = 12;
        // Animation Rectangle Settings
        Obstacle_Array[i].AnimRec.width = Obstacle.width/8;
        Obstacle_Array[i].AnimRec.height = Obstacle.height/8;
        Obstacle_Array[i].AnimRec.x = 0;
        Obstacle_Array[i].AnimRec.y = 0;
        //Animation Position Settings
        Obstacle_Array[i].ObjectPosition.x = Win_Settings[0] + (400*i);
        Obstacle_Array[i].ObjectPosition.y = Win_Settings[1] - Obstacle_Array[i].AnimRec.height;

    }

    int Obstacle_Vel{-400};

    // -------------- Finish Line --------------------
    float FinishLine = Obstacle_Array[Obstacle_Number-1].ObjectPosition.x + 500;
    bool FinishedLevel =  false;
    Color LevelColor = WHITE;

    // -------------- character Settings --------------
    Texture2D Player = LoadTexture("textures/scarfy.png");
    AnimData PlayerAnim;
    PlayerAnim.AnimRec.width = Player.width/6;
    PlayerAnim.AnimRec.height = Player.height;
    PlayerAnim.AnimRec.x = 0;
    PlayerAnim.AnimRec.y = 0;
    PlayerAnim.ObjectPosition.x = Win_Settings[0]/2 - PlayerAnim.AnimRec.width/2;
    PlayerAnim.ObjectPosition.y = Win_Settings[1] - PlayerAnim.AnimRec.height;
    PlayerAnim.Frame = 0;
    PlayerAnim.RunningTime =0;
    PlayerAnim.UpdateTime = 12; 

    

    // -------------- jUMP SETTINGS --------------
    bool canJump = true;
    int Jump_Vel{-600};


    // ++++++++++++++++++++++ Game LOOP +++++++++++++++++++
    while (!WindowShouldClose())
    {
        const float delta_time = GetFrameTime();

        BeginDrawing();
        ClearBackground(WHITE);

        // ------------------- DrawingBackground -------------------
        DrawBackground(Background,Bg_Pos,Bg_Pos1,bg_Scale,Bg_Velocity,delta_time,LevelColor);
        DrawBackground(Midground,Mg_Pos,Mg_Pos1,mg_Scale,mg_velocity,delta_time,LevelColor);
        DrawBackground(Foreground,Fg_Pos,Fg_Pos1,fg_Scale,fg_velocity,delta_time,LevelColor);
        
        if(!collision && !FinishedLevel){
            //Check to apply gravity
            if(!isGrounded(PlayerAnim, Win_Settings[1]))
            {
                velocity += g_acceleration*delta_time*Gravity_Scale ;    
                canJump = false;
            }
            else
            {
                velocity = 0;
                canJump =  true;
            }


            if(IsKeyPressed(KEY_SPACE) && canJump){
                Jump(velocity,Jump_Vel);
                //velocity += Jump_Vel;
            }

            

            // PlayerMovement
            PlayerAnim.ObjectPosition.y += velocity * delta_time;

            for(int i=0;i<Obstacle_Number;i++)
            {
                Obstacle_Array[i].ObjectPosition.x += Obstacle_Vel * delta_time;
            }

            FinishLine += Obstacle_Vel * delta_time;
            

            // Player Jumping
            if(canJump){
                PlayerAnim = UpdateAnimation(PlayerAnim,delta_time,6);
            }

            //------------------ Collision detection ------------------------
            for(AnimData Obstacle : Obstacle_Array){
                float pad = 50;
            Rectangle temp_rec{
                    Obstacle.ObjectPosition.x +pad,
                    Obstacle.ObjectPosition.y +pad,
                    Obstacle.AnimRec.width -2*pad,
                    Obstacle.AnimRec.height- 2*pad
            };
                Rectangle Player_temp_rec{
                    PlayerAnim.ObjectPosition.x,
                    PlayerAnim.ObjectPosition.y,
                    PlayerAnim.AnimRec.width,
                    PlayerAnim.AnimRec.height
                };

                if(CheckCollisionRecs(temp_rec,Player_temp_rec)){
                    collision =  true;
                }
                //collision = false;
            }

            if(PlayerAnim.ObjectPosition.x >= FinishLine){
                
                FinishedLevel = true;
            }

            // ------------- Drawing textures -----------
            if(!collision){
                DrawTextureRec(Player,PlayerAnim.AnimRec,PlayerAnim.ObjectPosition,WHITE);
                    for(int i=0;i<Obstacle_Number;i++){
                        Obstacle_Array[i] = UpdateAnimation(Obstacle_Array[i],delta_time,8);
                        DrawTextureRec(Obstacle,Obstacle_Array[i].AnimRec,Obstacle_Array[i].ObjectPosition,WHITE);
                    }
            }
            

        }
        else if(collision){
            DrawText("GameOver !", Win_Settings[0]/2,Win_Settings[1]/2,50,RED);
            
            
        }
        else{
            DrawText("* YOU WIN *",Win_Settings[0]/2,Win_Settings[1]/2,50,GREEN);
            
            if(IsKeyPressed(KEY_R)){
                LevelColor = getNewColor();
                ResetLevel(collision,FinishedLevel,Obstacle_Array,Obstacle_Number,Win_Settings);
            }
        }

        

        EndDrawing();
    }
    UnloadTexture(Player);
    UnloadTexture(Obstacle);
    CloseWindow();    

}