#include "CimaBits.h"

int main()
{
    // VARIABLES
    bool pantalla_desarrollador_activa = false;
    bool multimedia = false;

    // AQUI EMPIEZA EL CODIGO
    CancionPTR playlist = crear_cancion();

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

        if (!multimedia)
        {
            secciones_visuales_video();
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.68) / 3), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){(((ANCHO_PANTALLA * 0.68) / 3) + ((ANCHO_PANTALLA * 0.68) / 6) - 5), (ALTO_PANTALLA * 0.17) + 5, (ANCHO_PANTALLA * 0.68) / 6, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){720, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawTextEx(fuente2, "AUDIO", (Vector2){520, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
        }
        else
        {
            secciones_visuales_musica();
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6), 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){720, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawTextEx(fuente2, "AUDIO", (Vector2){720, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);
        }


        EndDrawing();
    }


    CloseWindow();
    return 0;
}
