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
    Texture2D *frames_video;
    int total_frames_video;

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
    bool animacion_rapida;
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

const char *ruta_vid[] = {
    "assets/cargar/video/1", "assets/cargar/video/2", "assets/cargar/video/3", "assets/cargar/video/4", "assets/cargar/video/5",
    "assets/cargar/video/6", "assets/cargar/video/7", "assets/cargar/video/8", "assets/cargar/video/4", "assets/cargar/video/5",
    "assets/cargar/video/3", "assets/cargar/video/7"};
//**************************************************************************************************************************
//  CONSTANTES
//**************************************************************************************************************************
#define ANCHO_PANTALLA 1920
#define ALTO_PANTALLA 1080
#define CANCIONES_VISIBLES 5
#define SCROLL_VELOCIDAD 20
#define MAX_TEXTURAS_CARGADAS 100
#define MAX_FRAMES 2000
//**************************************************************************************************************************
//  VARIABLES
//**************************************************************************************************************************

Estado_Audio audio_actual = {0};
Estado_Imagen imagen_actual = {0};
Estado_Imagen cache_texturas[MAX_TEXTURAS_CARGADAS];
int num_texturas_cargadas = 0;
Texture2D frames[MAX_FRAMES];
char rutas[MAX_FRAMES][256];
int total_frames = 0;
int frame_actual = 0;
float tiempo_frame = 0.0f;
float intervalo = 1.0f / 10.0f;
float x = (ANCHO_PANTALLA / 2) - 720;
float y = (ALTO_PANTALLA / 2) - 280;
//**************************************************************************************************************************
//  PORTOTIPOS
//**************************************************************************************************************************
int validar_duracion(const char *duracion);
CancionPTR crear_cancion();
int agregar_cancion(CancionPTR *playlist, const char *titulo, const char *artista, const char *duracion, const char *ruta_imagen, const char *ruta_audio, const char *ruta_video, int insertar);
void insertar_primero(CancionPTR nodo, CancionPTR *lista);
void insertar_ultimo(CancionPTR nodo, CancionPTR *lista);
void eliminar_cancion_actual(CancionPTR *cancion_actual, CancionPTR *playlist, int *total_canciones, bool *esta_reproduciendo);
CancionPTR buscar_canciones(CancionPTR lista, const char *termino, int *total_resultados);
int manejar_boton_simple(Boton_Interfaz boton);
void llenar_lista_canciones(CancionPTR *playlist, int *total_canciones);
int obtener_indice_cancion(CancionPTR playlist, CancionPTR cancion_actual, int total_canciones);
int calcular_inicio_para_centrar(int cancion_seleccionada, int total_canciones);
void actualizar_scroll(Estado_Scroll *scroll);
void cambiar_cancion_actual(CancionPTR *cancion_actual, CancionPTR nueva_cancion, bool *esta_reproduciendo);
Texture2D obtener_textura(const char *ruta);
void generar_rutas(char rutas[MAX_FRAMES][256], const char *nombre_carpeta);
void cargar_frames(Texture2D *frames, int *total_frames, char rutas[MAX_FRAMES][256]);
void liberar_frames(Texture2D *frames, int total_frames);
static int comparar_ci(const char *a, const char *b);
static int cmpTitulo_qsort(const void *a, const void *b);
static int cmpArtist_qsort(const void *a, const void *b);
static int cmpDuracion_qsort(const void *a, const void *b);
CancionPTR ordenarListaNombre(CancionPTR lista);
CancionPTR ordenarListaArtist(CancionPTR lista);
CancionPTR ordenarListaDuracion(CancionPTR lista);
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
//******************************************************************************************************
void eliminar_cancion_actual(CancionPTR *cancion_actual, CancionPTR *playlist, int *total_canciones, bool *esta_reproduciendo)
{
    if (*cancion_actual == NULL || *playlist == NULL)
        return;

    CancionPTR cancion_a_eliminar = *cancion_actual;

    if (*total_canciones == 1)
    {
        *playlist = NULL;
        *cancion_actual = NULL;
        ultima_cancion = NULL;
    }
    else
    {
        cancion_a_eliminar->anterior->siguiente = cancion_a_eliminar->siguiente;
        cancion_a_eliminar->siguiente->anterior = cancion_a_eliminar->anterior;

        if (*playlist == cancion_a_eliminar)
        {
            *playlist = cancion_a_eliminar->siguiente;
        }

        if (ultima_cancion == cancion_a_eliminar)
        {
            ultima_cancion = cancion_a_eliminar->anterior;
        }

        if (cancion_a_eliminar->siguiente != cancion_a_eliminar)
        {
            *cancion_actual = cancion_a_eliminar->siguiente;
        }
        else
        {
            *cancion_actual = cancion_a_eliminar->anterior;
        }
    }

    if (audio_actual.cargada && strcmp(audio_actual.ruta, cancion_a_eliminar->audio) == 0)
    {
        StopMusicStream(audio_actual.musica);
        UnloadMusicStream(audio_actual.musica);
        audio_actual.cargada = false;
        *esta_reproduciendo = false;
    }

    free(cancion_a_eliminar);
    (*total_canciones)--;

    if (*total_canciones == 0)
    {
        *playlist = NULL;
        *cancion_actual = NULL;
        ultima_cancion = NULL;
    }
}
//**************************************************************************************************************************
CancionPTR buscar_canciones(CancionPTR lista, const char *termino, int *total_resultados)
{
    if (!lista || !termino || strlen(termino) == 0)
        return NULL;

    CancionPTR resultado = NULL;
    *total_resultados = 0;

    CancionPTR actual = lista;
    bool primera = true;
    const long MAX_ITER = 1000000;
    long iter = 0;

    while (actual && (primera || actual != lista))
    {
        primera = false;
        iter++;
        if (iter > MAX_ITER)
            break;

        // Convertir a minúsculas para comparación insensible a mayúsculas
        char titulo_lower[65], artista_lower[45], duracion_lower[8], termino_lower[256];
        strncpy(titulo_lower, actual->titulo, sizeof(titulo_lower));
        strncpy(artista_lower, actual->artista, sizeof(artista_lower));
        strncpy(duracion_lower, actual->duracion, sizeof(duracion_lower));
        strncpy(termino_lower, termino, sizeof(termino_lower));

        for (int i = 0; titulo_lower[i]; i++)
            titulo_lower[i] = tolower(titulo_lower[i]);
        for (int i = 0; artista_lower[i]; i++)
            artista_lower[i] = tolower(artista_lower[i]);
        for (int i = 0; duracion_lower[i]; i++)
            duracion_lower[i] = tolower(duracion_lower[i]);
        for (int i = 0; termino_lower[i]; i++)
            termino_lower[i] = tolower(termino_lower[i]);

        // Buscar coincidencia parcial
        if (strstr(titulo_lower, termino_lower) || strstr(artista_lower, termino_lower) || strstr(duracion_lower, termino_lower))
        {
            CancionPTR copia = crear_cancion();
            if (!copia)
                break;

            strcpy(copia->titulo, actual->titulo);
            strcpy(copia->artista, actual->artista);
            strcpy(copia->duracion, actual->duracion);
            strcpy(copia->imagen, actual->imagen);
            strcpy(copia->audio, actual->audio);
            strcpy(copia->video, actual->video);

            insertar_ultimo(copia, &resultado);
            (*total_resultados)++;
        }

        actual = actual->siguiente;
    }

    return resultado;
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
        "MAIN THEME", "DONKEY KONG COUNTRY", "INKOMING!", "GREEN GREENS", "MAIN THEME",
        "POKEMON CENTER THEME", "WILD POKEMON BATTLE THEME", "GREEN HILL ZONE", "FLOWER GARDEN", "CIRCULO ESTELAR",
        "EL DESPERTADOR DEL HEROE", "TIERRA DE NADIE"};

    const char *artistas[] = {
        "ANIMAL CROSSING", "DONKEY KONG COUNTRY", "SPLATOON 2", "KIRBY'S DREAM LAND", "SUPER SMASH BROS. BRAWL",
        "POKEMON RED / BLUE", "POKEMON SWORD Y SHIELD", "SONIC THE HEDGEHOG", "YOSHI'S ISLAND: SMW 2", "MARIO KART DELUXE",
        "THE LEGEND OF ZELDA", "UNCHARTED"};

    const char *duraciones[] = {
        "2:08", "3:40", "2:55", "1:31", "2:48",
        "1:08", "4:59", "2:21", "2:53", "1:44",
        "1:07", "1:46"};

    const char *ruta_img[] = {
        "assets/cargar/portada/1.png",
        "assets/cargar/portada/2.png",
        "assets/cargar/portada/3.png",
        "assets/cargar/portada/4.png",
        "assets/cargar/portada/5.png",
        "assets/cargar/portada/6.png",
        "assets/cargar/portada/7.png",
        "assets/cargar/portada/8.png",
        "assets/cargar/portada/9.png",
        "assets/cargar/portada/10.jpg",
        "assets/cargar/portada/11.png",
        "assets/cargar/portada/12.png",
    };

    const char *ruta_aud[] = {
        "assets/cargar/audio/1.mp3",
        "assets/cargar/audio/2.mp3",
        "assets/cargar/audio/3.mp3",
        "assets/cargar/audio/4.mp3",
        "assets/cargar/audio/5.mp3",
        "assets/cargar/audio/6.mp3",
        "assets/cargar/audio/7.mp3",
        "assets/cargar/audio/8.mp3",
        "assets/cargar/audio/9.mp3",
        "assets/cargar/audio/10.mp3",
        "assets/cargar/audio/11.mp3",
        "assets/cargar/audio/12.mp3",
    };

    // Número total de canciones (se calcula automáticamente)
    int cantidad = sizeof(titulos) / sizeof(titulos[0]);

    for (int i = 0; i < cantidad; i++)
    {
        agregar_cancion(playlist, titulos[i], artistas[i], duraciones[i], ruta_img[i], ruta_aud[i], ruta_vid[i], 2);
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

    if (cancion_seleccionada <= 1)
    {
        inicio = 0;
    }
    else if (cancion_seleccionada >= total_canciones - 2)
    {
        inicio = total_canciones - CANCIONES_VISIBLES;
        if (inicio < 0)
            inicio = 0;
    }
    else
    {
        inicio = cancion_seleccionada - CANCIONES_VISIBLES / 2;
    }

    return inicio;
}
//**************************************************************************************************************************
void actualizar_scroll(Estado_Scroll *scroll)
{
    if (scroll->scrolling)
    {
        // Si la animación está desactivada, saltar directamente al objetivo
        if (scroll->animacion_rapida)
        {
            scroll->inicio = scroll->target_inicio;
            scroll->desplazamiento = 0;
            scroll->scrolling = false;
            scroll->animacion_rapida = false;
            return;
        }

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
    if (nueva_cancion == NULL)
        return;

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
Texture2D obtener_textura(const char *ruta)
{
    // Buscar en cache
    for (int i = 0; i < num_texturas_cargadas; i++)
    {
        if (strcmp(cache_texturas[i].ruta, ruta) == 0)
        {
            return cache_texturas[i].imagen;
        }
    }

    // Si no está en cache, cargarla
    if (num_texturas_cargadas < MAX_TEXTURAS_CARGADAS && ruta[0] != '\0')
    {
        Texture2D textura = LoadTexture(ruta);
        if (textura.id > 0)
        {
            SetTextureFilter(textura, TEXTURE_FILTER_BILINEAR);
            strcpy(cache_texturas[num_texturas_cargadas].ruta, ruta);
            cache_texturas[num_texturas_cargadas].imagen = textura;
            cache_texturas[num_texturas_cargadas].cargada = true;
            num_texturas_cargadas++;
            return textura;
        }
    }

    // Textura vacía si no se pudo cargar
    return (Texture2D){0};
}
//**************************************************************************************************************************
void generar_rutas(char rutas[MAX_FRAMES][256], const char *nombre_carpeta)
{
    for (int i = 1; i <= MAX_FRAMES; i++)
    {
        sprintf(rutas[i - 1], "%s/frame_%04d.png", nombre_carpeta, i);
    }
}
//**************************************************************************************************************************
void cargar_frames(Texture2D *frames, int *total_frames, char rutas[MAX_FRAMES][256])
{
    liberar_frames(frames, *total_frames);
    *total_frames = 0;

    // Inicializamos id a 0 para seguridad
    for (int i = 0; i < MAX_FRAMES; i++)
        frames[i].id = 0;

    for (int i = 0; i < MAX_FRAMES; i++)
    {
        if (FileExists(rutas[i]))
        {
            frames[*total_frames] = LoadTexture(rutas[i]);
            SetTextureFilter(frames[*total_frames], TEXTURE_FILTER_BILINEAR);
            TraceLog(LOG_INFO, "Cargado: %s", rutas[i]); // ayuda a debug
            (*total_frames)++;
        }
        else
        {
            break;
        }
    }
}
//**************************************************************************************************************************
void liberar_frames(Texture2D *frames, int total_frames)
{
    for (int i = 0; i < total_frames; i++)
    {
        if (frames[i].id != 0)
            UnloadTexture(frames[i]);
        frames[i].id = 0;
    }
}
//**************************************************************************************************************************
static int comparar_ci(const char *a, const char *b)
{
    if (!a)
        return (b ? -1 : 0);
    if (!b)
        return 1;
    while (*a && *b)
    {
        unsigned char ca = (unsigned char)tolower((unsigned char)*a);
        unsigned char cb = (unsigned char)tolower((unsigned char)*b);
        if (ca < cb)
            return -1;
        if (ca > cb)
            return 1;
        a++;
        b++;
    }
    if (*a)
        return 1;
    if (*b)
        return -1;
    return 0;
}
//**************************************************************************************************************************
/* qsort comparator: recibe punteros a elementos del arreglo (cada elemento es un CancionPTR) */
static int cmpTitulo_qsort(const void *a, const void *b)
{
    const CancionPTR *pa = (const CancionPTR *)a;
    const CancionPTR *pb = (const CancionPTR *)b;
    if (!pa || !pb)
        return 0;
    return comparar_ci((*pa)->titulo, (*pb)->titulo);
}
//**************************************************************************************************************************
static int cmpArtist_qsort(const void *a, const void *b)
{
    const CancionPTR *pa = (const CancionPTR *)a;
    const CancionPTR *pb = (const CancionPTR *)b;
    if (!pa || !pb)
        return 0;
    return comparar_ci((*pa)->artista, (*pb)->artista);
}
//**************************************************************************************************************************
static int cmpDuracion_qsort(const void *a, const void *b)
{
    const CancionPTR *pa = (const CancionPTR *)a;
    const CancionPTR *pb = (const CancionPTR *)b;
    if (!pa || !pb)
        return 0;
    return comparar_ci((*pa)->duracion, (*pb)->duracion);
}
//**************************************************************************************************************************
/* ordenarListaNombre: crea y devuelve UNA NUEVA lista doble circular ordenada por titulo.
   NO modifica la lista original. */
CancionPTR ordenarListaNombre(CancionPTR lista)
{
    if (!lista)
        return NULL;

    /* 1) Contar nodos (con protección contra corrupción) */
    CancionPTR p = lista;
    long n = 0;
    const long SAFETY = 1000000;
    bool primera = true;
    while (p && (primera || p != lista))
    {
        primera = false;
        n++;
        if (n > SAFETY)
            return NULL; /* posible corrupción de lista */
        p = p->siguiente;
    }
    if (n <= 0)
        return NULL;

    /* 2) Recolectar punteros a nodos originales en un arreglo */
    CancionPTR *orig = (CancionPTR *)malloc(sizeof(CancionPTR) * (size_t)n);
    if (!orig)
        return NULL;

    p = lista;
    primera = true;
    long idx = 0;
    while (p && (primera || p != lista))
    {
        primera = false;
        if (idx >= n)
            break;
        orig[idx++] = p;
        p = p->siguiente;
    }
    if (idx != n)
    {
        free(orig);
        return NULL;
    }

    /* 3) Ordenar el arreglo de punteros por título (case-insensitive) */
    qsort(orig, (size_t)n, sizeof(CancionPTR), cmpTitulo_qsort);

    /* 4) Crear nueva lista copiando nodos en orden y enlazarlos (doble circular) */
    CancionPTR nuevaCabeza = NULL;
    CancionPTR ultimo = NULL;

    for (long i = 0; i < n; ++i)
    {
        CancionPTR src = orig[i];
        CancionPTR copia = (CancionPTR)malloc(sizeof(Cancion));
        if (!copia)
        {
            /* liberar lo creado hasta ahora */
            if (nuevaCabeza)
            {
                /* romper circularidad para facilitar el free */
                nuevaCabeza->anterior->siguiente = NULL;
                CancionPTR it = nuevaCabeza;
                while (it)
                {
                    CancionPTR next = it->siguiente;
                    free(it);
                    it = next;
                }
            }
            free(orig);
            return NULL;
        }

        /* Copiar contenido con límites */
        strncpy(copia->titulo, src->titulo, sizeof(copia->titulo) - 1);
        copia->titulo[sizeof(copia->titulo) - 1] = '\0';
        strncpy(copia->artista, src->artista, sizeof(copia->artista) - 1);
        copia->artista[sizeof(copia->artista) - 1] = '\0';
        strncpy(copia->duracion, src->duracion, sizeof(copia->duracion) - 1);
        copia->duracion[sizeof(copia->duracion) - 1] = '\0';
        strncpy(copia->imagen, src->imagen, sizeof(copia->imagen) - 1);
        copia->imagen[sizeof(copia->imagen) - 1] = '\0';
        strncpy(copia->audio, src->audio, sizeof(copia->audio) - 1);
        copia->audio[sizeof(copia->audio) - 1] = '\0';
        strncpy(copia->video, src->video, sizeof(copia->video) - 1);
        copia->video[sizeof(copia->video) - 1] = '\0';

        /* Inicializar punteros del nodo copia */
        copia->siguiente = NULL;
        copia->anterior = NULL;

        if (!nuevaCabeza)
        {
            nuevaCabeza = copia;
            ultimo = copia;
        }
        else
        {
            /* enlazar al final */
            ultimo->siguiente = copia;
            copia->anterior = ultimo;
            ultimo = copia;
        }
    }

    /* 5) cerrar la circularidad (si hay nodos) */
    if (nuevaCabeza && ultimo)
    {
        nuevaCabeza->anterior = ultimo;
        ultimo->siguiente = nuevaCabeza;
    }

    free(orig);
    return nuevaCabeza;
}
//**************************************************************************************************************************
CancionPTR ordenarListaArtist(CancionPTR lista)
{
    if (!lista)
        return NULL;

    /* 1) Contar nodos (con protección contra corrupción) */
    CancionPTR p = lista;
    long n = 0;
    const long SAFETY = 1000000;
    bool primera = true;
    while (p && (primera || p != lista))
    {
        primera = false;
        n++;
        if (n > SAFETY)
            return NULL; /* posible corrupción de lista */
        p = p->siguiente;
    }
    if (n <= 0)
        return NULL;

    /* 2) Recolectar punteros a nodos originales en un arreglo */
    CancionPTR *orig = (CancionPTR *)malloc(sizeof(CancionPTR) * (size_t)n);
    if (!orig)
        return NULL;

    p = lista;
    primera = true;
    long idx = 0;
    while (p && (primera || p != lista))
    {
        primera = false;
        if (idx >= n)
            break;
        orig[idx++] = p;
        p = p->siguiente;
    }
    if (idx != n)
    {
        free(orig);
        return NULL;
    }

    /* 3) Ordenar el arreglo de punteros por título (case-insensitive) */
    qsort(orig, (size_t)n, sizeof(CancionPTR), cmpArtist_qsort);

    /* 4) Crear nueva lista copiando nodos en orden y enlazarlos (doble circular) */
    CancionPTR nuevaCabeza = NULL;
    CancionPTR ultimo = NULL;

    for (long i = 0; i < n; ++i)
    {
        CancionPTR src = orig[i];
        CancionPTR copia = (CancionPTR)malloc(sizeof(Cancion));
        if (!copia)
        {
            /* liberar lo creado hasta ahora */
            if (nuevaCabeza)
            {
                /* romper circularidad para facilitar el free */
                nuevaCabeza->anterior->siguiente = NULL;
                CancionPTR it = nuevaCabeza;
                while (it)
                {
                    CancionPTR next = it->siguiente;
                    free(it);
                    it = next;
                }
            }
            free(orig);
            return NULL;
        }

        /* Copiar contenido con límites */
        strncpy(copia->titulo, src->titulo, sizeof(copia->titulo) - 1);
        copia->titulo[sizeof(copia->titulo) - 1] = '\0';
        strncpy(copia->artista, src->artista, sizeof(copia->artista) - 1);
        copia->artista[sizeof(copia->artista) - 1] = '\0';
        strncpy(copia->duracion, src->duracion, sizeof(copia->duracion) - 1);
        copia->duracion[sizeof(copia->duracion) - 1] = '\0';
        strncpy(copia->imagen, src->imagen, sizeof(copia->imagen) - 1);
        copia->imagen[sizeof(copia->imagen) - 1] = '\0';
        strncpy(copia->audio, src->audio, sizeof(copia->audio) - 1);
        copia->audio[sizeof(copia->audio) - 1] = '\0';
        strncpy(copia->video, src->video, sizeof(copia->video) - 1);
        copia->video[sizeof(copia->video) - 1] = '\0';

        /* Inicializar punteros del nodo copia */
        copia->siguiente = NULL;
        copia->anterior = NULL;

        if (!nuevaCabeza)
        {
            nuevaCabeza = copia;
            ultimo = copia;
        }
        else
        {
            /* enlazar al final */
            ultimo->siguiente = copia;
            copia->anterior = ultimo;
            ultimo = copia;
        }
    }

    /* 5) cerrar la circularidad (si hay nodos) */
    if (nuevaCabeza && ultimo)
    {
        nuevaCabeza->anterior = ultimo;
        ultimo->siguiente = nuevaCabeza;
    }

    free(orig);
    return nuevaCabeza;
}
//**************************************************************************************************************************
CancionPTR ordenarListaDuracion(CancionPTR lista)
{
    if (!lista)
        return NULL;

    /* 1) Contar nodos (con protección contra corrupción) */
    CancionPTR p = lista;
    long n = 0;
    const long SAFETY = 1000000;
    bool primera = true;
    while (p && (primera || p != lista))
    {
        primera = false;
        n++;
        if (n > SAFETY)
            return NULL; /* posible corrupción de lista */
        p = p->siguiente;
    }
    if (n <= 0)
        return NULL;

    /* 2) Recolectar punteros a nodos originales en un arreglo */
    CancionPTR *orig = (CancionPTR *)malloc(sizeof(CancionPTR) * (size_t)n);
    if (!orig)
        return NULL;

    p = lista;
    primera = true;
    long idx = 0;
    while (p && (primera || p != lista))
    {
        primera = false;
        if (idx >= n)
            break;
        orig[idx++] = p;
        p = p->siguiente;
    }
    if (idx != n)
    {
        free(orig);
        return NULL;
    }

    /* 3) Ordenar el arreglo de punteros por título (case-insensitive) */
    qsort(orig, (size_t)n, sizeof(CancionPTR), cmpDuracion_qsort);

    /* 4) Crear nueva lista copiando nodos en orden y enlazarlos (doble circular) */
    CancionPTR nuevaCabeza = NULL;
    CancionPTR ultimo = NULL;

    for (long i = 0; i < n; ++i)
    {
        CancionPTR src = orig[i];
        CancionPTR copia = (CancionPTR)malloc(sizeof(Cancion));
        if (!copia)
        {
            /* liberar lo creado hasta ahora */
            if (nuevaCabeza)
            {
                /* romper circularidad para facilitar el free */
                nuevaCabeza->anterior->siguiente = NULL;
                CancionPTR it = nuevaCabeza;
                while (it)
                {
                    CancionPTR next = it->siguiente;
                    free(it);
                    it = next;
                }
            }
            free(orig);
            return NULL;
        }

        /* Copiar contenido con límites */
        strncpy(copia->titulo, src->titulo, sizeof(copia->titulo) - 1);
        copia->titulo[sizeof(copia->titulo) - 1] = '\0';
        strncpy(copia->artista, src->artista, sizeof(copia->artista) - 1);
        copia->artista[sizeof(copia->artista) - 1] = '\0';
        strncpy(copia->duracion, src->duracion, sizeof(copia->duracion) - 1);
        copia->duracion[sizeof(copia->duracion) - 1] = '\0';
        strncpy(copia->imagen, src->imagen, sizeof(copia->imagen) - 1);
        copia->imagen[sizeof(copia->imagen) - 1] = '\0';
        strncpy(copia->audio, src->audio, sizeof(copia->audio) - 1);
        copia->audio[sizeof(copia->audio) - 1] = '\0';
        strncpy(copia->video, src->video, sizeof(copia->video) - 1);
        copia->video[sizeof(copia->video) - 1] = '\0';

        /* Inicializar punteros del nodo copia */
        copia->siguiente = NULL;
        copia->anterior = NULL;

        if (!nuevaCabeza)
        {
            nuevaCabeza = copia;
            ultimo = copia;
        }
        else
        {
            /* enlazar al final */
            ultimo->siguiente = copia;
            copia->anterior = ultimo;
            ultimo = copia;
        }
    }

    /* 5) cerrar la circularidad (si hay nodos) */
    if (nuevaCabeza && ultimo)
    {
        nuevaCabeza->anterior = ultimo;
        ultimo->siguiente = nuevaCabeza;
    }

    free(orig);
    return nuevaCabeza;
}
//**************************************************************************************************************************