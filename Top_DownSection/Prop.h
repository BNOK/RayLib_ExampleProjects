#include "raylib.h"


class Prop{

private:
    Texture2D Texture{};
    Vector2 WorldPosition{};
    float Scale{};
     
public:
    Prop(Texture2D tex, Vector2 worldPos);    

    Vector2 DrawProp(Vector2 Position);
};