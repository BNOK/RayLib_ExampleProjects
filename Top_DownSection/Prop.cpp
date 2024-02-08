#include "Prop.h"

Prop::Prop(Texture2D tex, Vector2 worldPos):
    Texture(tex),
    WorldPosition(worldPos)
{}

Vector2 Prop::DrawProp(Vector2 Position){
    DrawTextureEx(Texture,WorldPosition,0.0f,Scale );
}

