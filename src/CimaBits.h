//  ExpoCiencia y Tecnología 2025
//  CimaBits
//**************************************************************************************************************************
//  LIBRERIA
//**************************************************************************************************************************
#include "estructura.h"
//**************************************************************************************************************************
//  CONSTANTES
//**************************************************************************************************************************
#define ANCHO_FONDO 5472 // 3840
#define ALTO_FONDO 3648  // 2160
#define TAMANIO_FUENTE 80
#define TAMANIO_FUENTE_SEC 60
#define TAMANIO_FUENTE_TER 45
#define TAMANIO_FUENTE_CUA 40
#define TAMANIO_FUENTE_QUI 20
#define REDONDEZ 0.5
#define SEGMENTOS 50
#define CANCIONES_VISIBLES 5
//**************************************************************************************************************************
//  VARIABLES GLOBALES
//**************************************************************************************************************************
Color color_blanco = {255, 255, 255, 255};
Color color_fondo = {255, 255, 255, 100};
Color color_verde = {0, 60, 19, 255};
Color color_verde_claro = {0, 157, 63, 255};
Color color_amarillo = {220, 238, 178, 255};
Color color_campo_activo = {0, 60, 19, 255};
Color color_campo_inactivo = {255, 255, 255, 100};
Color color_texto_activo = {255, 255, 255, 255};
Color color_texto_inactivo = {0, 60, 19, 255};
Color color_boton = {255, 255, 255, 100};
Color color_boton_pulsado = {0, 60, 19, 255};
Color color_casilla_marcada = {0, 60, 19, 255};
Color color_error = {204, 0, 0, 255};
Color color_exito = {0, 128, 0, 255};

Boton_Interfaz boton_buscar;
Boton_Interfaz boton_agregar;
Boton_Interfaz boton_play;
Boton_Interfaz boton_pausa;
Boton_Interfaz boton_retroceder;
Boton_Interfaz boton_adelantar;
Boton_Interfaz boton_activar_volumen;
Boton_Interfaz boton_silenciar_volumen;
Boton_Interfaz boton_eliminar;
Boton_Interfaz boton_aceptar_busqueda;
Boton_Interfaz boton_cancelar_busqueda;
Boton_Interfaz boton_aceptar_forms;
Boton_Interfaz boton_cancelar_forms;

Estado_Imagen imagen_actual = {0};
Estado_Audio audio_actual = {0};
//**************************************************************************************************************************
//  PROTOTIPOS
//**************************************************************************************************************************
Vector2 centrar_texto_horizontal(Font fuente, const char *texto, float tamanio_fuente, float espaciado, float posision_y);
void pantalla_desarrollador(Font fuente1, Font fuente2, Font fuente3);
void secciones_visuales_encabezados();
void secciones_visuales_musica();
void secciones_visuales_video();
void configurar_botones();
int formulario(CancionPTR *playlist, Vector2 posicion_mouse, Texture2D fondo, Font fuente1, Font fuente2, Font fuente3);
void dibujar_tabla_canciones(Font fuente1, Font fuente2, Cancion *playlist, int total_canciones, Estado_Scroll posision_scroll, int cancion_seleccionada, bool esta_reproduciendo);
void dibujar_linea_tiempo(Estado_Audio *audio, Font fuente, bool esta_reproduciendo);

