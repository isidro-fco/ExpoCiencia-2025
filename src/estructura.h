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

typedef Cancion *NodoPtr;
typedef Cancion *Lista;

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
//******************************************************************************************************

//******************************************************************************************************