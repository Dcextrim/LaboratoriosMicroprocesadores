#include <iostream>
#include <pthread.h>

using namespace std;

// Estructura para pasar parámetros a la función del hilo
struct thread_params {
    int start;    // Límite inferior de la secuencia de Fibonacci
    int end;      // Límite superior de la secuencia de Fibonacci
    int* fib_sum; // Puntero que almacena la suma de los números de Fibonacci
};

// Función que será ejecutada por el hilo para calcular la serie de Fibonacci
void* Fibonacci(void* params) {
    // Casteo el parámetro recibido a la estructura `thread_params`
    thread_params* p = (thread_params*)params;
    int start = p->start;  // Límite inferior
    int end = p->end;      // Límite superior
    int* fib_sum = p->fib_sum; // Puntero a la suma de los números de Fibonacci

    int a = 0, b = 1, temp;  // Variables para calcular la secuencia de Fibonacci
    *fib_sum = 0;            // Inicializo la suma en 0

    // Itero hasta alcanzar el valor límite `end`
    for (int i = 0; i < end; i++) {
        // Si el número actual de Fibonacci está dentro del rango permitido
        if (a <= end) {
            // Si el número es mayor o igual al inicio, lo incluimos en la suma
            if (a >= start) {
                *fib_sum += a;  // Sumar el número de Fibonacci a la suma total
                cout << "F" << i << "= " << a << endl; // Imprimir el número actual de Fibonacci
            }
            // Actualizamos los valores de `a` y `b` para la siguiente iteración de Fibonacci
            temp = a;
            a = b;
            b = temp + b;
        }
    }

    return NULL; // Fin de la función que ejecuta el hilo
}

int main() {
    int num;  // Número límite para la secuencia de Fibonacci
    cout << "Ingrese un número entre 0 y 100: ";
    cin >> num;

    // Verificamos que el número esté dentro del rango permitido
    if (num < 0 || num > 100) {
        cout << "Número fuera de rango. Debe estar entre 0 y 100." << endl;
        return 1;  // Finaliza el programa si el número no está dentro del rango
    }

    pthread_t thread;        // Identificador del hilo
    thread_params params;    // Estructura para los parámetros del hilo
    params.start = 0;        // Límite inferior de Fibonacci (empieza en 0)
    params.end = num;        // Límite superior (el número ingresado por el usuario)
    params.fib_sum = new int; // Reserva memoria dinámica para almacenar la suma

    // Creación del hilo que calculará la secuencia de Fibonacci
    pthread_create(&thread, NULL, Fibonacci, &params);

    // Espera a que el hilo termine su ejecución
    pthread_join(thread, NULL);

    // Imprimir la suma total de los números de Fibonacci encontrados
    cout << "Suma total de los números de la serie de Fibonacci: " << *params.fib_sum << endl;

    // Liberar la memoria reservada dinámicamente
    delete params.fib_sum;

    return 0;  // Finaliza el programa
}
