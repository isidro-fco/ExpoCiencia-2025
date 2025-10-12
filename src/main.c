#include "CimaBits.h"

bool pantalla_desarrollador_activa = false;
bool busqueda = false;
bool seleccion_busqueda = false;
bool multimedia = false;
bool esta_reproduciendo = true;
bool mutear = false;

int main()
{
    CancionPTR playlist = crear_cancion();

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "Pantalla Completa");

    Texture2D fondo = LoadTexture("assets/fondo1.jpg");
    SetTextureFilter(fondo, TEXTURE_FILTER_BILINEAR);

    Font fuente_titulo = LoadFontEx("assets/tipografia/TitanOne-Regular.ttf", 64, NULL, 0);
    Font fuente1 = LoadFontEx("assets/tipografia/BebasNeue-Regular.ttf", 64, NULL, 0);
    Font fuente2 = LoadFontEx("assets/tipografia/ChauPhilomeneOne-Regular.ttf", 64, NULL, 0);

    SetTextureFilter(fuente_titulo.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente1.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente2.texture, TEXTURE_FILTER_BILINEAR);

    configurar_botones();

    SetTargetFPS(60);
    InitAudioDevice();
    SetMasterVolume(0.5f);

    while (!WindowShouldClose())
    {
        Vector2 posicion_mouse = GetMousePosition();

        if (mutear)
        {
            if (manejar_boton_simple(boton_activar_volumen))
            {
                mutear = false;
                SetMasterVolume(0.0f);
            }
        }
        else
        {
            if (manejar_boton_simple(boton_silenciar_volumen))
            {
                mutear = true;
                SetMasterVolume(0.5f);
            }
        }

        BeginDrawing();
        // ClearBackground(RAYWHITE);

        /*if (!pantalla_desarrollador_activa)
         {
             pantalla_desarrollador(fuente_titulo, fuente1, fuente2);
             pantalla_desarrollador_activa = true;
         }*/

        float escala = fmaxf((float)ANCHO_PANTALLA / ANCHO_FONDO, (float)ALTO_PANTALLA / ALTO_FONDO);
        DrawTexturePro(fondo, (Rectangle){0, 0, ANCHO_FONDO, ALTO_FONDO}, (Rectangle){(ANCHO_PANTALLA - ANCHO_FONDO * escala) * 0.5f, (ALTO_PANTALLA - ALTO_FONDO * escala) * 0.5f, ANCHO_FONDO * escala, ALTO_FONDO * escala}, (Vector2){0, 0}, 0.0f, WHITE);

        secciones_visuales_encabezados();
        // PARTE SUPERIOR
        if (busqueda)
        {
            bool nombre = false;
            bool artista = false;
            bool duracion = false;
            
            Rectangle casilla_nombre = {ANCHO_PANTALLA * 0.4, ALTO_PANTALLA * 0.1, 25, 25};
            Rectangle casilla_artista = {ANCHO_PANTALLA * 0.6, ALTO_PANTALLA * 0.1, 25, 25};
            Rectangle casilla_duracion = {ANCHO_PANTALLA * 0.8, ALTO_PANTALLA * 0.1, 25, 25};

            DrawRectangleRounded((Rectangle){550, ALTO_PANTALLA * 0.01, 1200, 90}, REDONDEZ + 0.5, SEGMENTOS, color_verde);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(posicion_mouse, casilla_nombre))
                {
                    nombre = true;
                    artista = false;
                    duracion = false;
                    seleccion_busqueda = true;
                }
                else if (CheckCollisionPointRec(posicion_mouse, casilla_artista))
                {
                    nombre = false;
                    artista = true;
                    duracion = false;
                    seleccion_busqueda = true;
                }
                else if (CheckCollisionPointRec(posicion_mouse, casilla_duracion))
                {
                    nombre = false;
                    artista = false;
                    duracion = true;
                    seleccion_busqueda = true;
                }
            }
            
            DrawRectangleRounded(casilla_nombre, REDONDEZ, SEGMENTOS, nombre ? color_casilla_marcada : color_campo_inactivo);
            DrawRectangleRounded(casilla_artista, REDONDEZ, SEGMENTOS, artista ? color_casilla_marcada : color_campo_inactivo);
            DrawRectangleRounded(casilla_duracion, REDONDEZ, SEGMENTOS, duracion ? color_casilla_marcada : color_campo_inactivo);

            if (manejar_boton_simple(boton_aceptar_busqueda) && seleccion_busqueda)
            {
                DrawCircle(700, 700, 1.23, color_amarillo);
            }
            // Dibujar y manejar el botón para cerrar la búsqueda
            if (manejar_boton_simple(boton_cancelar_busqueda) || IsKeyPressed(KEY_DELETE))
            {
                busqueda = false;
            }
        }
        else // Si la búsqueda NO está activa
        {
            // Dibujar ambos botones normalmente
            if (manejar_boton_simple(boton_agregar))
            {
                formulario(&playlist, posicion_mouse, fondo, fuente_titulo, fuente1, fuente2);
            }

            if (manejar_boton_simple(boton_buscar) || IsKeyPressed(KEY_F))
            {
                busqueda = true;
            }
        }

        // PARTE CENTRAL
        if (multimedia)
        {
            secciones_visuales_video();
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) * 2 + 30, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){485, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
            if (CheckCollisionPointRec(posicion_mouse, (Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                multimedia = false;
            }
        }
        else
        {
            secciones_visuales_musica();
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){485, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
            DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);
            if (manejar_boton_simple(boton_eliminar) || IsKeyPressed(KEY_DELETE))
            {
            }
            if (CheckCollisionPointRec(posicion_mouse, (Rectangle){((ANCHO_PANTALLA * 0.4) / 4) * 2 + 30, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
            {
                multimedia = true;
            }
        }

        // PARTE INFERIOR
        if (manejar_boton_simple(boton_retroceder))
        {
            /* code */
        }

        if (esta_reproduciendo)
        {
            if (manejar_boton_simple(boton_pausa) || IsKeyPressed(KEY_SPACE))
            {
                esta_reproduciendo = false;
            }
        }
        else
        {
            if (manejar_boton_simple(boton_play) || IsKeyPressed(KEY_SPACE))
            {
                esta_reproduciendo = true;
            }
        }

        if (manejar_boton_simple(boton_adelantar))
        {
            /* code */
        }

        dibujar_linea_tiempo(&audio_actual, fuente2, esta_reproduciendo);

        if (mutear)
        {
            manejar_boton_simple(boton_silenciar_volumen);
        }
        else
        {
            manejar_boton_simple(boton_activar_volumen);
        }

        EndDrawing();
    }

    UnloadTexture(fondo);
    UnloadFont(fuente_titulo);
    UnloadFont(fuente1);
    UnloadFont(fuente2);
    UnloadTexture(boton_buscar.textura);
    UnloadTexture(boton_agregar.textura);
    UnloadTexture(boton_play.textura);
    UnloadTexture(boton_pausa.textura);
    UnloadTexture(boton_retroceder.textura);
    UnloadTexture(boton_adelantar.textura);
    UnloadTexture(boton_activar_volumen.textura);
    UnloadTexture(boton_silenciar_volumen.textura);
    UnloadTexture(boton_eliminar.textura);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
