/*  2023 -  Carmela Colqui <carmela.colqui@gmail.com>
  */

/** @file EITI_TPN° 9
 **
 ** @brief TPN° 9  - FreeRTOS
 **
 ** Ejemplo de un led parpadeando utilizando la capa de abstraccion de
 ** hardware y con sistema operativo FreeRTOS.
 **
 **
 ** @defgroup ejemplos Proyectos de ejemplo
 ** @brief Proyectos de ejemplos en Sistemas Embebidos
 ** @{
 */

/* === Inclusiones de cabeceras ============================================ */
#include "FreeRTOS.h"
#include "bsp.h"
#include "task.h"
#include <stdbool.h>
#include "semphr.h"

/* === Definicion y Macros ================================================= */

/* === Declaraciones de tipos de datos internos ============================ */

typedef struct parametros_s
{
    digital_output_t led;
    uint16_t periodo;
} *parametros_t;

/* === Declaraciones de funciones internas ================================= */

/* === Definiciones de variables internas ================================== */

static board_t board;
static SemaphoreHandle_t mutex;
/* === Definiciones de variables externas ================================== */

/* === Definiciones de funciones internas ================================== */

void Blinking(void *parameters)
{

    parametros_t parametros = (parametros_t)parameters;

    while (true)
    {
        xSemaphoreTake(mutex, portMAX_DELAY);
        DigitalOutputActivate(parametros->led);
        vTaskDelay(pdMS_TO_TICKS(parametros->periodo));
        DigitalOutputDeactivate(parametros->led);
        xSemaphoreGive(mutex);
        vTaskDelay(pdMS_TO_TICKS(parametros->periodo));
    }
}

/* === Definiciones de funciones externas ================================== */

/** @brief Función principal del programa
 **
 ** @returns 0 La función nunca debería termina
 **
 ** @remarks En un sistema embebido la función main() nunca debe terminar.
 **          El valor de retorno 0 es para evitar un error en el compilador.
 */
int main(void)
{

    static struct parametros_s parametros[2];

    /* Inicializaciones y configuraciones de dispositivos */
    board = BoardCreate();

    parametros[0].led = board->led_azul;
    parametros[0].periodo = 500;

    parametros[1].led = board->led_rojo;
    parametros[1].periodo = 500;

    mutex = xSemaphoreCreateMutex();

    /* Creación de las tareas */
    xTaskCreate(Blinking, "Azul", configMINIMAL_STACK_SIZE, &parametros[0], tskIDLE_PRIORITY + 1, NULL);
    xTaskCreate(Blinking, "Rojo", configMINIMAL_STACK_SIZE, &parametros[1], tskIDLE_PRIORITY + 2, NULL);

    /* Arranque del sistema operativo */
    vTaskStartScheduler();

    /* vTaskStartScheduler solo retorna si se detiene el sistema operativo */
    while (1)
    {
    };

    /* El valor de retorno es solo para evitar errores en el compilador*/
    return 0;
}
/* === Ciere de documentacion ============================================== */
/** @} Final de la definición del modulo para doxygen */