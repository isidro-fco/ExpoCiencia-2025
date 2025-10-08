// CimaBits - ExpoCiencias 2025
//******************************************************************************************************
// LIBRERIA
#include "estructura.h"

//******************************************************************************************************
// CONSTANTES
#define ANCHO_FONDO 3840
#define ALTO_FONDO 2160
#define TAMANIO_FUENTE 80
#define TAMANIO_FUENTE_SEC 60
#define TAMANIO_FUENTE_TER 45
#define TAMANIO_FUENTE_CUA 40
#define TAMANIO_FUENTE_QUI 20

//******************************************************************************************************
// ESTRUCTURAS
typedef struct
{
    Texture2D textura;
    Rectangle rect;
    bool activo;
} Boton_simple;

//******************************************************************************************************
// VARIABLES GLOBALES
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

float redondez = 0.5f;
int segmentos = 50;

//******************************************************************************************************
// PROTOTIPOS
Vector2 centrar_horizontal(Font font, const char *text, float fontSize, float spacing, float yPosition);
void pantalla_desarrollador(Font fuente1, Font fuente2, Font fuente3);
void secciones_visuales();
int formulario(Texture2D fondo, Font fuente1, Font fuente2, Font fuente3,char *titulo, char *artista, char *duracion, char *ruta_imagen, char *ruta_audio, char *ruta_video, int *insertar);

