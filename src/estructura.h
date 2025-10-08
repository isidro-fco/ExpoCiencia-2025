// CimaBits - ExpoCiencias 2025
//******************************************************************************************************
// LIBRERIAS
#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <stdbool.h>
#include <time.h>

//******************************************************************************************************
// CONSTANTES
#define ANCHO_PANTALLA 1920
#define ALTO_PANTALLA 1080

//******************************************************************************************************
// ESTRUCTURAS
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

//******************************************************************************************************
// PORTOTIPOS
int validar_duracion(const char *duracion);
//******************************************************************************************************
//-------------------------------------- FUNCIONES DE ESTRUCTURA ---------------------------------------
//******************************************************************************************************
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