#include <iostream>   // Para manejar la entrada y salida de datos (uso de cin y cout).
#include <pthread.h>  // Biblioteca para crear y manejar hilos utilizando Pthreads.
#include <semaphore.h> // Biblioteca para manejar semáforos (sincronización entre hilos).
#include <vector>     // Biblioteca para usar el contenedor vector.
#include <unistd.h>   // Biblioteca que contiene la función sleep, para simular tiempo de espera.
 
using namespace std;  // Facilita el uso de elementos del estándar C++ sin tener que escribir std:: constantemente.
 
// Estructura que contiene los datos de cada cliente (id y monto que intentará retirar).
struct ClienteData {
    int id;           // Identificador único del cliente.
    double monto;     // Monto que el cliente desea retirar.
};

// Variables globales
double saldo = 100000.00;  // Se inicializa el saldo de la cuenta con Q100,000.
sem_t sem;  // Semáforo para controlar el acceso al cajero, permitiendo solo a un cliente usarlo a la vez.


// Función que será ejecutada por cada hilo, simulando el retiro de dinero.
void* retirarDinero(void* arg) {
    // Convertir el argumento recibido (de tipo void) a un puntero de tipo ClienteData.
    ClienteData* data = (ClienteData*)arg;
    
    // Semáforo: el hilo esperará aquí si otro hilo ya está usando el cajero.
    sem_wait(&sem);
    
    // Mensaje que indica que el cliente ha accedido al cajero.
    cout << "Cliente " << data->id << " está usando el cajero." << endl;
    
    // Simula el tiempo que tarda el cliente en hacer la operación, aquí se pausa por 1 segundo.
    sleep(1);
    
    // Verifica si el saldo es suficiente para permitir el retiro.
    if (saldo >= data->monto) {
        // Si el saldo es suficiente, se resta el monto solicitado al saldo.
        saldo -= data->monto;
        // Mensaje que indica que el retiro fue exitoso, mostrando el saldo restante.
        cout << "Cliente " << data->id << " retiró Q" << data->monto 
             << ". Saldo restante: Q" << saldo << endl;
    } else {
        // Si el saldo es insuficiente, se muestra un mensaje de error.
        cout << "Cliente " << data->id << " no pudo retirar Q" << data->monto 
             << ". Saldo insuficiente." << endl;
    }
    
    // Se libera el semáforo, permitiendo que otro hilo (cliente) pueda usar el cajero.
    sem_post(&sem);
    
    // Finaliza la ejecución del hilo.
    pthread_exit(NULL);
}

int main() {
    int numClientes;  // Variable para almacenar la cantidad de clientes que usarán el cajero.
    
    // Solicitar al usuario la cantidad de clientes (hilos) que van a utilizar el cajero.
    cout << "Ingrese la cantidad de clientes: ";
    cin >> numClientes;  // Guardar el número ingresado por el usuario.
    
    // Crear un vector de hilos (uno por cliente).
    vector<pthread_t> hilos(numClientes);
    // Crear un vector para almacenar los datos de cada cliente.
    vector<ClienteData> clientesData(numClientes);
    
    // Se ingresan los montos de retiro para cada cliente.
    for (int i = 0; i < numClientes; i++) {
        // Asignar un identificador único para cada cliente.
        clientesData[i].id = i + 1;
        // Pedir al usuario que ingrese el monto que desea retirar cada cliente.
        cout << "Ingrese el monto a retirar para el cliente " << clientesData[i].id << ": ";
        cin >> clientesData[i].monto;  // Guardar el monto ingresado.
    }
    
    // Inicializar el semáforo con valor 1, permitiendo que solo un cliente use el cajero a la vez.
    sem_init(&sem, 0, 1);
    
    // Crear los hilos para cada cliente.
    for (int i = 0; i < numClientes; i++) {
        // Crear un hilo que ejecutará la función retirarDinero, pasándole los datos del cliente correspondiente.
        pthread_create(&hilos[i], NULL, retirarDinero, (void*)&clientesData[i]);
    }
    
    // Esperar a que todos los hilos terminen su ejecución antes de continuar.
    for (int i = 0; i < numClientes; i++) {
        pthread_join(hilos[i], NULL);  // Bloquea hasta que el hilo i haya terminado.
    }
    
    // Destruir el semáforo, liberando recursos del sistema.
    sem_destroy(&sem);
    
    // Imprimir el saldo final una vez que todas las transacciones han sido completadas.
    cout << "Transacciones completadas. Saldo final: Q" << saldo << endl;
    
    return 0;  // Termina el programa con éxito.
}