//******************************************************************************************************
//----------------------------------------- FUNCIONES VISUALES -----------------------------------------
//******************************************************************************************************
Vector2 centrar_horizontal(Font font, const char *text, float fontSize, float spacing, float yPosition)
{
    float textWidth = MeasureTextEx(font, text, fontSize, spacing).x;
    float xPosition = (ANCHO_PANTALLA - textWidth) / 2;
    return (Vector2){xPosition, yPosition};
}
//******************************************************************************************************
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
        DrawTextEx(fuente3, "DESARROLLADO PARA:", centrar_horizontal(fuente2, "DESARROLLADO PARA:", TAMANIO_FUENTE_SEC, 1, 720), TAMANIO_FUENTE_SEC, 0, color_verde);
        DrawTextEx(fuente3, "EXPOCIENCIA 2025", centrar_horizontal(fuente2, "EXPOCIENCIA 2025", TAMANIO_FUENTE, 1, 760), TAMANIO_FUENTE, 0, color_verde_claro);

        EndDrawing();
    }

    BeginDrawing();
    EndDrawing();

    UnloadTexture(logo);
}
//******************************************************************************************************
void secciones_visuales()
{
    DrawRectangleRounded((Rectangle){0, 0, ANCHO_PANTALLA, ALTO_PANTALLA * 0.15}, redondez, segmentos, color_fondo);
    DrawRectangleRec((Rectangle){0, 0, ANCHO_PANTALLA, ALTO_PANTALLA * 0.04}, color_blanco);

    DrawRectangleRounded((Rectangle){0, ALTO_PANTALLA * 0.85, ANCHO_PANTALLA, ALTO_PANTALLA * 0.15}, redondez, segmentos, color_fondo);
    DrawRectangleRec((Rectangle){0, ALTO_PANTALLA * 0.96, ANCHO_PANTALLA, ALTO_PANTALLA * 0.04}, color_blanco);

    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.01, ALTO_PANTALLA * 0.16, ANCHO_PANTALLA * 0.4, ALTO_PANTALLA * 0.68}, redondez - 0.3, segmentos, color_fondo);

    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.16, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, redondez - 0.3, segmentos, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.298, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, redondez - 0.3, segmentos, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.438, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, redondez - 0.3, segmentos, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.574, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, redondez - 0.3, segmentos, color_fondo);
    DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.42, ALTO_PANTALLA * 0.712, ANCHO_PANTALLA * 0.57, ALTO_PANTALLA * 0.128}, redondez - 0.3, segmentos, color_fondo);
}
//******************************************************************************************************
int formulario(Texture2D fondo, Font fuente1, Font fuente2, Font fuente3,char *titulo, char *artista, char *duracion, char *ruta_imagen, char *ruta_audio, char *ruta_video, int *insertar)
{
    bool formulario_completado = false;
    bool duracion_valida = false;
    int enfocado = 0;
    bool audio_cargado = false;
    bool imagen_cargada = false;
    Texture2D img_portada = {0};
    Sound audio = {0};

    // OPCIONES DE MARCA
    bool casilla_al_inicio_seleccionada = false;
    bool casilla_al_final_seleccionada = false;
    Rectangle casilla_al_inico = {ANCHO_PANTALLA * 0.05, 625, 25, 25};
    Rectangle casilla_al_final = {ANCHO_PANTALLA * 0.05, 655, 25, 25};

    // BOTONES
    Rectangle boton_aceptar = {550, ALTO_PANTALLA * 0.85, 200, 60};
    Rectangle boton_cancelar = {850, ALTO_PANTALLA * 0.85, 200, 60};

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

        // VERIFICAR CLICS EN BOTONES Y CAMPOS
        Vector2 posicion_mouse = GetMousePosition();

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
            else if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.525, 400, ANCHO_PANTALLA * 0.425, 60}))
                enfocado = 4;
            else if (CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.525, 680, ANCHO_PANTALLA * 0.425, 60}))
                enfocado = 5;

            // Casillas de radio
            else if (CheckCollisionPointRec(posicion_mouse, casilla_al_inico))
            {
                casilla_al_inicio_seleccionada = true;
                casilla_al_final_seleccionada = false;
            }
            else if (CheckCollisionPointRec(posicion_mouse, casilla_al_final))
            {
                casilla_al_inicio_seleccionada = false;
                casilla_al_final_seleccionada = true;
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
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.15, 80, ANCHO_PANTALLA * 0.7, 80}, redondez, segmentos, color_fondo);
        DrawTextEx(fuente1, "AGREGAR NUEVA CANCION", centrar_horizontal(fuente1, "AGREGAR NUEVA CANCION", TAMANIO_FUENTE, 1, 80), TAMANIO_FUENTE, 1, color_verde);

        // ETIQUETAS ARRIBA DE LOS CAMPOS
        DrawTextEx(fuente2, "TITULO DE LA CANCION:", (Vector2){ANCHO_PANTALLA * 0.05, 250}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 290, ANCHO_PANTALLA * 0.425, 60}, redondez, segmentos, enfocado == 0 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, titulo, (Vector2){ANCHO_PANTALLA * 0.06, 300}, TAMANIO_FUENTE_CUA, 0, enfocado == 0 ? color_texto_activo : color_texto_inactivo);

        DrawTextEx(fuente2, "NOMBRE DEL ARTISTA:", (Vector2){ANCHO_PANTALLA * 0.05, 360}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 400, ANCHO_PANTALLA * 0.425, 60}, redondez, segmentos, enfocado == 1 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, artista, (Vector2){ANCHO_PANTALLA * 0.06, 410}, TAMANIO_FUENTE_CUA, 0, enfocado == 1 ? color_texto_activo : color_texto_inactivo);

        // CAMPO PARA AUDIO
        DrawTextEx(fuente2, "AUDIO:", (Vector2){ANCHO_PANTALLA * 0.05, 470}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawTextEx(fuente2, "(RUTA DEL ARCHIVO)", (Vector2){ANCHO_PANTALLA * 0.1, 488}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 510, ANCHO_PANTALLA * 0.425, 60}, redondez, segmentos, enfocado == 2 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, ruta_audio, (Vector2){ANCHO_PANTALLA * 0.06, 520}, TAMANIO_FUENTE_CUA, 0, enfocado == 2 ? color_texto_activo : color_texto_inactivo);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 580, 110, 25}, redondez, segmentos, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CARGAR AUDIO", (Vector2){ANCHO_PANTALLA * 0.06, 585}, TAMANIO_FUENTE_QUI, 0, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_texto_activo : color_texto_inactivo);

        // Indicador de audio cargado
        if (audio_cargado)
        {
            DrawTextEx(fuente2, "AUDIO CARGADO", (Vector2){970, 620}, TAMANIO_FUENTE_QUI, 0, color_exito);
        }

        // CAMPO DE DURACIÓN CON VALIDACIÓN VISUAL
        Color color_duracion = enfocado == 2 ? color_campo_activo : color_campo_inactivo;

        DrawTextEx(fuente2, "DURACION [MM:SS]:", (Vector2){ANCHO_PANTALLA * 0.05, 610}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.05, 650, 400, 60}, redondez, segmentos, enfocado == 3 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, duracion, (Vector2){ANCHO_PANTALLA * 0.06, 660}, TAMANIO_FUENTE_CUA, 0, enfocado == 3 ? color_texto_activo : color_texto_inactivo);

        if (strlen(duracion) > 0 && !duracion_valida)
        {
            color_duracion = color_error;
        }

        // MENSAJE DE ERROR SI LA DURACIÓN NO ES VÁLIDA
        if (strlen(duracion) > 0 && !duracion_valida)
        {
            DrawTextEx(fuente3, "FORMATO INVALIDO. \nUSE MM:SS", (Vector2){520, 620}, TAMANIO_FUENTE_QUI, 0, color_error);
        }

        // CASILLAS DE MARCA (RADIO BUTTONS)
        DrawTextEx(fuente2, "SELECCIONE UNA OPCION:", (Vector2){ANCHO_PANTALLA * 0.05, 780}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawRectangleRounded(casilla_al_inico, redondez, segmentos, casilla_al_inicio_seleccionada ? color_casilla_marcada : color_campo_inactivo);
        DrawTextEx(fuente2, "INSERTAR AL INICIO", (Vector2){ANCHO_PANTALLA * 0.07, 720}, TAMANIO_FUENTE_CUA, 0, color_verde);
        DrawRectangleRounded(casilla_al_final, redondez, segmentos, casilla_al_final_seleccionada ? color_casilla_marcada : color_campo_inactivo);
        DrawTextEx(fuente2, "INSERTAR AL FINAL", (Vector2){ANCHO_PANTALLA * 0.07, 750}, TAMANIO_FUENTE_CUA, 0, color_verde);

        // IMAGEN
        DrawTextEx(fuente2, "IMAGEN:", (Vector2){ANCHO_PANTALLA * 0.525, 360}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawTextEx(fuente2, "[ RUTA DEL ARCHIVO ]", (Vector2){ANCHO_PANTALLA * 0.575, 378}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 400, ANCHO_PANTALLA * 0.425, 60}, redondez, segmentos, enfocado == 4 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, ruta_imagen, (Vector2){ANCHO_PANTALLA * 0.535, 410}, TAMANIO_FUENTE_CUA, 0, enfocado == 4 ? color_texto_activo : color_texto_inactivo);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 470, 120, 25}, redondez, segmentos, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 480, 120, 25}) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CARGAR IMAGEN", (Vector2){ANCHO_PANTALLA * 0.535, 475}, TAMANIO_FUENTE_QUI, 0, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 480, 120, 25}) ? color_texto_activo : color_texto_inactivo);

        DrawRectangle(ANCHO_PANTALLA * 0.845, 170, 200, 200, CheckCollisionPointRec(posicion_mouse, (Rectangle){ANCHO_PANTALLA * 0.76481, 170, 200, 200}) ? color_boton_pulsado : color_boton);

        SetTextureFilter(img_portada, TEXTURE_FILTER_BILINEAR);
        // Mostrar imagen cargada con tamaño 200x200
        if (imagen_cargada)
        {
            Rectangle dest = {1200, 170, 200, 200};
            Rectangle source = {0, 0, img_portada.width, img_portada.height};
            DrawTexturePro(img_portada, source, dest, (Vector2){0, 0}, 0.0f, WHITE);
        }

        // CAMPO PARA VIDEO
        DrawTextEx(fuente2, "VIDEO:", (Vector2){ANCHO_PANTALLA * 0.525, 640}, TAMANIO_FUENTE_TER, 0, color_verde);
        DrawTextEx(fuente2, "(RUTA DEL ARCHIVO)", (Vector2){ANCHO_PANTALLA * 0.565, 658}, TAMANIO_FUENTE_QUI, 0, color_verde);
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 680, ANCHO_PANTALLA * 0.425, 60}, redondez, segmentos, enfocado == 5 ? color_campo_activo : color_campo_inactivo);
        DrawTextEx(fuente3, ruta_video, (Vector2){ANCHO_PANTALLA * 0.535, 690}, TAMANIO_FUENTE_CUA, 0, enfocado == 5 ? color_texto_activo : color_texto_inactivo);

        // Botón para cargar audio
        DrawRectangleRounded((Rectangle){ANCHO_PANTALLA * 0.525, 750, 110, 25}, redondez, segmentos, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CARGAR VIDEO", (Vector2){ANCHO_PANTALLA * 0.535, 755}, TAMANIO_FUENTE_QUI, 0, CheckCollisionPointRec(posicion_mouse, (Rectangle){850, 615, 110, 25}) ? color_texto_activo : color_texto_inactivo);

        // BOTONES ACEPTAR Y CANCELAR
        DrawRectangleRounded(boton_aceptar, redondez, segmentos, CheckCollisionPointRec(posicion_mouse, boton_aceptar) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "ACEPTAR", (Vector2){boton_aceptar.x + 35, boton_aceptar.y + 15}, TAMANIO_FUENTE_TER, 0, CheckCollisionPointRec(posicion_mouse, boton_aceptar) ? color_texto_activo : color_texto_inactivo);

        DrawRectangleRounded(boton_cancelar, redondez, segmentos, CheckCollisionPointRec(posicion_mouse, boton_cancelar) ? color_boton_pulsado : color_boton);
        DrawTextEx(fuente2, "CANCELAR", (Vector2){boton_cancelar.x + 30, boton_cancelar.y + 15}, TAMANIO_FUENTE_TER, 0, CheckCollisionPointRec(posicion_mouse, boton_cancelar) ? color_texto_activo : color_texto_inactivo);

        // Mensaje de instrucción para arrastrar archivos
        DrawTextEx(fuente3, "Arrastra archivos de audio o imagen aqui", (Vector2){ANCHO_PANTALLA * 0.05, 700}, TAMANIO_FUENTE_CUA, 0, color_verde);

        EndDrawing();
    }

    // PROCESAR RESULTADO
    if (formulario_completado)
    {
        // Ya no asignamos textura y audio cargados, solo usamos las rutas
        // *insertar_al_inicio = casilla_al_inicio_seleccionada ? 1 : 0;

        return 1; // Éxito
    }

    // Limpiar recursos si se canceló
    if (imagen_cargada)
        UnloadTexture(img_portada);
    if (audio_cargado)
        UnloadSound(audio);
}
//******************************************************************************************************