//**************************************************************************************************************************
//  FUNCIONES VISUALES
//**************************************************************************************************************************
Vector2 centrar_texto_horizontal(Font fuente, const char *texto, float tamanio_fuente, float espaciado, float posision_y)
{
    float ancho_texto = MeasureTextEx(fuente, texto, tamanio_fuente, espaciado).x;
    float posision_x = (ANCHO_PANTALLA - ancho_texto) / 2;
    return (Vector2){posision_x, posision_y};
}
//**************************************************************************************************************************
void pantalla_desarrollador(Font fuente1, Font fuente2, Font fuente3)
{
    // VARIABLES LOCALES
    bool tiempo_terminado = false;
    Texture2D logo = LoadTexture("assets/logo.png");
    double tiempoInicio = GetTime();
    double duracion = 4.0;
    float escala = 0.7f;
    float posX = (ANCHO_PANTALLA - logo.width * escala) / 2;
    float posY = (ALTO_PANTALLA - logo.height * escala) / 2;

    // AQUI EMPIEZA EL CODIGO
    SetTextureFilter(logo, TEXTURE_FILTER_BILINEAR);

    while (!WindowShouldClose() && !tiempo_terminado)
    {
        if (GetTime() - tiempoInicio >= duracion)
        {
            tiempo_terminado = true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        DrawTextureEx(logo, (Vector2){posX, posY}, 0.0f, escala, WHITE);
        DrawTextEx(fuente3, "DESARROLLADO PARA:", centrar_texto_horizontal(fuente2, "DESARROLLADO PARA:", TAMANIO_FUENTE_SEC, 1, 720), TAMANIO_FUENTE_SEC, 0, color_verde);
        DrawTextEx(fuente3, "EXPOCIENCIA 2025", centrar_texto_horizontal(fuente2, "EXPOCIENCIA 2025", TAMANIO_FUENTE, 1, 760), TAMANIO_FUENTE, 0, color_verde_claro);

        EndDrawing();
    }

    BeginDrawing();
    EndDrawing();

    UnloadTexture(logo);
}
//**************************************************************************************************************************
void secciones_visuales_encabezados()
{
    DrawTriangle((Vector2){0, 0}, (Vector2){0, 39}, (Vector2){39, 0}, color_blanco);
    DrawTriangle((Vector2){ANCHO_PANTALLA, 0}, (Vector2){ANCHO_PANTALLA - 39, 0}, (Vector2){ANCHO_PANTALLA, 39}, color_blanco);
    DrawRectangleRounded((Rectangle){0, 0, ANCHO_PANTALLA, ALTO_PANTALLA * 0.15}, REDONDEZ + 0.1, SEGMENTOS, color_fondo);
    DrawRectangleRoundedLinesEx((Rectangle){8, 8, ANCHO_PANTALLA - 16, ALTO_PANTALLA * 0.135}, REDONDEZ, SEGMENTOS, 8, color_blanco);

    DrawTriangle((Vector2){0, ALTO_PANTALLA}, (Vector2){39, ALTO_PANTALLA}, (Vector2){0, ALTO_PANTALLA - 39}, color_blanco);
    DrawTriangle((Vector2){ANCHO_PANTALLA, ALTO_PANTALLA}, (Vector2){ANCHO_PANTALLA, ALTO_PANTALLA - 39}, (Vector2){ANCHO_PANTALLA - 39, ALTO_PANTALLA}, color_blanco);
    DrawRectangleRounded((Rectangle){0, ALTO_PANTALLA * 0.85, ANCHO_PANTALLA, ALTO_PANTALLA * 0.15}, REDONDEZ, SEGMENTOS, color_fondo);
    DrawRectangleRoundedLinesEx((Rectangle){8, ALTO_PANTALLA * 0.85 + 8, ANCHO_PANTALLA - 16, ALTO_PANTALLA * 0.135}, REDONDEZ, SEGMENTOS, 8, color_blanco);
}
//**************************************************************************************************************************
void secciones_visuales_musica()
{
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.01, ALTO_PANTALLA * 0.16, ANCHO_PANTALLA * 0.4, ALTO_PANTALLA * 0.68}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);

    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.16, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.298, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.438, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.574, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.712, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
}
//**************************************************************************************************************************
void secciones_visuales_video()
{
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.01, ALTO_PANTALLA * 0.16, ANCHO_PANTALLA * 0.68, ALTO_PANTALLA * 0.68}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);

    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.7, ALTO_PANTALLA * 0.16, ANCHO_PANTALLA * 0.29, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.7, ALTO_PANTALLA * 0.298, ANCHO_PANTALLA * 0.29, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.7, ALTO_PANTALLA * 0.438, ANCHO_PANTALLA * 0.29, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.7, ALTO_PANTALLA * 0.574, ANCHO_PANTALLA * 0.29, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.7, ALTO_PANTALLA * 0.712, ANCHO_PANTALLA * 0.29, ALTO_PANTALLA * 0.128}, REDONDEZ - 0.3, SEGMENTOS, color_fondo);
}
//**************************************************************************************************************************
void configurar_botones()
{
    // Carga las texturas
    Texture2D textura_buscar = LoadTexture("assets/iconos/buscar.png");
    Texture2D textura_agregar = LoadTexture("assets/iconos/agregar.png");
    Texture2D textura_play = LoadTexture("assets/iconos/play.png");
    Texture2D textura_pausa = LoadTexture("assets/iconos/pausa.png");
    Texture2D textura_retroceder = LoadTexture("assets/iconos/regresar.png");
    Texture2D textura_adelantar = LoadTexture("assets/iconos/adelantar.png");
    Texture2D textura_activar = LoadTexture("assets/iconos/subir_volumen.png");
    Texture2D textura_silenciar = LoadTexture("assets/iconos/mutear_volumen.png");
    Texture2D textura_eliminar = LoadTexture("assets/iconos/eliminar.png");
    Texture2D textura_aceptar_busqueda = LoadTexture("assets/iconos/lupa.png");
    Texture2D textura_cancelar_busqueda = LoadTexture("assets/iconos/cancelar.png");

    SetTextureFilter(textura_buscar, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_agregar, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_play, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_pausa, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_retroceder, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_adelantar, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_activar, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_silenciar, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_eliminar, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_aceptar_busqueda, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(textura_cancelar_busqueda, TEXTURE_FILTER_BILINEAR);

    // Factor de escala (25% del tamaño original → 75% más pequeños)
    float escala = 0.75f;

    // Calcula las posiciones y dimensiones para la sección superior
    float margen_superior_y = ALTO_PANTALLA * 0.02;
    float margen_derecho = ANCHO_PANTALLA - 50;
    float espaciado_botones_top = 180 * escala; // ajustado también

    // Botón buscar
    boton_buscar = (Boton_Interfaz){
        .textura = textura_buscar,
        .rect = {
            margen_derecho - (textura_agregar.width * escala) - espaciado_botones_top,
            margen_superior_y,
            textura_agregar.width * escala,
            textura_agregar.height * escala}};

    // Botón agregar
    boton_agregar = (Boton_Interfaz){
        .textura = textura_agregar,
        .rect = {
            margen_derecho - (textura_buscar.width * escala),
            margen_superior_y,
            textura_buscar.width * escala,
            textura_buscar.height * escala}};

    // Botón aceptar busqueda
    boton_aceptar_busqueda = (Boton_Interfaz){
        .textura = textura_aceptar_busqueda,
        .rect = {
            (margen_derecho /*- 200*/) - (textura_aceptar_busqueda.width * escala) - espaciado_botones_top,
            margen_superior_y,
            textura_aceptar_busqueda.width * (escala * 0.75),
            textura_aceptar_busqueda.height * (escala * 0.75)}};

    // Botón cancelar busqueda
    boton_cancelar_busqueda = (Boton_Interfaz){
        .textura = textura_cancelar_busqueda,
        .rect = {
            margen_derecho - (textura_cancelar_busqueda.width * escala),
            margen_superior_y,
            textura_cancelar_busqueda.width * escala,
            textura_cancelar_busqueda.height * escala}};

    // Botón eliminar
    boton_eliminar = (Boton_Interfaz){
        .textura = textura_eliminar,
        .rect = {
            740 - (textura_eliminar.width * escala),  // X: derecha del área - ancho del botón
            850 - (textura_eliminar.height * escala), // Y: parte inferior del área - alto del botón
            textura_eliminar.width * escala,
            textura_eliminar.height * escala}};

    // Calcula las posiciones y dimensiones para la sección inferior
    float margen_inferior_y = ALTO_PANTALLA * 0.80 + 90;
    float centro_pantalla_x = ANCHO_PANTALLA / 3;
    float espaciado_botones_bottom = 150 * escala; // ajustado también

    // Botón play
    boton_play = (Boton_Interfaz){
        .textura = textura_play,
        .rect = {
            centro_pantalla_x / 2 - (textura_play.width * escala / 2),
            margen_inferior_y,
            textura_play.width * escala,
            textura_play.height * escala}};

    boton_pausa = (Boton_Interfaz){
        .textura = textura_pausa,
        .rect = {
            centro_pantalla_x / 2 - (textura_pausa.width * escala / 2),
            margen_inferior_y,
            textura_pausa.width * escala,
            textura_pausa.height * escala}};

    // Botón retroceder
    boton_retroceder = (Boton_Interfaz){
        .textura = textura_retroceder,
        .rect = {
            centro_pantalla_x / 2 - (textura_retroceder.width * escala / 2) - espaciado_botones_bottom,
            margen_inferior_y,
            textura_retroceder.width * escala,
            textura_retroceder.height * escala}};

    // Botón adelantar
    boton_adelantar = (Boton_Interfaz){
        .textura = textura_adelantar,
        .rect = {
            centro_pantalla_x / 2 - (textura_adelantar.width * escala / 2) + espaciado_botones_bottom,
            margen_inferior_y,
            textura_adelantar.width * escala,
            textura_adelantar.height * escala}};

    // Botón activar audio
    boton_activar_volumen = (Boton_Interfaz){
        .textura = textura_activar,
        .rect = {
            centro_pantalla_x * 2.55 - (textura_activar.width * escala / 2) + espaciado_botones_bottom,
            margen_inferior_y,
            textura_activar.width * escala,
            textura_activar.height * escala}};

    // Botón silenciar
    boton_silenciar_volumen = (Boton_Interfaz){
        .textura = textura_silenciar,
        .rect = {
            centro_pantalla_x * 2.55 - (textura_silenciar.width * escala / 2) + espaciado_botones_bottom,
            margen_inferior_y,
            textura_silenciar.width * escala,
            textura_silenciar.height * escala}};
}
//**************************************************************************************************************************
int formulario(CancionPTR *playlist, Vector2 posicion_mouse, Texture2D fondo, Font fuente1, Font fuente2, Font fuente3)
{
    bool formulario_completado = false;
    bool duracion_valida = false;
    int enfocado = 0;
    bool audio_cargado = false;
    bool imagen_cargada = false;
    Texture2D img_portada = {0};
    Sound audio = {0};

    char titulo[65] = {0};
    char artista[45] = {0};
    char duracion[8] = {0};
    char ruta_imagen[255] = {0};
    char ruta_audio[255] = {0};
    char ruta_video[255] = {0};
    int insertar = 0;

    // OPCIONES DE MARCA
    bool casilla_al_inicio_seleccionada = false;
    bool casilla_al_final_seleccionada = false;
    Rectangle casilla_al_inico = {ANCHO_PANTALLA * 0.05, 765, 25, 25};
    Rectangle casilla_al_final = {ANCHO_PANTALLA * 0.05, 795, 25, 25};

    // BOTONES
    
    Rectangle boton_aceptar = {700, ALTO_PANTALLA * 0.85, 200, 60};
    Rectangle boton_cancelar = {1000, ALTO_PANTALLA * 0.85, 200, 60};

    while (!WindowShouldClose() && !formulario_completado)
    {
        // MANEJAR ENTRADA DE TECLADO PARA EL CAMPO EN FOCO
        if (enfocado >= 0 && enfocado <= 5)
        {
            char *current_buffer = NULL;
            int max_length = 0;

            switch (enfocado)
            {
            case 0:
                current_buffer = titulo;
                max_length = 64;
                break;
            case 1:
                current_buffer = artista;
                max_length = 44;
                break;
            case 2:
                current_buffer = ruta_audio;
                max_length = 255;

                break;
            case 3:
                current_buffer = duracion;
                max_length = 7;

                break;
            case 4:
                current_buffer = ruta_imagen;
                max_length = 255;
                break;
            case 5:
                current_buffer = ruta_video;
                max_length = 255;
                break;
            }

            if (current_buffer)
            {
                // Manejar entrada de texto
                int key = GetCharPressed();
                while (key > 0)
                {
                    if (strlen(current_buffer) < max_length)
                    {
                        if ((key >= 32 && key <= 126) || key == 58)
                        {
                            current_buffer[strlen(current_buffer)] = (char)key;
                            current_buffer[strlen(current_buffer) + 1] = '\0';
                        }
                    }
                    key = GetCharPressed();
                }

                // Manejar backspace
                if (IsKeyPressed(KEY_BACKSPACE) && strlen(current_buffer) > 0)
                {
                    current_buffer[strlen(current_buffer) - 1] = '\0';
                }

                // Manejar delete para limpiar todo
                if (IsKeyPressed(KEY_DELETE))
                {
                    current_buffer[0] = '\0';
                }
            }
        }

        // MANEJAR CAMBIO DE FOCO CON TAB
        if (IsKeyPressed(KEY_TAB))
        {
            enfocado = (enfocado + 1) % 6;
        }

        // MANEJAR ENTER PARA CARGAR IMAGEN
        if ((IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 480, 120, 25}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) && enfocado == 3 && strlen(ruta_imagen) > 0)
        {
            if (imagen_cargada)
                UnloadTexture(img_portada);

            img_portada = LoadTexture(ruta_imagen);
            if (img_portada.id > 0)
            {
                imagen_cargada = true;
            }
            else
            {
                imagen_cargada = false;
                printf("Error cargando textura: %s\n", ruta_imagen);
            }
        }

        // MANEJAR ENTER PARA CARGAR AUDIO
        if ((IsKeyPressed(KEY_ENTER) || (CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) && enfocado == 4 && strlen(ruta_audio) > 0)
        {
            if (audio_cargado)
                UnloadSound(audio);

            audio = LoadSound(ruta_audio);
            if (audio.frameCount > 0)
            {
                audio_cargado = true;
            }
            else
            {
                audio_cargado = false;
                printf("Error cargando audio: %s\n", ruta_audio);
            }
        }

        // MANEJAR ARRASTRE Y SOLTADO DE ARCHIVOS
        if (IsFileDropped())
        {
            FilePathList droppedFiles = LoadDroppedFiles();

            for (int i = 0; i < droppedFiles.count; i++)
            {
                const char *extension = GetFileExtension(droppedFiles.paths[i]);

                // Si es un archivo de audio
                if (IsFileExtension(droppedFiles.paths[i], ".mp3") ||
                    IsFileExtension(droppedFiles.paths[i], ".wav") ||
                    IsFileExtension(droppedFiles.paths[i], ".ogg") ||
                    IsFileExtension(droppedFiles.paths[i], ".flac"))
                {
                    if (audio_cargado)
                        UnloadSound(audio);

                    audio = LoadSound(droppedFiles.paths[i]);
                    if (audio.frameCount > 0)
                    {
                        audio_cargado = true;
                        strcpy(ruta_audio, droppedFiles.paths[i]);

                        // Obtener la duración en segundos
                        float duracion_segundos = (float)audio.frameCount / (float)audio.stream.sampleRate;
                        int minutos = (int)(duracion_segundos / 60);
                        int segundos = (int)fmod(duracion_segundos, 60);

                        // Formatear la duración en el buffer duracion
                        snprintf(duracion, 8, "%02d:%02d", minutos, segundos);
                    }
                }
                // Si es una imagen
                else if (IsFileExtension(droppedFiles.paths[i], ".png") ||
                         IsFileExtension(droppedFiles.paths[i], ".jpg") ||
                         IsFileExtension(droppedFiles.paths[i], ".jpeg") ||
                         IsFileExtension(droppedFiles.paths[i], ".bmp") ||
                         IsFileExtension(droppedFiles.paths[i], ".tga"))
                {
                    if (imagen_cargada)
                        UnloadTexture(img_portada);

                    img_portada = LoadTexture(droppedFiles.paths[i]);
                    if (img_portada.id > 0)
                    {
                        imagen_cargada = true;
                        strcpy(ruta_imagen, droppedFiles.paths[i]);
                    }
                }
            }

            UnloadDroppedFiles(droppedFiles);
        }

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            // Cambiar focus al hacer clic en campos
            if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.05, 290, ANCHO_PANTALLA * 0.425, 60}))
                enfocado = 0;
            else if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.05, 400, ANCHO_PANTALLA * 0.425, 60}))
                enfocado = 1;
            else if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.05, 510, ANCHO_PANTALLA * 0.425, 60}))
                enfocado = 2;
            else if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.05, 650, 400, 60}))
                enfocado = 3;
            else if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.525, 510, ANCHO_PANTALLA * 0.425, 60}))
                enfocado = 4;
            else if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.525, 650, ANCHO_PANTALLA * 0.425, 60}))
                enfocado = 5;

            // Casillas de radio
            else if (CheckCollisionPointRec(posicion_mouse, casilla_al_inico))
            {
                casilla_al_inicio_seleccionada = true;
                casilla_al_final_seleccionada = false;
                insertar = 1;
            }
            else if (CheckCollisionPointRec(posicion_mouse, casilla_al_final))
            {
                casilla_al_inicio_seleccionada = false;
                casilla_al_final_seleccionada = true;
                insertar = 2;
            }

            // Botones
            else if (CheckCollisionPointRec(posicion_mouse, boton_aceptar))
            {
                duracion_valida = validar_duracion(duracion);
                if (strlen(titulo) > 0 && strlen(artista) > 0 &&
                    duracion_valida && audio_cargado)
                {
                    formulario_completado = true;
                }
            }
            else if (CheckCollisionPointRec(posicion_mouse, boton_cancelar))
            {
                if (imagen_cargada)
                    UnloadTexture(img_portada);
                if (audio_cargado)
                    UnloadSound(audio);
                return 0; // Cancelado
            }
        }

        // VALIDAR DURACIÓN EN TIEMPO REAL
        if (enfocado == 2 && strlen(duracion) > 0)
        {
            duracion_valida = validar_duracion(duracion);
        }

        // DIBUJAR
        BeginDrawing();
        ClearBackground(RAYWHITE);
        float escala = fmaxf((float)ANCHO_PANTALLA / ANCHO_FONDO, (float)ALTO_PANTALLA / ALTO_FONDO);
        DrawTexturePro(fondo, (Rectangle){0, 0, ANCHO_FONDO, ALTO_FONDO},
                       (Rectangle){(ANCHO_PANTALLA - ANCHO_FONDO * escala) * 0.5f, (ALTO_PANTALLA - ALTO_FONDO * escala) * 0.5f, ANCHO_FONDO * escala, ALTO_FONDO * escala},
                       (Vector2){0, 0}, 0.0f, WHITE);

        // [CÓDIGO DE DIBUJO DE LA INTERFAZ COMPLETO]
        // TÍTULO DEL FORMULARIO
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.15, 80, ANCHO_PANTALLA * 0.7, 80}, REDONDEZ, SEGMENTOS, color_fondo);
        DrawTextEx(fuente1, "AGREGAR NUEVA CANCION", centrar_texto_horizontal(fuente1, "AGREGAR NUEVA CANCION", TAMANIO_FUENTE, 1, 80), TAMANIO_FUENTE, 1, color_verde);

        // ETIQUETAS ARRIBA DE LOS CAMPOS
        DrawTextEx(fuente2, "TITULO DE LA CANCION:", (Vector2){ANCHO_PANTALLA * 0.05, 250}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 290, ANCHO_PANTALLA * 0.425, 60}, REDONDEZ, SEGMENTOS, enfocado == 0 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, titulo, (Vector2){ANCHO_PANTALLA * 0.06, 300}, TAMANIO_FUENTE_CUA, 0, enfocado == 0 ? color_texto_activo : color_texto_inactivo);

        DrawTextEx(fuente2, "NOMBRE DEL ARTISTA:", (Vector2){ANCHO_PANTALLA * 0.05, 360}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 400, ANCHO_PANTALLA * 0.425, 60}, REDONDEZ, SEGMENTOS, enfocado == 1 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, artista, (Vector2){ANCHO_PANTALLA * 0.06, 410}, TAMANIO_FUENTE_CUA, 0, enfocado == 1 ? color_texto_activo : color_texto_inactivo);

        // CAMPO PARA AUDIO
        DrawTextEx(fuente2, "AUDIO:", (Vector2){ANCHO_PANTALLA * 0.05, 470}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawTextEx(fuente2, "(RUTA DEL ARCHIVO)", (Vector2){ANCHO_PANTALLA * 0.1, 488}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 510, ANCHO_PANTALLA * 0.425, 60}, REDONDEZ, SEGMENTOS, enfocado == 2 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, ruta_audio, (Vector2){ANCHO_PANTALLA * 0.06, 520}, TAMANIO_FUENTE_CUA, 0, enfocado == 2 ? color_texto_activo : color_texto_inactivo);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 580, 110, 25}, REDONDEZ, SEGMENTOS, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CARGAR AUDIO", (Vector2){ANCHO_PANTALLA * 0.06, 585}, TAMANIO_FUENTE_QUI, 0, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_texto_activo : color_texto_inactivo);

        // Indicador de audio cargado
        if (audio_cargado)
        {
            DrawTextEx(fuente2, "AUDIO CARGADO", (Vector2){970, 620}, TAMANIO_FUENTE_QUI, 0, color_exito);
        }

        // CAMPO DE DURACIÓN CON VALIDACIÓN VISUAL
        Color color_duracion = enfocado == 2 ? color_campo_activo : color_campo_inactivo;

        DrawTextEx(fuente2, "DURACION [MM:SS]:", (Vector2){ANCHO_PANTALLA * 0.05, 610}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 650, 400, 60}, REDONDEZ, SEGMENTOS, enfocado == 3 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, duracion, (Vector2){ANCHO_PANTALLA * 0.06, 660}, TAMANIO_FUENTE_CUA, 0, enfocado == 3 ? color_texto_activo : color_texto_inactivo);

        if (strlen(duracion) > 0 && !duracion_valida)
        {
            color_duracion = color_error;
        }

        // MENSAJE DE ERROR SI LA DURACIÓN NO ES VÁLIDA
        if (strlen(duracion) > 0 && !duracion_valida)
        {
            DrawTextEx(fuente3, "FORMATO INVALIDO. \nUSE MM:SS", (Vector2){520, 660}, TAMANIO_FUENTE_QUI, 0, color_error);
        }

        // CASILLAS DE MARCA (RADIO BUTTONS)
        DrawTextEx(fuente2, "SELECCIONE UNA OPCION:", (Vector2){ANCHO_PANTALLA * 0.05, 720}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded(casilla_al_inico, REDONDEZ, SEGMENTOS, casilla_al_inicio_seleccionada ? color_casilla_marcada : color_campo_inactivo);
        DrawTextEx(fuente2, "INSERTAR AL INICIO", (Vector2){ANCHO_PANTALLA * 0.07, 760}, TAMANIO_FUENTE_CUA, 0, color_verde);
        DrawRectangleRounded(casilla_al_final, REDONDEZ, SEGMENTOS, casilla_al_final_seleccionada ? color_casilla_marcada : color_campo_inactivo);
        DrawTextEx(fuente2, "INSERTAR AL FINAL", (Vector2){ANCHO_PANTALLA * 0.07, 790}, TAMANIO_FUENTE_CUA, 0, color_verde);

        // IMAGEN
        DrawTextEx(fuente2, "IMAGEN:", (Vector2){ANCHO_PANTALLA * 0.525, 470}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawTextEx(fuente2, "[ RUTA DEL ARCHIVO ]", (Vector2){ANCHO_PANTALLA * 0.575, 488}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 510, ANCHO_PANTALLA * 0.425, 60}, REDONDEZ, SEGMENTOS, enfocado == 4 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, ruta_imagen, (Vector2){ANCHO_PANTALLA * 0.535, 520}, TAMANIO_FUENTE_CUA, 0, enfocado == 4 ? color_texto_activo : color_texto_inactivo);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 580, 120, 25}, REDONDEZ, SEGMENTOS, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 480, 120, 25}) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CARGAR IMAGEN", (Vector2){ANCHO_PANTALLA * 0.535, 585}, TAMANIO_FUENTE_QUI, 0, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 480, 120, 25}) ? color_texto_activo : color_texto_inactivo);

        DrawRectangle(ANCHO_PANTALLA * 0.795, 170, 300, 300, CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.76481, 170, 200, 200}) ? color_boton_pulsado : color_boton);

        SetTextureFilter(img_portada, TEXTURE_FILTER_BILINEAR);
        // Mostrar imagen cargada con tamaño 200x200
        if (imagen_cargada)
        {
            Rectangle dest = {ANCHO_PANTALLA * 0.795, 170, 300, 300};
            Rectangle source = {0, 0, img_portada.width, img_portada.height};
            DrawTexturePro(img_portada, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
        }

        // CAMPO PARA VIDEO
        DrawTextEx(fuente2, "VIDEO:", (Vector2){ANCHO_PANTALLA * 0.525, 610}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawTextEx(fuente2, "(RUTA DEL ARCHIVO)", (Vector2){ANCHO_PANTALLA * 0.565, 628}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 650, ANCHO_PANTALLA * 0.425, 60}, REDONDEZ, SEGMENTOS, enfocado == 5 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, ruta_video, (Vector2){ANCHO_PANTALLA * 0.535, 660}, TAMANIO_FUENTE_CUA, 0, enfocado == 5 ? color_texto_activo : color_texto_inactivo);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 720, 110, 25}, REDONDEZ, SEGMENTOS, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CARGAR VIDEO", (Vector2){ANCHO_PANTALLA * 0.535, 725}, TAMANIO_FUENTE_QUI, 0, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_texto_activo : color_texto_inactivo);

        // BOTONES ACEPTAR Y CANCELAR
        DrawRectangleRounded(boton_aceptar, REDONDEZ, SEGMENTOS, CheckCollisionPointRec(posicion_mouse, boton_aceptar) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "ACEPTAR", (Vector2){boton_aceptar.x + 45, boton_aceptar.y + 10}, TAMANIO_FUENTE_TER, 0, CheckCollisionPointRec(posicion_mouse, boton_aceptar) ? color_texto_activo : color_texto_inactivo);

        DrawRectangleRounded(boton_cancelar, REDONDEZ, SEGMENTOS, CheckCollisionPointRec(posicion_mouse, boton_cancelar) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CANCELAR", (Vector2){boton_cancelar.x + 40, boton_cancelar.y + 10}, TAMANIO_FUENTE_TER, 0, CheckCollisionPointRec(posicion_mouse, boton_cancelar) ? color_texto_activo : color_texto_inactivo);

        // Mensaje de instrucción para arrastrar archivos
        DrawTextEx(fuente3, "ARRASTRA ARCHIVOS DEL AUDIO, IMAGEN Y VIDEO AQUI", centrar_texto_horizontal(fuente3, "ARRASTRA ARCHIVOS DEL AUDIO, IMAGEN Y VIDEO AQUI", TAMANIO_FUENTE_CUA, 1, 830), TAMANIO_FUENTE_CUA, 1, color_verde);

        EndDrawing();
    }

    // PROCESAR RESULTADO
    if (formulario_completado)
    {
        printf("entre");
        agregar_cancion(playlist, titulo, artista, duracion, ruta_imagen, ruta_audio, ruta_video, insertar);
        mostrarPlaylist(*playlist);
        return 1; // Éxito
    }
}
//**************************************************************************************************************************
void dibujar_tabla_canciones(Font fuente1, Font fuente2, Cancion *playlist, int total_canciones, Estado_Scroll posision_scroll, int cancion_seleccionada, bool esta_reproduciendo)
{
    const int tabla_x = 550;
    const int tabla_y = 150;
    const int altura_fila = 122;
    const int ancho_columna = 1050;

    static Cancion *ultima_cancion_seleccionada = NULL;

    if (playlist == NULL)
    {
        DrawTextEx(fuente2, "No hay canciones para mostrar", (Vector2){550, 200}, TAMANIO_FUENTE_TER, 0, RED);
        return;
    }

    // Obtener el puntero al primer nodo visible
    Cancion *actual = playlist;
    for (int i = 0; i < posision_scroll.inicio; i++)
    {
        if (actual == NULL)
            break;
        actual = actual->siguiente;
    }

    // Dibujar filas de canciones visibles con desplazamiento suave
    for (int i = 0; i < CANCIONES_VISIBLES; i++)
    {
        if (actual == NULL)
            break;

        int indice = posision_scroll.inicio + i;
        int y = tabla_y + i * altura_fila;

        // Solo dibujar si está dentro del área visible
        if (y < tabla_y - altura_fila || y > tabla_y + (CANCIONES_VISIBLES * altura_fila))
        {
            actual = actual->siguiente;
            continue;
        }

        // Resaltar la canción seleccionada
        if (indice == cancion_seleccionada)
        {
            DrawRectangleRounded((Rectangle){tabla_x, y, ancho_columna - 6, altura_fila - 6}, REDONDEZ, SEGMENTOS, color_verde);
            ultima_cancion_seleccionada = actual; // Guardar la canción seleccionada
        }
        else
        {
            DrawRectangleRounded((Rectangle){tabla_x, y, ancho_columna - 6, altura_fila - 6}, REDONDEZ, SEGMENTOS, color_fondo);
        }

        // Dibujar texto del nodo actual
        if (indice == cancion_seleccionada)
        {
            DrawTextEx(fuente2, actual->titulo, (Vector2){tabla_x + 35, y + 30}, TAMANIO_FUENTE_TER, 0, WHITE);
            DrawTextEx(fuente2, actual->artista, (Vector2){tabla_x + 35, y + 60}, TAMANIO_FUENTE_TER, 0, WHITE);
            DrawTextEx(fuente2, actual->duracion, (Vector2){tabla_x + 980, y + 45}, TAMANIO_FUENTE_TER, 0, WHITE);
        }
        else
        {
            DrawTextEx(fuente2, actual->titulo, (Vector2){tabla_x + 35, y + 30}, TAMANIO_FUENTE_TER, 0, color_verde);
            DrawTextEx(fuente2, actual->artista, (Vector2){tabla_x + 35, y + 60}, TAMANIO_FUENTE_TER, 0, color_verde);
            DrawTextEx(fuente2, actual->duracion, (Vector2){tabla_x + 980, y + 45}, TAMANIO_FUENTE_TER, 0, color_verde);
        }

        // Avanzar al siguiente nodo de la lista
        if (actual->siguiente != NULL && actual->siguiente != playlist)
        {
            actual = actual->siguiente;
        }
        else
        {
            // Si es el último nodo de una lista no circular, salir del loop
            break;
        }
    }

    // Manejo de la canción seleccionada (textura y audio)
    if (ultima_cancion_seleccionada != NULL)
    {
        // Cargar textura solo si es diferente a la actual
        if (strcmp(ultima_cancion_seleccionada->imagen, imagen_actual.ruta) != 0)
        {
            if (imagen_actual.cargada)
            {
                UnloadTexture(imagen_actual.imagen);
            }
            imagen_actual.imagen = LoadTexture(ultima_cancion_seleccionada->imagen);
            imagen_actual.cargada = true;
            strcpy(imagen_actual.ruta, ultima_cancion_seleccionada->imagen);
            SetTextureFilter(imagen_actual.imagen, TEXTURE_FILTER_BILINEAR);
        }
        // Dibujar la textura de la portada
        if (imagen_actual.cargada)
        {
            DrawTexturePro(imagen_actual.imagen, (Rectangle){0, 0, imagen_actual.imagen.width, imagen_actual.imagen.height}, (Rectangle){145, 160, 250, 250}, (Vector2){0, 0}, 0, WHITE);
        }
        // Manejo de audio
        if (strcmp(ultima_cancion_seleccionada->audio, audio_actual.ruta) != 0)
        {
            if (audio_actual.cargada)
            {
                StopMusicStream(audio_actual.musica);
                UnloadMusicStream(audio_actual.musica);
            }
            audio_actual.musica = LoadMusicStream(ultima_cancion_seleccionada->audio);
            audio_actual.cargada = true;
            strcpy(audio_actual.ruta, ultima_cancion_seleccionada->audio);
            audio_actual.duracion = GetMusicTimeLength(audio_actual.musica);
            audio_actual.tiempo_actual = 0;
        }

        // Dibujar información de la canción seleccionada
        DrawTextEx(fuente1, "NOMBRE DE LA CANCION:", (Vector2){60, 440}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawTextEx(fuente2, ultima_cancion_seleccionada->titulo, (Vector2){60, 470}, TAMANIO_FUENTE_SEC, 0, color_verde);
        DrawTextEx(fuente1, "NOMBRE DEL ARTISTA:", (Vector2){60, 520}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawTextEx(fuente2, ultima_cancion_seleccionada->artista, (Vector2){60, 550}, TAMANIO_FUENTE_SEC, 0, color_verde);
        DrawTextEx(fuente1, "DURACION", (Vector2){60, 600}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawTextEx(fuente2, ultima_cancion_seleccionada->duracion, (Vector2){60, 630}, TAMANIO_FUENTE_SEC, 0, color_verde);
    }
}
//**************************************************************************************************************************
void dibujar_linea_tiempo(Estado_Audio *audio, Font fuente, bool esta_reproduciendo)
{
    if (!audio->cargada)
        return;

    const int barra_x = 550;
    const int barra_y = ALTO_PANTALLA * 0.85 + 50;
    const int barra_width = 800;
    const int barra_height = 35;

    // Actualizar tiempo actual si está reproduciendo
    if (esta_reproduciendo)
    {
        audio->tiempo_actual = GetMusicTimePlayed(audio->musica);
        if (audio->tiempo_actual >= audio->duracion)
        {
            // Canción terminada, pasar a la siguiente
            audio->tiempo_actual = 0;
        }
    }

    // Dibujar barra de progreso
    DrawRectangleRounded((Rectangle){barra_x, barra_y, barra_width, barra_height}, REDONDEZ + 0.3, SEGMENTOS, color_amarillo);
    float progreso = (audio->duracion > 0) ? (audio->tiempo_actual / audio->duracion) : 0;
    DrawRectangleRounded((Rectangle){barra_x, barra_y, barra_width * progreso, barra_height}, REDONDEZ + 0.3, SEGMENTOS + 90, color_verde);

    // Dibujar tiempos
    char tiempo_actual_str[10];
    char duracion_str[10];

    int minutos_actual = (int)audio->tiempo_actual / 60;
    int segundos_actual = (int)audio->tiempo_actual % 60;
    int minutos_total = (int)audio->duracion / 60;
    int segundos_total = (int)audio->duracion % 60;

    sprintf(tiempo_actual_str, "%d:%02d", minutos_actual, segundos_actual);
    sprintf(duracion_str, "%d:%02d", minutos_total, segundos_total);

    DrawTextEx(fuente, tiempo_actual_str, (Vector2){barra_x - 50, barra_y}, TAMANIO_FUENTE_CUA, 0, color_verde);
    DrawTextEx(fuente, duracion_str, (Vector2){barra_x + barra_width + 10, barra_y}, TAMANIO_FUENTE_CUA, 0, color_verde);
}
//**************************************************************************************************************************
