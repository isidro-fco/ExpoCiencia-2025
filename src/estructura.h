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

typedef struct
{
    Texture2D textura;
    Rectangle rect;
    bool activo;
} Boton_Simple;

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

typedef struct
{
    Texture2D frame_video;
    bool cargada;
    bool reproduciendo;
    float duracion;
    float tiempo_actual;
    char ruta[255];
    void *reproducir_video;
} Estado_Video;
//**************************************************************************************************************************
//  CONSTANTES
//**************************************************************************************************************************
#define ANCHO_PANTALLA 1920
#define ALTO_PANTALLA 1080
//**************************************************************************************************************************
//  PORTOTIPOS
//**************************************************************************************************************************
int validar_duracion(const char *duracion);
CancionPTR crear_cancion();
int agregar_cancion(CancionPTR *playlist, const char *titulo, const char *artista, const char *duracion, const char *ruta_imagen, const char *ruta_audio, const char *ruta_video, int insertar);
void insertar_primero(CancionPTR nodo, CancionPTR *lista);
void insertar_ultimo(CancionPTR nodo, CancionPTR *lista);

void mostrarPlaylist(CancionPTR lista);
void mostrarCancion(CancionPTR cancion);
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
//******************************************************************************************************
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
//******************************************************************************************************
void insertar_primero(CancionPTR nodo, CancionPTR *lista)
{
    if (nodo == NULL)
    {
        return;
    }

    // insertar cuando esta vacio
    if (*lista == NULL)
    {
        *lista = nodo;
    }
    else
    {
        // primero
        (*lista)->anterior = nodo;
        nodo->siguiente = *lista;
        *lista = nodo;
    }
}
//******************************************************************************************************
void insertar_ultimo(CancionPTR nodo, CancionPTR *lista)
{
    if (nodo == NULL)
    {
        return;
    }

    // insertar cuando esta vacio
    if (*lista == NULL)
    {
        *lista = nodo;
    }
    else
    {
        // ultimo
        CancionPTR aux = *lista;
        while (aux->siguiente != NULL)
        {
            aux = aux->siguiente;
        }

        aux->siguiente = nodo;
        nodo->anterior = aux;
    }
}
//******************************************************************************************************

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