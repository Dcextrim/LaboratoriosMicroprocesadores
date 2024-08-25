/*
 * Archivo: ModificacionVariables.c
 * Descripción: Este programa modifica dos variables en paralelo utilizando OpenMP.
 * 
 * Funcionalidad:
 * - Modifica variable1 de manera compartida entre los hilos.
 * - Modifica variable2 de manera privada para cada hilo.
 * 
 * Estructura:
 * - Utiliza la directiva #pragma omp parallel for para ejecutar el ciclo en paralelo.
 * - La variable1 se gestiona con la cláusula shared.
 * - La variable2 se gestiona con la cláusula private.
 * 
 * Referencia:
 * OPENMP API Specification: Version 5.0 November 2018
 *
 * Fecha modificación: 08-24-2024
 */

#include <stdio.h>
#include <omp.h>

int main() {
    int variable1 = 0;
    int variable2 = 0;
    int i;

    // Usando shared para variable1 y private para variable2
    #pragma omp parallel for shared(variable1) private(variable2)
    for (i = 0; i < 10; i++) {
        variable2 = i; // Cada hilo tiene su propia copia de variable2
        #pragma omp critical
        {
            variable1 += variable2; // Acceso compartido a variable1
        }
        printf("Hilo %d: variable1 = %d, variable2 = %d\n", omp_get_thread_num(), variable1, variable2);
    }

    printf("Resultado final: variable1 = %d\n", variable1);
    return 0;
}
