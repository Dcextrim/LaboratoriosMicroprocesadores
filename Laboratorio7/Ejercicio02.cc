#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

std::mutex mtx; 
std::condition_variable cv; 
int silo = 0; 
int bodega = 0; 

const int CAPACIDAD_SILO = 5;
const int BOLSAS_OBJETIVO = 400;

void tostadora1() {
    while (bodega < BOLSAS_OBJETIVO) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simula producción de 1 lb cada segundo
        std::unique_lock<std::mutex> lock(mtx);
        silo += 1;
        std::cout << "Tostadora 1: produjo 1 lb, silo: " << silo << " lbs\n";
        cv.notify_all(); // Notifica a la empacadora si el silo alcanza las 5 lbs
    }
}

void tostadora2() {
    while (bodega < BOLSAS_OBJETIVO) {
        std::this_thread::sleep_for(std::chrono::seconds(1)); // Simula producción de 1 lb cada segundo
        std::unique_lock<std::mutex> lock(mtx);
        silo += 1;
        std::cout << "Tostadora 2: produjo 1 lb, silo: " << silo << " lbs\n";
        cv.notify_all(); // Notifica a la empacadora si el silo alcanza las 5 lbs
    }
}

void empacadora() {
    while (bodega < BOLSAS_OBJETIVO) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait(lock, [] { return silo >= CAPACIDAD_SILO || bodega >= BOLSAS_OBJETIVO; }); // Espera hasta que haya al menos 5 lbs en el silo
        if (bodega >= BOLSAS_OBJETIVO) break;
        
        while (silo > 0 && bodega < BOLSAS_OBJETIVO) {
            std::this_thread::sleep_for(std::chrono::seconds(1)); // Simula empaquetado de 1 bolsa por segundo
            silo -= 1;
            bodega += 1;
            std::cout << "Empacadora: empacó 1 bolsa, bodega: " << bodega << " bolsas, silo: " << silo << " lbs\n";
        }
    }
}

int main() {
    // Crear hilos para las tostadoras y empacadora
    std::thread t1(tostadora1);
    std::thread t2(tostadora2);
    std::thread emp(empacadora);

    // Esperar que terminen
    t1.join();
    t2.join();
    emp.join();

    std::cout << "Producción completada: " << bodega << " bolsas en la bodega\n";
    return 0;
}
