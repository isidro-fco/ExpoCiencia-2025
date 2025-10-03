#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

#define ANCHO_PANTALLA 1920
#define ALTO_PANTALLA 1080
#define ANCHO_FONDO 3840
#define ALTO_FONDO 2160

typedef struct _cancion
{
    char titulo[65];
    char artista[45];
    char duracion[8];
    char portada[255];
    char audio[255];
    char video[255];

    struct _cancion *siguiente;
    struct _cancion *anterior;
} Cancion;

typedef Cancion *NodoPtr;
typedef Cancion *Lista;