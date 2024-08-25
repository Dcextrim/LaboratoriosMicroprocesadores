/*
 * Archivo: Parte3Lab5.c
 * Descripción: Este programa ejecuta tres secciones en paralelo utilizando OpenMP.
 * 
 * Funcionalidad:
 * - Calcula el factorial de un número.
 * - Genera la serie de Fibonacci.
 * - Encuentra el valor máximo en un arreglo.
 * 
 * Estructura:
 * - Utiliza la directiva #pragma omp parallel sections para ejecutar las secciones en paralelo.
 * - La función Fibonacci está diseñada para utilizar tareas anidadas con #pragma omp task.
 * 
 * Referencia:
 * OPENMP API Specification: Version 5.0 November 2018
 *
 * Fecha modificación: 08-24-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

int fib(int n) { 
    if (n < 2) 
        return n; 
    else {
        int i, j;
        #pragma omp task shared(i)
        i = fib(n - 1);
        #pragma omp task shared(j)
        j = fib(n - 2);
        #pragma omp taskwait
        return i + j;
    }
}

int main(int argc, char* argv[]) {
    printf("Ejecutando secciones en paralelo...\n");

    #pragma omp parallel sections
    {
        #pragma omp section
        {
            // Lógica para calcular el factorial de un número
            int n = 10; 
            long long factorial = 1;
            for (int i = 1; i <= n; ++i) {
                factorial *= i;
            }
            printf("Factorial de %d es %lld\n", n, factorial);
        }

        #pragma omp section
        {
            // Lógica para generar la serie de Fibonacci
            int n = 10;
            int resultado_fib = fib(n);
            printf("Fibonacci de %d es %d\n", n, resultado_fib);
        }

        #pragma omp section
        {
            // Lógica para encontrar el máximo en un arreglo
            int arr[] = {1, 3, 5, 7, 9, 2, 4, 6, 8, 0};
            int size = sizeof(arr) / sizeof(arr[0]);
            int max = arr[0];
            for (int i = 1; i < size; ++i) {
                if (arr[i] > max) {
                    max = arr[i];
                }
            }
            printf("El máximo en el arreglo es %d\n", max);
        }
    }

    printf("Todas las secciones han terminado.\n");

    return 0;
}
