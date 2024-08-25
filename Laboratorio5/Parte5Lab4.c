/*
 * Archivo: Ejercicio_5A_Paralelo.c
 * Descripción: Este programa cuenta cuántas veces aparece un valor específico 
 * ('key') en un arreglo de números aleatorios utilizando OpenMP con descomposición de tareas recursiva.
 * 
 * Funcionalidad:
 * - Genera un arreglo de tamaño N con valores aleatorios.
 * - Inserta manualmente el valor 'key' en tres posiciones específicas del arreglo.
 * - Cuenta cuántas veces aparece 'key' en el arreglo usando una función llamada count_key_parallel().
 * - Imprime el número de apariciones de 'key' en el arreglo.
 * 
 * Estructura:
 * - Utiliza la directiva #pragma omp parallel y #pragma omp single para iniciar la ejecución paralela.
 * - La función count_key_parallel() utiliza descomposición de tareas recursiva con #pragma omp task.
 * 
 * Referencia:
 * Chandra, . R. et al. Parallel Programming in OpenMP
 *
 * Fecha modificación: 08-24-2024
 */

#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#define N 131072
#define THRESHOLD 1024  // Umbral para cambiar a ejecución secuencial

long count_key_parallel(long *a, long key, long start, long end) {
    long count = 0;

    if (end - start <= THRESHOLD) {
        // Ejecución secuencial si el tamaño del subarreglo es menor o igual al umbral
        for (long i = start; i < end; i++)
            if (a[i] == key) count++;
    } else {
        // Ejecución paralela
        long mid = (start + end) / 2;
        long count1 = 0, count2 = 0;

        #pragma omp task shared(count1)
        count1 = count_key_parallel(a, key, start, mid);

        #pragma omp task shared(count2)
        count2 = count_key_parallel(a, key, mid, end);

        #pragma omp taskwait
        count = count1 + count2;
    }

    return count;
}

int main() {
    long a[N], key = 42, nkey = 0;
    for (long i = 0; i < N; i++) a[i] = rand() % N;
    a[N % 43] = key; 
    a[N % 73] = key; 
    a[N % 3] = key;

    #pragma omp parallel
    {
        #pragma omp single
        nkey = count_key_parallel(a, key, 0, N);  // cuenta key en paralelo
    }

    printf("Numero de veces que 'key' aparece en paralelo: %ld\n", nkey);

    return 0;
}
