#include "CimaBits.h"

int main()
{
    // VARIABLES
    bool pantalla_desarrollador_activa = false;
    
    // AQUI EMPIEZA EL CODIGO
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "Pantalla Completa");

    Texture2D fondo = LoadTexture("Assets/fondo.jpg");
    SetTextureFilter(fondo, TEXTURE_FILTER_BILINEAR);

    Font fuente_titulo = LoadFontEx("Assets/tipografia/TitanOne-Regular.ttf", 64, NULL, 0);
    Font fuente1 = LoadFontEx("Assets/tipografia/BebasNeue-Regular.ttf", 64, NULL, 0);
    Font fuente2 = LoadFontEx("Assets/tipografia/ChauPhilomeneOne-Regular.ttf", 64, NULL, 0);

    SetTextureFilter(fuente_titulo.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente1.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente2.texture, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(RAYWHITE);

       /*if (!pantalla_desarrollador_activa)
        {
            pantalla_desarrollador(fuente_titulo, fuente1, fuente2);
            pantalla_desarrollador_activa = true;
        }*/

        float escala = fmaxf((float)ANCHO_PANTALLA / ANCHO_FONDO, (float)ALTO_PANTALLA / ALTO_FONDO);
        DrawTexturePro(fondo, (Rectangle){0, 0, ANCHO_FONDO, ALTO_FONDO}, (Rectangle){(ANCHO_PANTALLA - ANCHO_FONDO * escala) * 0.5f, (ALTO_PANTALLA - ALTO_FONDO * escala) * 0.5f, ANCHO_FONDO * escala, ALTO_FONDO * escala}, (Vector2){0, 0}, 0.0f, WHITE);

        formulario(fondo, fuente_titulo, fuente1, fuente2);

        DrawText("Pantalla Completa!", 100, 100, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
