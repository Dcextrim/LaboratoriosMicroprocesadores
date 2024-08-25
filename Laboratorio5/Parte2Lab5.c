/*
 * Archivo: Ejercicio_5A.c
 * Descripción: Este programa calcula la suma de los primeros N números naturales
 * utilizando un ciclo for paralelo con OpenMP.
 * 
 * Funcionalidad:
 * - Define N como una constante grande.
 * - Utiliza la cláusula reduction con + para acumular la suma en una variable compartida.
 * - Mide los tiempos de ejecución usando omp_get_wtime().
 * 
 * Referencia:
 * OPENMP API Specification: Version 5.0 November 2018
 *
 * Fecha modificación: 08-16-2024
 */

#include <omp.h>
#include <stdio.h>

#define N 3
int main ()
{
    double iniciotiempo,
            fintiempo;
    int contador = 0;
    iniciotiempo = omp_get_wtime();
    #pragma omp parallel for reduction(+ : contador)
    for (int i = 1 ; i<=N ; ++i)
    {
            contador =+i;
    }
    fintiempo = omp_get_wtime();
    printf("Total: %d\nTiempo de ejecucion: %f",contador,fintiempo-iniciotiempo);
    return 0;
}