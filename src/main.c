#include "CimaBits.h"

int main()
{
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "Pantalla Completa");

    Texture2D fondo = LoadTexture("Assets/fondo.jpg");
    SetTextureFilter(fondo, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        float escala = fmaxf((float)ANCHO_PANTALLA / ANCHO_FONDO, (float)ALTO_PANTALLA / ALTO_FONDO);
        DrawTexturePro(fondo, (Rectangle){0, 0, ANCHO_FONDO, ALTO_FONDO}, (Rectangle){(ANCHO_PANTALLA - ANCHO_FONDO * escala) * 0.5f, (ALTO_PANTALLA - ALTO_FONDO * escala) * 0.5f, ANCHO_FONDO * escala, ALTO_FONDO * escala}, (Vector2){0, 0}, 0.0f, WHITE);
        
        DrawText("Pantalla Completa!", 100, 100, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
