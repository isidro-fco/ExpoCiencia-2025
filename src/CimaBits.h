#include "estructura.h"

typedef struct
{
    Texture2D textura;
    Rectangle rect;
    bool activo;
} Boton_simple;

Color color_blanco = {255, 255, 255, 255};
Color color_fondo = {255, 255, 255, 100};
Color color_verde = {58, 97, 60, 255};
Color color_amarillo = {220, 238, 178, 255};

float redondez = 0.5f;
int segmentos = 10;

//******************************************************************************************************
// Función para centrar texto horizontalmente
Vector2 centrar_horizontal(Font font, const char *text, float fontSize, float spacing, float yPosition)
{
    float textWidth = MeasureTextEx(font, text, fontSize, spacing).x;
    float xPosition = (ANCHO_PANTALLA - textWidth) / 2;
    return (Vector2){xPosition, yPosition};
}
//******************************************************************************************************

//******************************************************************************************************
void pantalla_desarrollador(Font fuente1, Font fuente2, Font fuente3)
{
    // VARIABLES LOCALES
    bool tiempo_terminado = false;
    Texture2D logo = LoadTexture("Assets/logo.png");
    Music musica = LoadMusicStream("Assets/intro.wav");
    // Control de tiempo
    double tiempoInicio = GetTime();
    double duracion = 8.0;
    float escala = 0.7f;
    float posX = (ANCHO_PANTALLA - logo.width * escala) / 2;
    float posY = (ALTO_PANTALLA - logo.height * escala) / 2;

    // AQUI EMPIEZA EL CODIGO
    SetTextureFilter(logo, TEXTURE_FILTER_BILINEAR);

    SetMusicVolume(musica, 1.0f);
    PlayMusicStream(musica);

    // Bucle principal
    while (!WindowShouldClose() && !tiempo_terminado)
    {

        UpdateMusicStream(musica);

        if (GetTime() - tiempoInicio >= duracion)
        {
            tiempo_terminado = true;
        }

        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Dibujar elementos

        DrawTextureEx(logo, (Vector2){posX, posY}, 0.0f, escala, WHITE);

        DrawTextEx(fuente2, "DESARROLLADO PARA:", centrar_horizontal(fuente2, "DESARROLLADO PARA:", 14, 1, 720), 14, 0, color_verde);

        DrawTextEx(fuente2, "EXPOCIENCIA 2025", centrar_horizontal(fuente2, "EXPOCIENCIA 2025", 14, 1, 760), 14, 0, color_verde);

        EndDrawing();
    }
    // Detener música suavemente
    StopMusicStream(musica);

    // Esperar un frame adicional para que se complete la detención
    BeginDrawing();
    EndDrawing();
    // Limpiar recursos PERO NO CERRAR LA VENTANA
    UnloadTexture(logo);
    UnloadMusicStream(musica);
}
//******************************************************************************************************
void secciones_visuales()
{

    DrawRectangleRounded((Rectangle){0, 0, ANCHO_PANTALLA, ALTO_PANTALLA * 0.15}, redondez, segmentos, color_fondo);
    DrawRectangleRec((Rectangle){0, 0, ANCHO_PANTALLA, ALTO_PANTALLA * 0.04}, color_blanco);

    DrawRectangleRounded((Rectangle){0, ALTO_PANTALLA * 0.85, ANCHO_PANTALLA, ALTO_PANTALLA * 0.15}, redondez, segmentos, color_fondo);
    DrawRectangleRec((Rectangle){0, ALTO_PANTALLA * 0.96, ANCHO_PANTALLA, ALTO_PANTALLA * 0.04}, color_blanco);
}
//******************************************************************************************************