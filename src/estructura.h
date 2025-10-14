//  CimaBits - ExpoCiencias 2025
//**************************************************************************************************************************
//  LIBRERIAS
//**************************************************************************************************************************
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>
//**************************************************************************************************************************
//  ESTRUCTURAS
//**************************************************************************************************************************
typedef struct _cancion
{
    char titulo[65];
    char artista[45];
    char duracion[8];
    char imagen[255];
    char audio[255];
    char video[255];

    struct _cancion *siguiente;
    struct _cancion *anterior;
} Cancion;

typedef Cancion *CancionPTR;
CancionPTR ultima_cancion;

typedef struct
{
    Texture2D textura;
    Rectangle rect;
    bool activo;
} Boton_Interfaz;

typedef struct
{
    int inicio;
    float desplazamiento;
    int target_inicio;
    bool scrolling;
} Estado_Scroll;

typedef struct
{
    Texture2D imagen;
    bool cargada;
    char ruta[255];
} Estado_Imagen;

typedef struct
{
    Music musica;
    bool cargada;
    bool reproduciendo;
    float duracion;
    float tiempo_actual;
    char ruta[255];
} Estado_Audio;

/*typedef struct
{
    Texture2D frame_video;
    bool cargada;
    bool reproduciendo;
    float duracion;
    float tiempo_actual;
    char ruta[255];
    void *reproducir_video;
} Estado_Video;*/

typedef enum
{
    SECCION_NINGUNA = 0,
    SECCION_SUPERIOR,
    SECCION_INFERIOR,
    SECCION_IZQUIERDA,
    SECCION_DERECHA
} Seccion_Pantalla;
//**************************************************************************************************************************
//  CONSTANTES
//**************************************************************************************************************************
#define ANCHO_PANTALLA 1920
#define ALTO_PANTALLA 1080
#define CANCIONES_VISIBLES 5
#define SCROLL_VELOCIDAD 20
//**************************************************************************************************************************
//  VARIABLES
//**************************************************************************************************************************
Estado_Audio audio_actual = {0};
Estado_Imagen imagen_actual = {0};
//**************************************************************************************************************************
//  PORTOTIPOS
//**************************************************************************************************************************
int validar_duracion(const char *duracion);
CancionPTR crear_cancion();
int agregar_cancion(CancionPTR *playlist, const char *titulo, const char *artista, const char *duracion, const char *ruta_imagen, const char *ruta_audio, const char *ruta_video, int insertar);
void insertar_primero(CancionPTR nodo, CancionPTR *lista);
void insertar_ultimo(CancionPTR nodo, CancionPTR *lista);
int manejar_boton_simple(Boton_Interfaz boton);
void llenar_lista_canciones(CancionPTR *playlist, int *total_canciones);
int obtener_indice_cancion(CancionPTR playlist, CancionPTR cancion_actual, int total_canciones);
int calcular_inicio_para_centrar(int cancion_seleccionada, int total_canciones);
void actualizar_scroll(Estado_Scroll *scroll);
void cambiar_cancion_actual(CancionPTR *cancion_actual, CancionPTR nueva_cancion, bool *esta_reproduciendo);

