#include "CimaBits.h"

int main()
{
    // VARIABLES
    bool pantalla_desarrollador_activa = false;
    bool multimedia = true;

    // AQUI EMPIEZA EL CODIGO
    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "Pantalla Completa");

    Texture2D fondo = LoadTexture("assets/fondo.jpg");
    SetTextureFilter(fondo, TEXTURE_FILTER_BILINEAR);

    Font fuente_titulo = LoadFontEx("assets/tipografia/TitanOne-Regular.ttf", 64, NULL, 0);
    Font fuente1 = LoadFontEx("assets/tipografia/BebasNeue-Regular.ttf", 64, NULL, 0);
    Font fuente2 = LoadFontEx("assets/tipografia/ChauPhilomeneOne-Regular.ttf", 64, NULL, 0);

    SetTextureFilter(fuente_titulo.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente1.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente2.texture, TEXTURE_FILTER_BILINEAR);

    char titulo[65];
    char artista[45];
    char duracion[8];
    char imagen[255];
    char audio[255];
    char video[255];
    int insertar;
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

        secciones_visuales_encabezados();

        if (multimedia)
        {
            secciones_visuales_video();
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA*0.68)/3)+0.1, 300, (ANCHO_PANTALLA*0.68)/3, 50}, REDONDEZ,SEGMENTOS, color_fondo);
        }
        else
        {
            secciones_visuales_musica();
        }
        
        
        

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
