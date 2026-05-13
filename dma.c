/*
    Simulación didáctica de DMA
    ANSI C compatible

    Objetivo:
    Mostrar cómo un controlador DMA copia datos
    desde un dispositivo hacia memoria RAM
    sin que la CPU copie byte por byte.
*/

#include <stdio.h>
#include <string.h>

#define BUFFER_SIZE 64

/* -----------------------------------
   Memoria del dispositivo (ej: disco)
   ----------------------------------- */
char device_buffer[BUFFER_SIZE] =
    "Datos enviados por dispositivo usando DMA";

/* -----------------------------------
   Memoria principal RAM
   ----------------------------------- */
char ram_buffer[BUFFER_SIZE];

/* -----------------------------------
   Estructura del controlador DMA
   ----------------------------------- */
typedef struct
{
    char *source;       /* origen de datos */
    char *destination;  /* destino en RAM */
    int count;          /* bytes a transferir */
    int active;         /* DMA en ejecución */
} DMA_Controller;

/* controlador global */
DMA_Controller dma;

/* -----------------------------------
   Configurar DMA
   Similar a escribir registros hardware
   ----------------------------------- */
void dma_setup(char *src, char *dst, int bytes)
{
    dma.source = src;
    dma.destination = dst;
    dma.count = bytes;
    dma.active = 1;

    printf("DMA configurado.\n");
    printf("Origen      : dispositivo\n");
    printf("Destino     : RAM\n");
    printf("Bytes       : %d\n\n", bytes);
}

/* -----------------------------------
   Ejecutar transferencia DMA
   ----------------------------------- */
void dma_transfer()
{
    int i;

    if (!dma.active)
    {
        printf("DMA no activo.\n");
        return;
    }

    /*
        Copia directa.
        Conceptualmente esto lo hace hardware.
    */
    for (i = 0; i < dma.count; i++)
    {
        dma.destination[i] = dma.source[i];
    }

    dma.active = 0;

    printf("Transferencia DMA completada.\n");
}

/* -----------------------------------
   Interrupción al finalizar
   ----------------------------------- */
void dma_interrupt_handler()
{
    printf("Interrupcion: DMA finalizo transferencia.\n");
}

/* -----------------------------------
   Programa principal
   ----------------------------------- */
int main()
{
    int bytes;

    bytes = strlen(device_buffer) + 1;

    printf("CPU solicita lectura desde dispositivo.\n\n");

    /*
        La CPU solo configura DMA.
        No copia byte por byte.
    */
    dma_setup(device_buffer, ram_buffer, bytes);

    /*
        CPU podria ejecutar otros procesos aquí.
    */

    dma_transfer();

    /*
        Al terminar, DMA avisa con interrupción.
    */
    dma_interrupt_handler();

    printf("\nContenido en RAM:\n%s\n", ram_buffer);

    return 0;
}