void mostrarPlaylist(CancionPTR lista);
void mostrarCancion(CancionPTR cancion);
//**************************************************************************************************************************
//**************************************************************************************************************************
//  FUNCIONES DE ESTRUCTURA
//**************************************************************************************************************************
int validar_duracion(const char *duracion)
{
    // VARIABLES LOCALES
    int minutos, segundos;
    // AQUI EMPIEZA LA FUNCION
    if (sscanf(duracion, "%d:%d", &minutos, &segundos) != 2)
    {
        return 0;
    }
    if (minutos < 0 || minutos > 59)
    {
        return 0;
    }
    if (segundos < 0 || segundos > 59)
    {
        return 0;
    }
    return 1;
}
//**************************************************************************************************************************
CancionPTR crear_cancion()
{
    CancionPTR nueva = (CancionPTR)malloc(sizeof(Cancion));

    nueva->anterior = NULL;
    nueva->siguiente = NULL;

    strcpy(nueva->artista, "");
    strcpy(nueva->audio, "");
    strcpy(nueva->duracion, "");
    strcpy(nueva->imagen, "");
    strcpy(nueva->titulo, "");
    strcpy(nueva->video, "");

    return nueva;
}
//**************************************************************************************************************************
int agregar_cancion(CancionPTR *playlist, const char *titulo, const char *artista, const char *duracion, const char *ruta_imagen, const char *ruta_audio, const char *ruta_video, int insertar)
{
    CancionPTR nodo = crear_cancion();
    if (nodo == NULL)
    {
        return 0;
    }

    strcpy(nodo->titulo, titulo);
    strcpy(nodo->artista, artista);
    strcpy(nodo->duracion, duracion);
    strcpy(nodo->imagen, ruta_imagen);
    strcpy(nodo->audio, ruta_audio);
    strcpy(nodo->video, ruta_video);

    nodo->siguiente = NULL;
    nodo->anterior = NULL;

    switch (insertar)
    {
    case 1:
        insertar_primero(nodo, playlist);
        break;
    case 2:
        insertar_ultimo(nodo, playlist);
        break;
    }

    return 1;
}
//**************************************************************************************************************************
void insertar_primero(CancionPTR nodo, CancionPTR *lista)
{
    if (nodo == NULL)
        return;

    if (*lista == NULL)
    {
        *lista = nodo;
        nodo->siguiente = nodo;
        nodo->anterior = nodo;
        ultima_cancion = nodo;
    }
    else
    {
        CancionPTR ultimo = (*lista)->anterior;

        nodo->siguiente = *lista;
        nodo->anterior = ultimo;

        (*lista)->anterior = nodo;
        ultimo->siguiente = nodo;

        *lista = nodo;
    }
}
//**************************************************************************************************************************
void insertar_ultimo(CancionPTR nodo, CancionPTR *lista)
{
    if (nodo == NULL)
        return;

    if (*lista == NULL)
    {
        *lista = nodo;
        nodo->siguiente = nodo;
        nodo->anterior = nodo;
    }
    else
    {
        CancionPTR ultimo = (*lista)->anterior;

        nodo->siguiente = *lista;
        nodo->anterior = ultimo;

        ultimo->siguiente = nodo;
        (*lista)->anterior = nodo;
    }
    ultima_cancion = nodo;
}
//**************************************************************************************************************************
int manejar_boton_simple(Boton_Interfaz boton)
{
    Vector2 posision_mouse = GetMousePosition();
    Color color_boton = WHITE;
    int boton_presionado = 0;

    if (CheckCollisionPointRec(posision_mouse, boton.rect))
    {
        color_boton = GRAY;

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            boton_presionado = 1;
        }
    }

    DrawTexturePro(boton.textura, (Rectangle){0, 0, boton.textura.width, boton.textura.height}, boton.rect, (Vector2){0, 0}, 0.0f, color_boton);

    return boton_presionado;
}
//**************************************************************************************************************************
void llenar_lista_canciones(CancionPTR *playlist, int *total_canciones)
{
    if (*total_canciones > 0)
    {
        return;
    }

    // Datos de canciones
    const char *titulos[] = {
        "COME & GO", "DRINKIN", "FAST CAR", "FIRESTONE", "FRIDAY",
        "HISTORY", "LET IT BE ME", "LET ME GO", "LEVITATING", "LONELY",
        "SORRY", "UNDER CONTROL", "WHAT WOULD YOU DO?", "WHERE ARE U NOW", "WOULD YOU EVER"};

    const char *artistas[] = {
        "MARSHMELLO", "JOEL CORRY", "JONAS BLUE", "KYGO", "RITON Y NIGHCRAWLERS",
        "JOEL CORRY", "BACKSTREET BOYS", "ALESSO", "DUA LIPA", "JOEL CORRY",
        "JOEL CORRY", "ALESSO", "JOEL CORRY", "JACK U", "SKRILLEX"};

    const char *duraciones[] = {
        "3:25", "2:29", "3:32", "4:31", "2:49",
        "2:56", "3:44", "2:54", "3:23", "3:10",
        "3:08", "3:04", "2:54", "4:10", "3:54"};

    const char *ruta_img[] = {
        "insertar/8.jpg", "insertar/5.jpg", "insertar/6.jpg", "insertar/7.jpg", "insertar/9.jpg",
        "insertar/5.jpg", "insertar/2.jpg", "insertar/1.jpg", "insertar/3.jpg", "insertar/5.jpg",
        "insertar/5.jpg", "insertar/1.jpg", "insertar/5.jpg", "insertar/4.jpg", "insertar/0.jpg"};

    const char *ruta_aud[] = {
        "insertar/Come Go.mp3", "insertar/Drinkin.mp3", "insertar/Fast Car.mp3", "insertar/Firestone.mp3", "insertar/Friday.mp3",
        "insertar/History.mp3", "insertar/Let It Be Me.mp3", "insertar/Let Me Go.mp3", "insertar/Levitating.mp3", "insertar/Lonely.mp3",
        "insertar/Sorry.mp3", "insertar/Under Control.mp3", "insertar/What Would You Do.mp3", "insertar/Where Are U Now.mp3", "insertar/Would You Ever.mp3"};

    for (int i = 0; i < 15; i++)
    {
        agregar_cancion(playlist, titulos[i], artistas[i], duraciones[i], ruta_img[i], ruta_aud[i], "\0", 2);
        (*total_canciones)++;
    }
}
//**************************************************************************************************************************
int obtener_indice_cancion(CancionPTR playlist, CancionPTR cancion_actual, int total_canciones)
{
    if (playlist == NULL || cancion_actual == NULL)
        return 0;

    CancionPTR actual = playlist;
    int indice = 0;

    do
    {
        if (actual == cancion_actual)
            return indice;

        actual = actual->siguiente;
        indice++;

        if (indice >= total_canciones)
            break;

    } while (actual != playlist && actual != NULL);

    return 0;
}
//**************************************************************************************************************************
int calcular_inicio_para_centrar(int cancion_seleccionada, int total_canciones)
{
    int inicio;
    
    if (cancion_seleccionada <= 1) {
        inicio = 0;
    }
    else if (cancion_seleccionada >= total_canciones - 2) {
        inicio = total_canciones - CANCIONES_VISIBLES;
        if (inicio < 0) inicio = 0;
    }
    else {
        inicio = cancion_seleccionada - CANCIONES_VISIBLES / 2;
    }
    
    return inicio;
}
//**************************************************************************************************************************
void actualizar_scroll(Estado_Scroll *scroll)
{
    if (scroll->scrolling)
    {
        // Si hemos llegado al objetivo
        if (scroll->inicio == scroll->target_inicio && scroll->desplazamiento == 0)
        {
            scroll->scrolling = false;
            return;
        }

        // Calcular dirección y distancia
        int direccion = (scroll->target_inicio > scroll->inicio) ? 1 : -1;

        // Mover el desplazamiento
        scroll->desplazamiento += SCROLL_VELOCIDAD * direccion;

        // Si el desplazamiento supera el tamaño de una fila, cambiar el índice
        if (abs(scroll->desplazamiento) >= 80)
        { // 80 es la altura de cada fila
            scroll->inicio += direccion;
            scroll->desplazamiento -= 80 * direccion;
        }

        // Si llegamos al índice objetivo, ajustar para terminar exactamente
        if (scroll->inicio == scroll->target_inicio)
        {
            if (scroll->desplazamiento != 0)
            {
                // Ajustar el desplazamiento para terminar en 0
                scroll->desplazamiento += (scroll->desplazamiento > 0) ? -SCROLL_VELOCIDAD : SCROLL_VELOCIDAD;
                if (abs(scroll->desplazamiento) < SCROLL_VELOCIDAD)
                    scroll->desplazamiento = 0;
            }
        }
    }
}
//**************************************************************************************************************************
void cambiar_cancion_actual(CancionPTR *cancion_actual, CancionPTR nueva_cancion, bool *esta_reproduciendo)
{
     if (nueva_cancion == NULL) return;
     
    if (*cancion_actual != nueva_cancion)
    {
        // Detener y liberar recursos de la canción actual
        if (audio_actual.cargada)
        {
            StopMusicStream(audio_actual.musica);
            UnloadMusicStream(audio_actual.musica);
            audio_actual.cargada = false;
        }
        if (imagen_actual.cargada)
        {
            UnloadTexture(imagen_actual.imagen);
            imagen_actual.cargada = false;
        }

        // Actualizar el puntero de la canción
        *cancion_actual = nueva_cancion;
        *esta_reproduciendo = true;
    }
}
//**************************************************************************************************************************
CancionPTR siguiente_cancion(CancionPTR actual)
{
    if (actual == NULL)
        return NULL;
    return actual->siguiente;
}
//**************************************************************************************************************************
CancionPTR anterior_cancion(CancionPTR actual)
{
    if (actual == NULL)
        return NULL;
    return actual->anterior;
}
//**************************************************************************************************************************
// PRUEBA DE QUE SI SE LLENA LA PLAYLIST
void mostrarPlaylist(CancionPTR lista)
{
    CancionPTR aux = lista;
    printf("Titulo%15sArtista\n", " ");
    while (aux != NULL)
    {
        mostrarCancion(aux);
        aux = aux->siguiente;
    }
}
void mostrarCancion(CancionPTR cancion)
{
    printf("%-20s %-20s\n", cancion->titulo, cancion->artista);
}