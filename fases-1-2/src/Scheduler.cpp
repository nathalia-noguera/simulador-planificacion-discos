#include "../include/Scheduler.hpp"
#include <iostream>
#include <fstream>
#include <cmath>

// Este algoritmo es O(n) porque recorre el arreglo de solicitudes una sola vez de forma lineal.
void ejecutarFCFS(Disk disco, std::vector<Request> solicitudes) {
    if (solicitudes.empty()) {
        std::cout << "La lista de solicitudes está vacía." << std::endl;
        return;
    }

    int tiempoTotal = 0;
    
    // CORRECCIÓN 1: Usamos tu función getter en inglés
    int posicionBrazo = disco.getCurrentHeadPosition(); 
    
    std::ofstream archivo("results/fcfs_resultados.txt");
    
    std::cout << "\n--- Resultados Algoritmo FCFS ---" << std::endl;
    archivo << "--- Resultados Algoritmo FCFS ---\n";

    for (int i = 0; i < solicitudes.size(); i++) {
        
        // CORRECCIÓN 2: Aquí asumo que la variable en tu Request.hpp se llama "cylinder". 
        // ¡Si se llama de otra forma, cámbiala en esta línea!
        int cilindroDestino = solicitudes[i].cylinder; 
        
        int distancia = std::abs(cilindroDestino - posicionBrazo);
        
        // CORRECCIÓN 3: Usamos tu función getter en inglés
        int tiempoMovimiento = distancia * disco.getTimePerCylinder(); 
        
        tiempoTotal = tiempoTotal + tiempoMovimiento;
        posicionBrazo = cilindroDestino;

        std::cout << "Visitando cilindro: " << cilindroDestino << " | Distancia: " << distancia << std::endl;
        archivo << "Visitando cilindro: " << cilindroDestino << " | Distancia: " << distancia << "\n";
    }

    int tiempoPromedio = tiempoTotal / solicitudes.size();
    
    std::cout << "\nTiempo total: " << tiempoTotal << " | Promedio: " << tiempoPromedio << std::endl;
    archivo << "\nTiempo total: " << tiempoTotal << " | Promedio: " << tiempoPromedio << "\n";
    
    archivo.close();
}