#include "CimaBits.h"

int main()
{
    bool pantalla_desarrollador_activa = false;
    bool busqueda = false;
    static bool seleccion_busqueda = false;
    static bool nombre = false;
    static bool artista = false;
    static bool duracion = false;
    bool abrir_busqueda_anterior = busqueda;

    bool multimedia = false;
    bool esta_reproduciendo = true;
    bool esta_silenciado = false;
    bool mostrando_filtro_anterior = false;
    bool mutear = false;

    int total_canciones = 0;

    CancionPTR playlist = NULL;
    CancionPTR cancion_actual = NULL;
    llenar_lista_canciones(&playlist, &total_canciones);
    cancion_actual = playlist;

    int cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "CimaBits 2025");

    Estado_Scroll scroll = {0, 0, 0, false};

    scroll.inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);
    scroll.target_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);

    Texture2D fondo = LoadTexture("assets/fondo1.jpg");
    SetTextureFilter(fondo, TEXTURE_FILTER_BILINEAR);

    Font fuente_titulo = LoadFontEx("assets/tipografia/TitanOne-Regular.ttf", 64, NULL, 0);
    Font fuente1 = LoadFontEx("assets/tipografia/BebasNeue-Regular.ttf", 64, NULL, 0);
    Font fuente2 = LoadFontEx("assets/tipografia/ChauPhilomeneOne-Regular.ttf", 64, NULL, 0);

    SetTextureFilter(fuente_titulo.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente1.texture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(fuente2.texture, TEXTURE_FILTER_BILINEAR);

    Seccion_Pantalla seccion_actual = SECCION_NINGUNA;
    Seccion_Pantalla Seccion_activa = SECCION_NINGUNA;

    configurar_botones();

    SetTargetFPS(60);
    InitAudioDevice();
    SetMasterVolume(0.5f);

    while (!WindowShouldClose())
    {
        actualizar_scroll(&scroll);
        Vector2 posicion_mouse = GetMousePosition();

        // Lógica de selección con el mouse
        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !scroll.scrolling)
        {
            const int tabla_y = 150;
            const int fila_altura = 80;

            if (posicion_mouse.x >= 50 && posicion_mouse.x <= 750 &&
                posicion_mouse.y >= tabla_y && posicion_mouse.y <= tabla_y + (CANCIONES_VISIBLES * fila_altura))
            {
                int fila = (posicion_mouse.y - tabla_y) / fila_altura;
                int idx = scroll.inicio + fila;

                if (idx >= 0 && idx < total_canciones)
                {
                    // Navegar a la canción seleccionada
                    Cancion *nueva_cancion = playlist;
                    for (int i = 0; i < idx; i++)
                    {
                        nueva_cancion = nueva_cancion->siguiente;
                    }

                    cambiar_cancion_actual(&cancion_actual, nueva_cancion, &esta_reproduciendo);

                    // Sincronizar el índice de la UI
                    cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);

                    // Actualizar scroll
                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);;
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }
        }

        // Navegación con teclado o botones
        if (!scroll.scrolling)
        {
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN) || manejar_boton_simple(boton_adelantar))
            {
                if (total_canciones > 0)
                {
                    CancionPTR siguiente = siguiente_cancion(cancion_actual);
                    cambiar_cancion_actual(&cancion_actual, siguiente, &esta_reproduciendo);
                    cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);;
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || manejar_boton_simple(boton_retroceder))
            {
                if (total_canciones > 0)
                {
                    CancionPTR anterior = anterior_cancion(cancion_actual);
                    cambiar_cancion_actual(&cancion_actual, anterior, &esta_reproduciendo);
                    cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);;
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            // Scroll rápido con PageUp/PageDown
            if (IsKeyPressed(KEY_PAGE_DOWN))
            {
                CancionPTR temp = cancion_actual;
                for (int i = 0; i < CANCIONES_VISIBLES; i++)
                {
                    temp = siguiente_cancion(temp);
                }
                cambiar_cancion_actual(&cancion_actual, temp, &esta_reproduciendo);
                cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);
                int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);;
                scroll.target_inicio = nuevo_inicio;
                scroll.scrolling = true;
            }

            if (IsKeyPressed(KEY_PAGE_UP))
            {
                CancionPTR temp = cancion_actual;
                for (int i = 0; i < CANCIONES_VISIBLES; i++)
                {
                    temp = anterior_cancion(temp);
                }
                cambiar_cancion_actual(&cancion_actual, temp, &esta_reproduciendo);
                cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);
                int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);;
                scroll.target_inicio = nuevo_inicio;
                scroll.scrolling = true;
            }

            // Ir al inicio/fin con Home/End
            if (IsKeyPressed(KEY_HOME))
            {
                cambiar_cancion_actual(&cancion_actual, playlist, &esta_reproduciendo);
                cancion_seleccionada = 0;
                scroll.target_inicio = 0;
                scroll.scrolling = true;
            }

            if (IsKeyPressed(KEY_END))
            {
                cambiar_cancion_actual(&cancion_actual, ultima_cancion, &esta_reproduciendo);
                cancion_seleccionada = total_canciones - 1;
                scroll.target_inicio = total_canciones - CANCIONES_VISIBLES;
                scroll.scrolling = true;
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        if (!pantalla_desarrollador_activa)
        {
            pantalla_desarrollador(fuente_titulo, fuente1, fuente2);
            pantalla_desarrollador_activa = true;
            continue;
        }

        float escala = fmaxf((float)ANCHO_PANTALLA / ANCHO_FONDO, (float)ALTO_PANTALLA / ALTO_FONDO);
        DrawTexturePro(fondo, (Rectangle){0, 0, ANCHO_FONDO, ALTO_FONDO}, (Rectangle){(ANCHO_PANTALLA - ANCHO_FONDO * escala) * 0.5f, (ALTO_PANTALLA - ALTO_FONDO * escala) * 0.5f, ANCHO_FONDO * escala, ALTO_FONDO * escala}, (Vector2){0, 0}, 0.0f, WHITE);

        secciones_visuales_encabezados();
        // PARTE SUPERIOR
        if (busqueda)
        {
            Rectangle casilla_nombre = {ANCHO_PANTALLA * 0.4, ALTO_PANTALLA * 0.1, 30, 30};
            Rectangle casilla_artista = {ANCHO_PANTALLA * 0.6, ALTO_PANTALLA * 0.1, 30, 30};
            Rectangle casilla_duracion = {ANCHO_PANTALLA * 0.8, ALTO_PANTALLA * 0.1, 30, 30};

            DrawRectangleRounded((Rectangle){550, ALTO_PANTALLA * 0.01, 1200, 90}, REDONDEZ + 0.5, SEGMENTOS, color_verde);
            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (busqueda && !abrir_busqueda_anterior)
                {
                    nombre = false;
                    artista = false;
                    duracion = false;
                    seleccion_busqueda = false;
                }

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

            DrawTextEx(fuente2, "FILTROS:", (Vector2){ANCHO_PANTALLA * 0.3, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawRectangleRounded(casilla_nombre, REDONDEZ, SEGMENTOS, nombre ? color_casilla_marcada : color_campo_inactivo);
            DrawTextEx(fuente2, "NOMBRE", (Vector2){ANCHO_PANTALLA * 0.4 + 35, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawRectangleRounded(casilla_artista, REDONDEZ, SEGMENTOS, artista ? color_casilla_marcada : color_campo_inactivo);
            DrawTextEx(fuente2, "ARTISTA", (Vector2){ANCHO_PANTALLA * 0.6 + 35, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawRectangleRounded(casilla_duracion, REDONDEZ, SEGMENTOS, duracion ? color_casilla_marcada : color_campo_inactivo);
            DrawTextEx(fuente2, "DURACION", (Vector2){ANCHO_PANTALLA * 0.8 + 35, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);

            if (manejar_boton_simple(boton_aceptar_busqueda) && !seleccion_busqueda)
            {
                DrawCircle(700, 700, 300, color_amarillo);
            }
            // Dibujar y manejar el botón para cerrar la búsqueda
            if (manejar_boton_simple(boton_cancelar_busqueda))
            {
                busqueda = false;
            }
        }
        else // Si la búsqueda NO está activa
        {
            // Dibujar ambos botones normalmente
            if (manejar_boton_simple(boton_agregar))
            {
                formulario(&playlist, total_canciones, fondo, fuente_titulo, fuente1, fuente2);
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
            secciones_visuales_musica(&playlist, cancion_actual, total_canciones, scroll, cancion_seleccionada, esta_reproduciendo, fuente1, fuente2);
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
