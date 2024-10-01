#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <unistd.h>
#include <cstdio>

using namespace std;

const int NUM_PRODUCTOS = 8;
const int NUM_MESES = 2;

struct Producto {
    string nombre;
    vector<double> precio_unitario;
    vector<double> costo_fijo;
    vector<int> unidades_vendidas;
    vector<double> ventas;
    vector<double> utilidad;
};

vector<Producto> productos(NUM_PRODUCTOS);
vector<double> monto_total_ventas(NUM_MESES, 0.0);
vector<double> utilidad_total(NUM_MESES, 0.0);

pthread_mutex_t candado; // Mutex para proteger el acceso a las variables compartidas
pthread_cond_t calculo_terminado; // Variable condicional para esperar a que todos los cálculos se terminen
int productos_calculados = 0; // Contador de productos ya calculados

// Función para calcular ventas y utilidad por producto y mes
void calcular_producto(int id_producto) {
    for (int mes = 0; mes < NUM_MESES; ++mes) {
        double ventas = productos[id_producto].precio_unitario[mes] * productos[id_producto].unidades_vendidas[mes];
        double utilidad = ventas - (productos[id_producto].costo_fijo[mes] * productos[id_producto].unidades_vendidas[mes]);

        pthread_mutex_lock(&candado); // Bloqueo para proteger las variables compartidas
        
        productos[id_producto].ventas[mes] = ventas;
        productos[id_producto].utilidad[mes] = utilidad;
        monto_total_ventas[mes] += ventas;
        utilidad_total[mes] += utilidad;
        productos_calculados++; // Incrementa el número de productos calculados

        printf("Calculo hilo %d para mes %d iniciado\n", id_producto, mes + 1);

        if (productos_calculados == NUM_PRODUCTOS) { 
            // Si todos los productos han sido calculados, notifica que el cálculo ha terminado
            pthread_cond_broadcast(&calculo_terminado); 
        }

        pthread_mutex_unlock(&candado); // Libera el mutex

        usleep(1000000); // Simula un segundo

        pthread_mutex_lock(&candado);
        printf("Calculo hilo %d para mes %d terminado\n", id_producto, mes + 1);
        pthread_mutex_unlock(&candado);
    }
}

// Función para inicializar los datos de los productos
void inicializar_datos() {
    vector<string> nombres = {"Porcion pastel de chocolate", "White mocha", "Café americano 8onz", "Latte 8onz", 
                              "Toffee coffee", "Cappuccino 8onz", "S'mores Latte", "Café tostado molido"};

    vector<vector<double>> precios = {{60.00, 60.00}, {32.00, 32.00}, {22.00, 22.00}, {24.00, 24.00},
                                      {28.00, 28.00}, {24.00, 24.00}, {32.00, 32.00}, {60.00, 60.00}};

    vector<vector<double>> costos = {{20.00, 20.00}, {19.20, 19.20}, {13.20, 13.20}, {17.20, 17.20},
                                     {20.10, 20.10}, {17.20, 17.20}, {23.00, 23.00}, {20.00, 20.00}};

    vector<vector<int>> unidades = {{300, 250}, {400, 380}, {1590, 800}, {200, 250},
                                    {390, 600}, {1455, 1200}, {800, 1540}, {60, 15}};

    for (int i = 0; i < NUM_PRODUCTOS; ++i) {
        productos[i].nombre = nombres[i];
        productos[i].precio_unitario = precios[i];
        productos[i].costo_fijo = costos[i];
        productos[i].unidades_vendidas = unidades[i];
        productos[i].ventas.resize(NUM_MESES, 0.0);
        productos[i].utilidad.resize(NUM_MESES, 0.0);
    }
}

// Función para imprimir los resultados del mes
void imprimir_resultados(int mes) {
    pthread_mutex_lock(&candado);

    // Espera a que todos los productos se hayan calculado antes de imprimir
    while (productos_calculados < NUM_PRODUCTOS) {
        pthread_cond_wait(&calculo_terminado, &candado);
    }

    printf("\n----------------------------------------\n");
    printf("REPORTE DEL MES DE %s\n", (mes == 0 ? "JULIO" : "AGOSTO"));
    
    printf("\n--- Monto Ventas por Producto ---\n");
    for (const auto& producto : productos) {
        printf("- %s: Q%.2f\n", producto.nombre.c_str(), producto.ventas[mes]);
    }

    printf("\n--- Utilidad por Producto ---\n");
    for (const auto& producto : productos) {
        printf("- %s: Q%.2f\n", producto.nombre.c_str(), producto.utilidad[mes]);
    }

    printf("\n--- Monto Total Ventas del Mes: Q%.2f\n", monto_total_ventas[mes]);
    printf("--- Utilidad del mes: Q%.2f\n", utilidad_total[mes]);

    pthread_mutex_unlock(&candado);
}

int main() {
    pthread_t hilos[NUM_PRODUCTOS];
    productos_calculados = 0; // Inicializa el contador de productos calculados

    // Inicializa el mutex y las variables de condición
    pthread_mutex_init(&candado, NULL);
    pthread_cond_init(&calculo_terminado, NULL);

    inicializar_datos();

    // Crear los hilos para calcular los productos
    for (int i = 0; i < NUM_PRODUCTOS; ++i) {
        pthread_create(&hilos[i], NULL, (void* (*)(void*))calcular_producto, (void*)(intptr_t)i);
    }

    // Esperar a que todos los hilos terminen
    for (int i = 0; i < NUM_PRODUCTOS; ++i) {
        pthread_join(hilos[i], NULL);
    }

    // Imprimir resultados para los meses
    imprimir_resultados(0); // Julio
    imprimir_resultados(1); // Agosto

    // Limpiar el mutex y las variables condicionales
    pthread_mutex_destroy(&candado);
    pthread_cond_destroy(&calculo_terminado);

    return 0;
}
