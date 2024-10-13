
#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

using namespace std;

const char* productos[] = {"Pata", "Respaldo", "Asiento", "Pata", "Pata"};
const int numProductos = 5;  
const int MAX_BUFFER = 5;    
const int MAX_SILLAS = 3;    

int buffer[MAX_BUFFER];       
int in = 0;                   
int out = 0;                  
int sillasProducidas = 0;    
int piezasProducidas[3] = {0, 0, 0}; // Contador de piezas fabricadas: Pata, Respaldo, Asiento

// Semáforos y mutex
sem_t vacios;   
sem_t llenos;   
pthread_mutex_t mutex;

// Función  simulación de un productor (fabrica una pieza de silla)
void* productor(void* arg) {
    int id = *(int*)arg;
    int piezaId;
    
    while (true) {
        if (sillasProducidas >= MAX_SILLAS) break;  // Detiene la producción si se alcanza el máximo de sillas
        piezaId = rand() % numProductos;  // Seleccionar una pieza  al azar

        sem_wait(&vacios);  // Espera hasta que hay espacio en el buffer
        pthread_mutex_lock(&mutex);  // Protege el acceso al buffer

        buffer[in] = piezaId;  // Añade la pieza al buffer
        cout << "Productor " << id << " ha fabricado la pieza " << productos[piezaId]
             << " y la coloco en la posicion " << in << endl;
        piezasProducidas[piezaId % 3]++;  // Incrementar el contador de piezas
        in = (in + 1) % MAX_BUFFER;  // Avanza el índice circular del buffer

        pthread_mutex_unlock(&mutex);
        sem_post(&llenos);  // Incrementa el número de productos disponibles
        
        sleep(1);  // Simula el tiempo de fabricación
    }
    
    return NULL;
}

// Función simulación de un consumidor (ensambla una silla)
void* consumidor(void* arg) {
    int id = *(int*)arg;
    int piezas[3] = {0, 0, 0};  // Piezas recogidas por el consumidor: Pata, Respaldo, Asiento
    
    while (true) {
        if (sillasProducidas >= MAX_SILLAS) break;  // Detiene el consumo si se alcanza el máximo de sillas

        sem_wait(&llenos);  // Espera hasta que haya productos en el buffer
        pthread_mutex_lock(&mutex);  // Protege el acceso al buffer

        int piezaId = buffer[out];  // Retira la pieza del buffer
        cout << "Consumidor " << id << " ha retirado la pieza " << productos[piezaId]
             << " de la posicion " << out << endl;
        out = (out + 1) % MAX_BUFFER;  // Avanza el índice circular del buffer
        
        pthread_mutex_unlock(&mutex);
        sem_post(&vacios);  // Libera espacio en el buffer

        // Almacena la pieza en el consumidor (para ensamblar la silla)
        piezas[piezaId % 3]++;
        
        // Si se han recogido suficientes piezas para ensamblar una silla (1 respaldo, 1 asiento, 4 patas)
        if (piezas[0] >= 4 && piezas[1] >= 1 && piezas[2] >= 1) {
            sillasProducidas++;
            cout << "Consumidor " << id << " ha ensamblado una silla! Total sillas: " << sillasProducidas << endl;
            piezas[0] -= 4; piezas[1]--; piezas[2]--;  // Resta las piezas utilizadas
        }

        sleep(1);  // Simula el tiempo de ensamblaje
    }

    return NULL;
}

// Función principal
int main() {
    pthread_t productores[2], consumidores[2];
    int ids[2] = {1, 2};

    // Inicializar semáforos
    sem_init(&vacios, 0, MAX_BUFFER);
    sem_init(&llenos, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    // Crear hilos productores
    for (int i = 0; i < 2; i++) {
        pthread_create(&productores[i], NULL, productor, &ids[i]);
    }

    // Crear hilos consumidores
    for (int i = 0; i < 2; i++) {
        pthread_create(&consumidores[i], NULL, consumidor, &ids[i]);
    }

    // Esperar que los hilos terminen
    for (int i = 0; i < 2; i++) {
        pthread_join(productores[i], NULL);
        pthread_join(consumidores[i], NULL);
    }

    // Generar reporte al finalizar
    cout << "Reporte final:" << endl;
    cout << "Sillas producidas: " << sillasProducidas << endl;
    cout << "Piezas sobrantes: " << endl;
    cout << "Patas: " << piezasProducidas[0] % 4 << endl;
    cout << "Respaldos: " << piezasProducidas[1] << endl;
    cout << "Asientos: " << piezasProducidas[2] << endl;

    // Destruir semáforos y mutex
    sem_destroy(&vacios);
    sem_destroy(&llenos);
    pthread_mutex_destroy(&mutex);

    return 0;
}
