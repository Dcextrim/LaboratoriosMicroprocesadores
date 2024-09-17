#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NUM_THREADS 10  // Definición del número de hilos que se van a crear

/* 
 * Función que será ejecutada por cada hilo.
 * Recibe un parámetro genérico de tipo void*, que se convierte a un puntero a entero.
 * El hilo imprimirá un mensaje con el número de hilo recibido y luego se terminará.
 */
void *PrintHello(void *paramID) {
    int *id = (int *)paramID;  // Conversión de tipo void* a int* para obtener el ID del hilo
    printf("Hello World! desde el hilo #%d!\n", *id);  // Imprime el mensaje con el número del hilo
    pthread_exit(NULL);  // Termina el hilo y devuelve NULL
    return NULL;  // Retorno opcional, no se utiliza ya que pthread_exit finaliza el hilo
}

int main(int argc, char *argv[]) {
    pthread_t threadsID;  // Variable para almacenar el identificador de un hilo
    int param[NUM_THREADS];  // Arreglo para almacenar los parámetros que se pasarán a cada hilo
    int rc, t;  // Variables de control: rc para el código de retorno y t como contador

    // Ciclo para crear los hilos y hacer join en el mismo ciclo
    for (t = 0; t < NUM_THREADS; t++) {
        printf("Creando hilo %d\n", t);  // Mensaje indicando que se está creando un hilo
        param[t] = t;  // Almacena el número de hilo en el arreglo param
        /* 
         * pthread_create: crea un hilo y lo inicia.
         * Argumentos:
         * 1. &threadsID: referencia al identificador del hilo
         * 2. NULL: atributos por defecto para el hilo
         * 3. PrintHello: función que ejecutará el hilo
         * 4. (void *)&param[t]: parámetro que se le pasa a la función, convertido a void*
         */
        rc = pthread_create(&threadsID, NULL, PrintHello, (void *)&param[t]);
        if (rc) {  // Si pthread_create devuelve un código distinto de 0, ocurrió un error
            printf("ERROR; return code from pthread_create() is %d\n", rc);  // Imprime el código de error
            exit(-1);  // Termina el programa si hay error
        }

        /* 
         * pthread_join: espera a que el hilo termine.
         * Se coloca inmediatamente después de pthread_create dentro del mismo ciclo.
         * Argumentos:
         * 1. threadsID: identificador del hilo que se va a esperar
         * 2. NULL: no se recupera ningún valor de retorno del hilo
         */
        rc = pthread_join(threadsID, NULL);
        if (rc) {  // Si pthread_join devuelve un código distinto de 0, ocurrió un error
            printf("ERROR; return code from pthread_join() is %d\n", rc);  // Imprime el código de error
            exit(-1);  // Termina el programa si hay error
        }
    }

    return 0;  // Fin del programa
}
