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
    bool confirmar_eliminacion = false;
    double tiempo_confirmacion = 0;
    bool mutear = false;
    int total_canciones = 0;

    CancionPTR playlist = NULL;
    CancionPTR cancion_actual = NULL;
    llenar_lista_canciones(&playlist, &total_canciones);
    cancion_actual = playlist;
    static CancionPTR ultima_cancion_video = NULL;

    // Nombre--filtro
    CancionPTR playlistNombre = NULL;
    CancionPTR cancion_actual_nombre = NULL;
    int total_canciones_nombre = 0;
    int cancion_seleccionada_nombre = 0;
    bool una_vez_nombre = true;
    // Artista--filtro
    CancionPTR playlistArtist = NULL;
    CancionPTR cancion_actual_artist = NULL;
    int total_canciones_artist = 0;
    int cancion_seleccionada_artist = 0;
    bool una_vez_artist = true;
    // Duracion--filtro
    CancionPTR playlistDuration = NULL;
    CancionPTR cancion_actual_duration = NULL;
    int total_canciones_duration = 0;
    int cancion_seleccionada_duration = 0;
    bool una_vez_duration = true;
    // Busqueda--filtro
    CancionPTR playlistBusqueda = NULL;
    CancionPTR cancion_actual_busqueda = NULL;
    int total_canciones_busqueda = 0;
    int cancion_seleccionada_busqueda = 0;
    bool una_vez_busqueda = true;
    char texto_busqueda[256] = {0};
    int campo_enfocado_busqueda = 0;

    int cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);

    SetConfigFlags(FLAG_FULLSCREEN_MODE);
    InitWindow(ANCHO_PANTALLA, ALTO_PANTALLA, "CimaBits 2025");

    Estado_Scroll scroll = {0, 0, 0, false, false};

    scroll.inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);
    scroll.target_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);

    Texture2D logo = LoadTexture("assets/logo.png");
    SetTextureFilter(logo, TEXTURE_FILTER_BILINEAR);
    Texture2D fondo = LoadTexture("assets/fondo.jpg");
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

    x = ANCHO_PANTALLA * 0.01f;
    y = ALTO_PANTALLA * 0.16f;

    SetTargetFPS(60);
    InitAudioDevice();
    SetMasterVolume(0.5f);

    while (!WindowShouldClose())
    {
        actualizar_scroll(&scroll);
        Vector2 posicion_mouse = GetMousePosition();

        // NAVEGACIÓN CON CURSOR
        if ((IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !scroll.scrolling) && !nombre && !artista && !duracion)
        {
            const float contenedor_y = ALTO_PANTALLA * 0.16;
            const float contenedor_altura = ALTO_PANTALLA * 0.68;
            const float fila_altura = contenedor_altura / CANCIONES_VISIBLES;

            if (posicion_mouse.x >= ANCHO_PANTALLA * 0.42 && posicion_mouse.x <= ANCHO_PANTALLA * 0.99 &&
                posicion_mouse.y >= contenedor_y && posicion_mouse.y <= contenedor_y + contenedor_altura)
            {
                int fila = (posicion_mouse.y - contenedor_y) / fila_altura;
                int idx = scroll.inicio + fila;

                if (idx >= 0 && idx < total_canciones)
                {
                    CancionPTR nueva_cancion = playlist;
                    for (int i = 0; i < idx && nueva_cancion != NULL; i++)
                    {
                        nueva_cancion = nueva_cancion->siguiente;
                    }

                    cambiar_cancion_actual(&cancion_actual, nueva_cancion, &esta_reproduciendo);
                    cancion_seleccionada = obtener_indice_cancion(playlist, cancion_actual, total_canciones);
                    generar_rutas(rutas, cancion_actual->video);
                    cargar_frames(frames, &total_frames, rutas);
                    frame_actual = 0;
                    tiempo_frame = 0;

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada, total_canciones);
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }
        }

        // NAVEGACIÓN CON TECLADO O BOTONES - PARA BÚSQUEDA
        if (!scroll.scrolling && busqueda && !una_vez_busqueda && playlistBusqueda != NULL)
        {
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN) || manejar_boton_simple(boton_adelantar))
            {
                if (total_canciones_busqueda > 0)
                {
                    CancionPTR siguiente = siguiente_cancion(cancion_actual_busqueda);
                    cambiar_cancion_actual(&cancion_actual_busqueda, siguiente, &esta_reproduciendo);
                    cancion_seleccionada_busqueda = obtener_indice_cancion(playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda);

                    // Cargar video de la canción de búsqueda
                    if (cancion_actual_busqueda != NULL && cancion_actual_busqueda->video[0] != '\0')
                    {
                        generar_rutas(rutas, cancion_actual_busqueda->video);
                        cargar_frames(frames, &total_frames, rutas);
                        frame_actual = 0;
                        tiempo_frame = 0;
                    }

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_busqueda, total_canciones_busqueda);
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || manejar_boton_simple(boton_retroceder))
            {
                if (total_canciones_busqueda > 0)
                {
                    CancionPTR anterior = anterior_cancion(cancion_actual_busqueda);
                    cambiar_cancion_actual(&cancion_actual_busqueda, anterior, &esta_reproduciendo);
                    cancion_seleccionada_busqueda = obtener_indice_cancion(playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda);

                    // Cargar video de la canción de búsqueda
                    if (cancion_actual_busqueda != NULL && cancion_actual_busqueda->video[0] != '\0')
                    {
                        generar_rutas(rutas, cancion_actual_busqueda->video);
                        cargar_frames(frames, &total_frames, rutas);
                        frame_actual = 0;
                        tiempo_frame = 0;
                    }

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_busqueda, total_canciones_busqueda);
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }
        }

        // NAVEGACIÓN AVANZADA PARA BÚSQUEDA
        if (!scroll.scrolling && busqueda && !una_vez_busqueda && playlistBusqueda != NULL)
        {
            if (IsKeyPressed(KEY_PAGE_DOWN))
            {
                if (total_canciones_busqueda > 0)
                {
                    CancionPTR temp = cancion_actual_busqueda;
                    for (int i = 0; i < CANCIONES_VISIBLES && temp != NULL; i++)
                    {
                        temp = siguiente_cancion(temp);
                    }
                    if (temp != NULL)
                    {
                        cambiar_cancion_actual(&cancion_actual_busqueda, temp, &esta_reproduciendo);
                        cancion_seleccionada_busqueda = obtener_indice_cancion(playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda);

                        if (cancion_actual_busqueda != NULL && cancion_actual_busqueda->video[0] != '\0')
                        {
                            generar_rutas(rutas, cancion_actual_busqueda->video);
                            cargar_frames(frames, &total_frames, rutas);
                            frame_actual = 0;
                            tiempo_frame = 0;
                        }

                        int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_busqueda, total_canciones_busqueda);
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            if (IsKeyPressed(KEY_PAGE_UP))
            {
                if (total_canciones_busqueda > 0)
                {
                    CancionPTR temp = cancion_actual_busqueda;
                    for (int i = 0; i < CANCIONES_VISIBLES && temp != NULL; i++)
                    {
                        temp = anterior_cancion(temp);
                    }
                    if (temp != NULL)
                    {
                        cambiar_cancion_actual(&cancion_actual_busqueda, temp, &esta_reproduciendo);
                        cancion_seleccionada_busqueda = obtener_indice_cancion(playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda);

                        if (cancion_actual_busqueda != NULL && cancion_actual_busqueda->video[0] != '\0')
                        {
                            generar_rutas(rutas, cancion_actual_busqueda->video);
                            cargar_frames(frames, &total_frames, rutas);
                            frame_actual = 0;
                            tiempo_frame = 0;
                        }

                        int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_busqueda, total_canciones_busqueda);
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            if (IsKeyPressed(KEY_HOME))
            {
                if (total_canciones_busqueda > 0)
                {
                    cambiar_cancion_actual(&cancion_actual_busqueda, playlistBusqueda, &esta_reproduciendo);
                    cancion_seleccionada_busqueda = 0;

                    if (playlistBusqueda != NULL && playlistBusqueda->video[0] != '\0')
                    {
                        generar_rutas(rutas, playlistBusqueda->video);
                        cargar_frames(frames, &total_frames, rutas);
                        frame_actual = 0;
                        tiempo_frame = 0;
                    }

                    scroll.target_inicio = 0;
                    scroll.scrolling = true;
                }
            }

            if (IsKeyPressed(KEY_END))
            {
                if (total_canciones_busqueda > 0)
                {
                    CancionPTR ultima_busqueda = playlistBusqueda;
                    if (ultima_busqueda != NULL)
                    {
                        // Encontrar la última canción en la lista de búsqueda
                        while (ultima_busqueda->siguiente != NULL && ultima_busqueda->siguiente != playlistBusqueda)
                        {
                            ultima_busqueda = ultima_busqueda->siguiente;
                        }

                        cambiar_cancion_actual(&cancion_actual_busqueda, ultima_busqueda, &esta_reproduciendo);
                        cancion_seleccionada_busqueda = total_canciones_busqueda - 1;

                        if (ultima_busqueda != NULL && ultima_busqueda->video[0] != '\0')
                        {
                            generar_rutas(rutas, ultima_busqueda->video);
                            cargar_frames(frames, &total_frames, rutas);
                            frame_actual = 0;
                            tiempo_frame = 0;
                        }

                        scroll.target_inicio = total_canciones_busqueda - CANCIONES_VISIBLES;
                        if (scroll.target_inicio < 0)
                            scroll.target_inicio = 0;
                        scroll.scrolling = true;
                    }
                }
            }
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // PANTALLA DE PRESENTACIÓN
        if (!pantalla_desarrollador_activa)
        {
            pantalla_desarrollador(logo, fuente_titulo, fuente1, fuente2);
            pantalla_desarrollador_activa = true;
        }

        // FONDO
        float escala = fmaxf((float)ANCHO_PANTALLA / ANCHO_FONDO, (float)ALTO_PANTALLA / ALTO_FONDO);
        DrawTexturePro(fondo, (Rectangle){0, 0, ANCHO_FONDO, ALTO_FONDO}, (Rectangle){(ANCHO_PANTALLA - ANCHO_FONDO * escala) * 0.5f, (ALTO_PANTALLA - ALTO_FONDO * escala) * 0.5f, ANCHO_FONDO * escala, ALTO_FONDO * escala}, (Vector2){0, 0}, 0.0f, WHITE);

        secciones_visuales_encabezados(logo);
        // PARTE SUPERIOR
        if (busqueda)
        {
            Rectangle casilla_nombre = {ANCHO_PANTALLA * 0.5, ALTO_PANTALLA * 0.1, 30, 30};
            Rectangle casilla_artista = {ANCHO_PANTALLA * 0.65, ALTO_PANTALLA * 0.1, 30, 30};
            Rectangle casilla_duracion = {ANCHO_PANTALLA * 0.8, ALTO_PANTALLA * 0.1, 30, 30};

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

            DrawTextEx(fuente2, "ORDENAMIENTOS:", (Vector2){ANCHO_PANTALLA * 0.3, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawRectangleRounded(casilla_nombre, REDONDEZ, SEGMENTOS, nombre ? color_casilla_marcada : color_campo_inactivo);
            DrawTextEx(fuente2, "NOMBRE", (Vector2){ANCHO_PANTALLA * 0.5 + 35, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawRectangleRounded(casilla_artista, REDONDEZ, SEGMENTOS, artista ? color_casilla_marcada : color_campo_inactivo);
            DrawTextEx(fuente2, "ARTISTA", (Vector2){ANCHO_PANTALLA * 0.65 + 35, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);
            DrawRectangleRounded(casilla_duracion, REDONDEZ, SEGMENTOS, duracion ? color_casilla_marcada : color_campo_inactivo);
            DrawTextEx(fuente2, "DURACION", (Vector2){ANCHO_PANTALLA * 0.8 + 35, ALTO_PANTALLA * 0.1 - 3}, TAMANIO_FUENTE_CUA, 1, color_verde);

            // Entrada de texto
            Rectangle campo_texto = {550, ALTO_PANTALLA * 0.01, 1200, 90};
            DrawRectangleRounded(campo_texto, REDONDEZ, SEGMENTOS, color_verde);
            DrawTextEx(fuente1, texto_busqueda, (Vector2){campo_texto.x + 20, campo_texto.y + 20}, TAMANIO_FUENTE_SEC, 0, color_blanco);

            // Manejo de entrada
            if (campo_enfocado_busqueda == 0)
            {
                int key = GetCharPressed();
                while (key > 0)
                {
                    if (strlen(texto_busqueda) < sizeof(texto_busqueda) - 1)
                    {
                        if ((key >= 32 && key <= 126) || key == 58)
                        {
                            texto_busqueda[strlen(texto_busqueda)] = (char)key;
                            texto_busqueda[strlen(texto_busqueda) + 1] = '\0';
                        }
                    }
                    key = GetCharPressed();
                }

                if (IsKeyPressed(KEY_BACKSPACE) && strlen(texto_busqueda) > 0)
                {
                    texto_busqueda[strlen(texto_busqueda) - 1] = '\0';
                }
            }

            if (manejar_boton_simple(boton_aceptar_busqueda) && !seleccion_busqueda)
            {
                if (strlen(texto_busqueda) > 0)
                {
                    playlistBusqueda = buscar_canciones(playlist, texto_busqueda, &total_canciones_busqueda);
                    cancion_actual_busqueda = playlistBusqueda;
                    cancion_seleccionada_busqueda = 0; // Siempre empezar desde el primero

                    // Cargar video de la primera canción de búsqueda
                    if (cancion_actual_busqueda != NULL && cancion_actual_busqueda->video[0] != '\0')
                    {
                        generar_rutas(rutas, cancion_actual_busqueda->video);
                        cargar_frames(frames, &total_frames, rutas);
                        frame_actual = 0;
                        tiempo_frame = 0;
                    }

                    int inicio = calcular_inicio_para_centrar(cancion_seleccionada_busqueda, total_canciones_busqueda);
                    int maxInicio = (total_canciones_busqueda - CANCIONES_VISIBLES > 0) ? (total_canciones_busqueda - CANCIONES_VISIBLES) : 0;
                    scroll.inicio = (inicio < 0) ? 0 : (inicio > maxInicio ? maxInicio : inicio);
                    scroll.target_inicio = scroll.inicio;

                    una_vez_busqueda = false;
                }
            }

            // Dibujar y manejar el botón para cerrar la búsqueda
            if (manejar_boton_simple(boton_cancelar_busqueda))
            {
                busqueda = false;
                una_vez_busqueda = true;
                playlistBusqueda = NULL;
                total_canciones_busqueda = 0;
                texto_busqueda[0] = '\0';
            }
        }
        else // Si la búsqueda NO está activa
        {
            // Dibujar ambos botones normalmente
            if (manejar_boton_simple(boton_agregar))
            {
                formulario(&playlist, &total_canciones, fondo, fuente_titulo, fuente1, fuente2);
            }

            if (manejar_boton_simple(boton_buscar) || IsKeyPressed(KEY_F))
            {
                busqueda = true;
            }
        }

        // PARTE CENTRAL
        if (busqueda)
        {
            if (multimedia)
            {
                secciones_visuales_video(&playlistBusqueda, cancion_actual, total_canciones, scroll, cancion_seleccionada, esta_reproduciendo, fuente1, fuente2);
                ver_video(frames, total_frames, &frame_actual, &tiempo_frame, intervalo, x, y, 0.68f, esta_reproduciendo);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) * 2 + 30, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
                DrawTextEx(fuente2, "VISUAL", (Vector2){478, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);
                DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
                if (CheckCollisionPointRec(posicion_mouse, (Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    multimedia = false;
                }
            }
            else if (!multimedia && !nombre && !artista && !duracion)
            {
                secciones_visuales_musica(&playlistBusqueda, cancion_actual, total_canciones, scroll, cancion_seleccionada, esta_reproduciendo, fuente1, fuente2);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
                DrawTextEx(fuente2, "VISUAL", (Vector2){478, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
                DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);

                // Manejo de eliminación con confirmación
                if (manejar_boton_simple(boton_eliminar) || IsKeyPressed(KEY_DELETE))
                {
                    if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                    {
                        // Segunda pulsación dentro del tiempo: eliminar
                        eliminar_cancion_actual(&cancion_actual, &playlist, &total_canciones, &esta_reproduciendo);
                        confirmar_eliminacion = false;
                        esta_reproduciendo = true;
                    }
                    else
                    {
                        // Primera pulsación: activar confirmación
                        confirmar_eliminacion = true;
                        tiempo_confirmacion = GetTime();
                    }
                }

                // Dibujar confirmación de eliminación (esto está bien)
                if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                {
                    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA / 2 - 200, ALTO_PANTALLA / 2 - 50, 445, 100}, REDONDEZ, SEGMENTOS, color_verde);
                    DrawTextEx(fuente2, "ELIMINAR CANCION?", (Vector2){ANCHO_PANTALLA / 2 - 180, ALTO_PANTALLA / 2 - 40}, TAMANIO_FUENTE_TER, 0, color_blanco);
                    DrawTextEx(fuente2, "PULSE DE NUEVO PARA CONFIRMAR", (Vector2){ANCHO_PANTALLA / 2 - 180, ALTO_PANTALLA / 2}, TAMANIO_FUENTE_CUA, 0, color_blanco);

                    // Contador regresivo
                    int segundos_restantes = 3 - (int)(GetTime() - tiempo_confirmacion);
                    DrawTextEx(fuente2, TextFormat("%d", segundos_restantes), (Vector2){ANCHO_PANTALLA / 2 + 150, ALTO_PANTALLA / 2 - 40}, TAMANIO_FUENTE_TER, 0, RED);
                }
                else if (confirmar_eliminacion)
                {
                    // Tiempo de confirmación expirado
                    confirmar_eliminacion = false;
                }

                if (CheckCollisionPointRec(posicion_mouse, (Rectangle){((ANCHO_PANTALLA * 0.4) / 4) * 2 + 30, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    multimedia = true;
                }
            }
        }
        else
        {
            if (multimedia)
            {
                secciones_visuales_video(&playlist, cancion_actual, total_canciones, scroll, cancion_seleccionada, esta_reproduciendo, fuente1, fuente2);
                ver_video(frames, total_frames, &frame_actual, &tiempo_frame, intervalo, x, y, 0.68f, esta_reproduciendo);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) * 2 + 30, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
                DrawTextEx(fuente2, "VISUAL", (Vector2){478, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);
                DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
                if (CheckCollisionPointRec(posicion_mouse, (Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    multimedia = false;
                }
            }
            else if (!multimedia && !nombre && !artista && !duracion)
            {
                secciones_visuales_musica(&playlist, cancion_actual, total_canciones, scroll, cancion_seleccionada, esta_reproduciendo, fuente1, fuente2);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
                DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
                DrawTextEx(fuente2, "VISUAL", (Vector2){478, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
                DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);

                // Manejo de eliminación con confirmación
                if (manejar_boton_simple(boton_eliminar) || IsKeyPressed(KEY_DELETE))
                {
                    if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                    {
                        // Segunda pulsación dentro del tiempo: eliminar
                        eliminar_cancion_actual(&cancion_actual, &playlist, &total_canciones, &esta_reproduciendo);
                        confirmar_eliminacion = false;
                        esta_reproduciendo = true;
                    }
                    else
                    {
                        // Primera pulsación: activar confirmación
                        confirmar_eliminacion = true;
                        tiempo_confirmacion = GetTime();
                    }
                }

                // Dibujar confirmación de eliminación (esto está bien)
                if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                {
                    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA / 2 - 200, ALTO_PANTALLA / 2 - 50, 445, 100}, REDONDEZ, SEGMENTOS, color_verde);
                    DrawTextEx(fuente2, "ELIMINAR CANCION?", (Vector2){ANCHO_PANTALLA / 2 - 180, ALTO_PANTALLA / 2 - 40}, TAMANIO_FUENTE_TER, 0, color_blanco);
                    DrawTextEx(fuente2, "PULSE DE NUEVO PARA CONFIRMAR", (Vector2){ANCHO_PANTALLA / 2 - 180, ALTO_PANTALLA / 2}, TAMANIO_FUENTE_CUA, 0, color_blanco);

                    // Contador regresivo
                    int segundos_restantes = 3 - (int)(GetTime() - tiempo_confirmacion);
                    DrawTextEx(fuente2, TextFormat("%d", segundos_restantes), (Vector2){ANCHO_PANTALLA / 2 + 150, ALTO_PANTALLA / 2 - 40}, TAMANIO_FUENTE_TER, 0, RED);
                }
                else if (confirmar_eliminacion)
                {
                    // Tiempo de confirmación expirado
                    confirmar_eliminacion = false;
                }

                if (CheckCollisionPointRec(posicion_mouse, (Rectangle){((ANCHO_PANTALLA * 0.4) / 4) * 2 + 30, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                {
                    multimedia = true;
                }
            }
        }

        // --------- Reemplaza el bloque if (nombre) con éste ---------
        if (nombre)
        {
            if (una_vez_nombre)
            {
                // Crear la lista ordenada y llenar el total ANTES de calcular índices
                playlistNombre = ordenarListaNombre(playlist);
                llenar_lista_canciones(&playlistNombre, &total_canciones_nombre);

                // Inicializa cancion_actual_nombre al primer elemento real (NO al header)
                cancion_actual_nombre = playlistNombre; // fallback por seguridad

                // Obtener índice correcto ahora que total y puntero están bien
                cancion_seleccionada_nombre = obtener_indice_cancion(playlistNombre, cancion_actual_nombre, total_canciones_nombre);

                // Calcular scroll con valores válidos y clamping para no salirse de rango
                int inicio = calcular_inicio_para_centrar(cancion_seleccionada_nombre, total_canciones_nombre);
                if (inicio < 0)
                    inicio = 0;
                int maxInicio = (total_canciones_nombre - CANCIONES_VISIBLES > 0) ? (total_canciones_nombre - CANCIONES_VISIBLES) : 0;
                if (inicio > maxInicio)
                    inicio = maxInicio;

                scroll.inicio = inicio;
                scroll.target_inicio = inicio;

                una_vez_nombre = false;
            }

            cancion_seleccionada_nombre = obtener_indice_cancion(playlistNombre, cancion_actual_nombre, total_canciones_nombre);
            secciones_visuales_musica(&playlistNombre, cancion_actual_nombre, total_canciones_nombre, scroll, cancion_seleccionada_nombre, esta_reproduciendo, fuente1, fuente2);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){485, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
            DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);

            // Manejo de eliminación con confirmación
            if (manejar_boton_simple(boton_eliminar) || IsKeyPressed(KEY_DELETE))
            {
                if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                {
                    // Segunda pulsación dentro del tiempo: eliminar
                    eliminar_cancion_actual(&cancion_actual_nombre, &playlistNombre, &total_canciones_nombre, &esta_reproduciendo);
                    confirmar_eliminacion = false;
                }
                else
                {
                    // Primera pulsación: activar confirmación
                    confirmar_eliminacion = true;
                    tiempo_confirmacion = GetTime();
                }
            }

            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN) || manejar_boton_simple(boton_adelantar))
            {
                if (total_canciones_nombre > 0)
                {
                    CancionPTR siguiente = siguiente_cancion(cancion_actual_nombre);
                    cambiar_cancion_actual(&cancion_actual_nombre, siguiente, &esta_reproduciendo);

                    cancion_seleccionada_nombre = obtener_indice_cancion(playlistNombre, cancion_actual_nombre, total_canciones_nombre);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_nombre, total_canciones_nombre);
                    ;
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || manejar_boton_simple(boton_retroceder))
            {
                if (total_canciones_nombre > 0)
                {
                    CancionPTR anterior = anterior_cancion(cancion_actual_nombre);
                    cambiar_cancion_actual(&cancion_actual_nombre, anterior, &esta_reproduciendo);
                    cancion_seleccionada_nombre = obtener_indice_cancion(playlistNombre, cancion_actual_nombre, total_canciones_nombre);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_nombre, total_canciones_nombre);
                    ;
                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }
        }
        // -------------------- BLOQUE ARTISTA --------------------
        if (artista)
        {
            if (una_vez_artist)
            {
                // Crear la lista ordenada por artista y llenar el total ANTES de calcular índices
                // (usa la función de ordenación que tengas para artista; si no existe, reemplaza por la que corresponda)
                playlistArtist = ordenarListaArtist(playlist); // <- asegúrate que exista; si no, usa la que tengas
                llenar_lista_canciones(&playlistArtist, &total_canciones_artist);

                // Inicializa cancion_actual_artist al primer elemento real (NO al header)
                cancion_actual_artist = playlistArtist; // fallback por seguridad

                // Obtener índice correcto ahora que total y puntero están bien
                cancion_seleccionada_artist = obtener_indice_cancion(playlistArtist, cancion_actual_artist, total_canciones_artist);

                // Calcular scroll con valores válidos y clamping para no salirse de rango
                int inicio = calcular_inicio_para_centrar(cancion_seleccionada_artist, total_canciones_artist);
                if (inicio < 0)
                    inicio = 0;
                int maxInicio = (total_canciones_artist - CANCIONES_VISIBLES > 0) ? (total_canciones_artist - CANCIONES_VISIBLES) : 0;
                if (inicio > maxInicio)
                    inicio = maxInicio;

                scroll.inicio = inicio;
                scroll.target_inicio = inicio;

                una_vez_artist = false;
            }

            // Mantener indice actualizado
            cancion_seleccionada_artist = obtener_indice_cancion(playlistArtist, cancion_actual_artist, total_canciones_artist);

            // Dibujo
            secciones_visuales_musica(&playlistArtist, cancion_actual_artist, total_canciones_artist, scroll, cancion_seleccionada_artist, esta_reproduciendo, fuente1, fuente2);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){485, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
            DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);

            // Eliminación
            if (manejar_boton_simple(boton_eliminar) || IsKeyPressed(KEY_DELETE))
            {
                if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                {
                    eliminar_cancion_actual(&cancion_actual_artist, &playlistArtist, &total_canciones_artist, &esta_reproduciendo);
                    confirmar_eliminacion = false;
                }
                else
                {
                    confirmar_eliminacion = true;
                    tiempo_confirmacion = GetTime();
                }
            }

            // Navegación (derecha/abajo)
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN) || manejar_boton_simple(boton_adelantar))
            {
                if (total_canciones_artist > 0)
                {
                    CancionPTR siguiente = siguiente_cancion(cancion_actual_artist);
                    cambiar_cancion_actual(&cancion_actual_artist, siguiente, &esta_reproduciendo);

                    cancion_seleccionada_artist = obtener_indice_cancion(playlistArtist, cancion_actual_artist, total_canciones_artist);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_artist, total_canciones_artist);
                    if (nuevo_inicio < 0)
                        nuevo_inicio = 0;
                    int maxInicio = (total_canciones_artist - CANCIONES_VISIBLES > 0) ? (total_canciones_artist - CANCIONES_VISIBLES) : 0;
                    if (nuevo_inicio > maxInicio)
                        nuevo_inicio = maxInicio;

                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            // Navegación (izquierda/arriba)
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || manejar_boton_simple(boton_retroceder))
            {
                if (total_canciones_artist > 0)
                {
                    CancionPTR anterior = anterior_cancion(cancion_actual_artist);
                    cambiar_cancion_actual(&cancion_actual_artist, anterior, &esta_reproduciendo);

                    cancion_seleccionada_artist = obtener_indice_cancion(playlistArtist, cancion_actual_artist, total_canciones_artist);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_artist, total_canciones_artist);
                    if (nuevo_inicio < 0)
                        nuevo_inicio = 0;
                    int maxInicio = (total_canciones_artist - CANCIONES_VISIBLES > 0) ? (total_canciones_artist - CANCIONES_VISIBLES) : 0;
                    if (nuevo_inicio > maxInicio)
                        nuevo_inicio = maxInicio;

                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }
        }
        // -------------------- FIN BLOQUE ARTISTA --------------------

        // -------------------- BLOQUE DURACION --------------------
        if (duracion)
        {
            if (una_vez_duration)
            {
                // Crear la lista ordenada por duración y llenar el total ANTES de calcular índices
                playlistDuration = ordenarListaDuracion(playlist); // <- asegúrate que exista; si no, adapta
                llenar_lista_canciones(&playlistDuration, &total_canciones_duration);

                // Inicializa cancion_actual_duration al primer elemento real (NO al header)
                cancion_actual_duration = playlistDuration; // fallback por seguridad

                // Obtener índice correcto ahora que total y puntero están bien
                cancion_seleccionada_duration = obtener_indice_cancion(playlistDuration, cancion_actual_duration, total_canciones_duration);

                // Calcular scroll con valores válidos y clamping para no salirse de rango
                int inicio = calcular_inicio_para_centrar(cancion_seleccionada_duration, total_canciones_duration);
                if (inicio < 0)
                    inicio = 0;
                int maxInicio = (total_canciones_duration - CANCIONES_VISIBLES > 0) ? (total_canciones_duration - CANCIONES_VISIBLES) : 0;
                if (inicio > maxInicio)
                    inicio = maxInicio;

                scroll.inicio = inicio;
                scroll.target_inicio = inicio;

                una_vez_duration = false;
            }

            // Mantener indice actualizado
            cancion_seleccionada_duration = obtener_indice_cancion(playlistDuration, cancion_actual_duration, total_canciones_duration);

            // Dibujo
            secciones_visuales_musica(&playlistDuration, cancion_actual_duration, total_canciones_duration, scroll, cancion_seleccionada_duration, esta_reproduciendo, fuente1, fuente2);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){485, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
            DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);

            // Eliminación
            if (manejar_boton_simple(boton_eliminar) || IsKeyPressed(KEY_DELETE))
            {
                if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                {
                    eliminar_cancion_actual(&cancion_actual_duration, &playlistDuration, &total_canciones_duration, &esta_reproduciendo);
                    confirmar_eliminacion = false;
                }
                else
                {
                    confirmar_eliminacion = true;
                    tiempo_confirmacion = GetTime();
                }
            }

            // Navegación (derecha/abajo)
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN) || manejar_boton_simple(boton_adelantar))
            {
                if (total_canciones_duration > 0)
                {
                    CancionPTR siguiente = siguiente_cancion(cancion_actual_duration);
                    cambiar_cancion_actual(&cancion_actual_duration, siguiente, &esta_reproduciendo);

                    cancion_seleccionada_duration = obtener_indice_cancion(playlistDuration, cancion_actual_duration, total_canciones_duration);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_duration, total_canciones_duration);
                    if (nuevo_inicio < 0)
                        nuevo_inicio = 0;
                    int maxInicio = (total_canciones_duration - CANCIONES_VISIBLES > 0) ? (total_canciones_duration - CANCIONES_VISIBLES) : 0;
                    if (nuevo_inicio > maxInicio)
                        nuevo_inicio = maxInicio;

                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            // Navegación (izquierda/arriba)
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || manejar_boton_simple(boton_retroceder))
            {
                if (total_canciones_duration > 0)
                {
                    CancionPTR anterior = anterior_cancion(cancion_actual_duration);
                    cambiar_cancion_actual(&cancion_actual_duration, anterior, &esta_reproduciendo);

                    cancion_seleccionada_duration = obtener_indice_cancion(playlistDuration, cancion_actual_duration, total_canciones_duration);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_duration, total_canciones_duration);
                    if (nuevo_inicio < 0)
                        nuevo_inicio = 0;
                    int maxInicio = (total_canciones_duration - CANCIONES_VISIBLES > 0) ? (total_canciones_duration - CANCIONES_VISIBLES) : 0;
                    if (nuevo_inicio > maxInicio)
                        nuevo_inicio = maxInicio;

                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }
        }
        // -------------------- FIN BLOQUE DURACION --------------------

        //============================================================
        if (!una_vez_busqueda && playlistBusqueda != NULL)
        {
            // Mantener índice actualizado
            cancion_seleccionada_busqueda = obtener_indice_cancion(playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda);

            // Dibujo
            secciones_visuales_musica(&playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda, scroll, cancion_seleccionada_busqueda, esta_reproduciendo, fuente1, fuente2);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4), ALTO_PANTALLA * 0.17, (ANCHO_PANTALLA * 0.68) / 3, 50}, REDONDEZ + 0.4, SEGMENTOS, color_verde);
            DrawRectangleRounded((Rectangle){((ANCHO_PANTALLA * 0.4) / 4) + 5, (ALTO_PANTALLA * 0.17) + 5, ((ANCHO_PANTALLA * 0.68) / 6) - 10, 40}, REDONDEZ + 0.4, SEGMENTOS, color_blanco);
            DrawTextEx(fuente2, "VIDEO", (Vector2){485, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_blanco);
            DrawTextEx(fuente2, "AUDIO", (Vector2){260, (ALTO_PANTALLA * 0.17) + 7}, TAMANIO_FUENTE_CUA, 1, color_verde);

            // Eliminación
            if (manejar_boton_simple(boton_eliminar) || IsKeyPressed(KEY_DELETE))
            {
                if (confirmar_eliminacion && (GetTime() - tiempo_confirmacion) < 3.0)
                {
                    eliminar_cancion_actual(&cancion_actual_busqueda, &playlistBusqueda, &total_canciones_busqueda, &esta_reproduciendo);
                    confirmar_eliminacion = false;
                }
                else
                {
                    confirmar_eliminacion = true;
                    tiempo_confirmacion = GetTime();
                }
            }

            // Navegación (derecha/abajo)
            if (IsKeyPressed(KEY_RIGHT) || IsKeyPressed(KEY_DOWN) || manejar_boton_simple(boton_adelantar))
            {
                if (total_canciones_busqueda > 0)
                {
                    CancionPTR siguiente = siguiente_cancion(cancion_actual_busqueda);
                    cambiar_cancion_actual(&cancion_actual_busqueda, siguiente, &esta_reproduciendo);

                    cancion_seleccionada_busqueda = obtener_indice_cancion(playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_busqueda, total_canciones_busqueda);
                    if (nuevo_inicio < 0)
                        nuevo_inicio = 0;
                    int maxInicio = (total_canciones_busqueda - CANCIONES_VISIBLES > 0) ? (total_canciones_busqueda - CANCIONES_VISIBLES) : 0;
                    if (nuevo_inicio > maxInicio)
                        nuevo_inicio = maxInicio;

                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }

            // Navegación (izquierda/arriba)
            if (IsKeyPressed(KEY_LEFT) || IsKeyPressed(KEY_UP) || manejar_boton_simple(boton_retroceder))
            {
                if (total_canciones_busqueda > 0)
                {
                    CancionPTR anterior = anterior_cancion(cancion_actual_busqueda);
                    cambiar_cancion_actual(&cancion_actual_busqueda, anterior, &esta_reproduciendo);

                    cancion_seleccionada_busqueda = obtener_indice_cancion(playlistBusqueda, cancion_actual_busqueda, total_canciones_busqueda);

                    int nuevo_inicio = calcular_inicio_para_centrar(cancion_seleccionada_busqueda, total_canciones_busqueda);
                    if (nuevo_inicio < 0)
                        nuevo_inicio = 0;
                    int maxInicio = (total_canciones_busqueda - CANCIONES_VISIBLES > 0) ? (total_canciones_busqueda - CANCIONES_VISIBLES) : 0;
                    if (nuevo_inicio > maxInicio)
                        nuevo_inicio = maxInicio;

                    if (nuevo_inicio != scroll.inicio)
                    {
                        scroll.target_inicio = nuevo_inicio;
                        scroll.scrolling = true;
                    }
                }
            }
        }

        //=============================================================

        // PARTE INFERIOR
        manejar_boton_simple(boton_retroceder);

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

        if (audio_actual.cargada)
        {
            if (esta_reproduciendo)
            {
                if (IsMusicStreamPlaying(audio_actual.musica) == false)
                {
                    PlayMusicStream(audio_actual.musica);
                }
                UpdateMusicStream(audio_actual.musica);
            }
            else
            {
                PauseMusicStream(audio_actual.musica);
            }
        }

        manejar_boton_simple(boton_adelantar);

        dibujar_linea_tiempo(&audio_actual, fuente2, esta_reproduciendo);

        if (mutear)
        {
            if (manejar_boton_simple(boton_silenciar_volumen))
            {
                mutear = false;
                SetMasterVolume(0.5f); // Restaurar volumen al desactivar mute
            }
        }
        else
        {
            if (manejar_boton_simple(boton_activar_volumen))
            {
                mutear = true;
                SetMasterVolume(0.0f); // Silenciar al activar mute
            }
        }

        EndDrawing();
    }

    UnloadTexture(logo);
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
    liberar_frames(frames, total_frames);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}
