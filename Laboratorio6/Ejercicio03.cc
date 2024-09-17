#include <iostream>
#include <pthread.h>
#include <vector>
#include <cmath> // Para pow()

// Estructura para pasar los parámetros a los hilos
struct ThreadData {
    int n;
    double result;
};

// Función que calculará cada término de la serie
void* calculate_term(void* arg) {
    ThreadData* data = (ThreadData*)arg;
    int n = data->n;

    // Calcular el término de la serie: (-1)^(n+1) / n
    data->result = pow(-1, n+1) / n;

    // Finaliza el hilo retornando el resultado en la estructura
    pthread_exit(nullptr);

    // Agrega un return explícito para evitar warnings
    return nullptr;
}


int main() {
    int max_n;
    std::cout << "Ingrese el valor máximo de n: ";
    std::cin >> max_n;

    // Vector para almacenar los identificadores de los hilos
    std::vector<pthread_t> threads(max_n);
    std::vector<ThreadData> thread_data(max_n);

    // Crear los hilos
    for (int i = 0; i < max_n; ++i) {
        thread_data[i].n = i + 1; // Se empieza desde n = 1
        pthread_create(&threads[i], nullptr, calculate_term, &thread_data[i]);
    }

    // Variable para almacenar la suma de la serie
    double sum = 0.0;

    // Esperar a que los hilos terminen y acumular los resultados
    for (int i = 0; i < max_n; ++i) {
        pthread_join(threads[i], nullptr);
        sum += thread_data[i].result;
    }

    // Imprimir el resultado de la serie
    std::cout << "El valor de la serie es: " << sum << std::endl;

    return 0;
}
