#include "../include/Scheduler.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>

// Este algoritmo es O(n) porque recorre el arreglo de solicitudes una sola vez de forma lineal.
int ejecutarFCFS(Disk disco, std::vector<Request> solicitudes) {
    if (solicitudes.empty()) {
        std::cout << "La lista de solicitudes está vacía." << std::endl;
        return 0;
    }

    int tiempoTotal = 0;
    
    // CORRECCIÓN 1: Usamos tu función getter en inglés
    int posicionBrazo = disco.getCurrentHeadPosition(); 
    
    std::ofstream archivo("results/fcfs_resultados.txt");
    
    std::cout << "\n--- Resultados Algoritmo FCFS ---" << std::endl;
    archivo << "--- Resultados Algoritmo FCFS ---\n";

    for (size_t i = 0; i < solicitudes.size(); i++) {
        
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
    return tiempoTotal;
}

// Este algoritmo es O(n^2) porque por cada solicitud debe buscar linealmente cuál es la más cercana.
int ejecutarSSTF(Disk disco, std::vector<Request> solicitudes) {
    if (solicitudes.empty()) {
        std::cout << "La lista de solicitudes está vacía." << std::endl;
        return 0;
    }

    int tiempoTotal = 0;
    int posicionBrazo = disco.getCurrentHeadPosition(); 
    
    // Forma de marcar cuáles solicitudes ya se atendieron
    std::vector<bool> atendido(solicitudes.size(), false);
    
    std::ofstream archivo("results/sstf_resultados.txt");
    
    std::cout << "\n--- Resultados Algoritmo SSTF ---" << std::endl;
    archivo << "--- Resultados Algoritmo SSTF ---\n";

    for (size_t i = 0; i < solicitudes.size(); i++) {
        int min_distancia = 99999999;
        int ind_cercano = -1;
        bool huboEmpate = false;
        int cilindroEmpateRechazado = -1;

        // En cada paso, buscar la solicitud más cercana al brazo
        for (size_t j = 0; j < solicitudes.size(); j++) {
            if (!atendido[j]) {
                int distancia = std::abs(solicitudes[j].cylinder - posicionBrazo);
                if (distancia < min_distancia) {
                    min_distancia = distancia;
                    ind_cercano = j;
                    huboEmpate = false; // Se encontró uno más cercano, se reinicia la bandera
                } else if (distancia == min_distancia) {
                    // Decidir qué hacer cuando dos solicitudes están a la misma distancia
                    huboEmpate = true;
                    // Opcionalmente preguntarlo, pero para no saturar al usuario,
                    // dejamos la justificación impresa en pantalla automáticamente:
                    if (solicitudes[j].cylinder < solicitudes[ind_cercano].cylinder) {
                        cilindroEmpateRechazado = solicitudes[ind_cercano].cylinder;
                        ind_cercano = j;
                    } else {
                        cilindroEmpateRechazado = solicitudes[j].cylinder;
                    }
                }
            }
        }

        int cilindroDestino = solicitudes[ind_cercano].cylinder;

        // Mover el brazo e imprimir qué pasa en caso de empate
        if (huboEmpate) {
            std::cout << "  [!] EMPATE DETECTADO: El cilindro " << cilindroDestino 
                      << " y el " << cilindroEmpateRechazado << " están a la misma distancia (" << min_distancia << ")." << std::endl;
            std::cout << "      -> Decisión: Se escogió el cilindro " << cilindroDestino << " por ser el número menor." << std::endl;
        }

        // Marcar la solicitud como atendida
        atendido[ind_cercano] = true;
        
        int tiempoMovimiento = min_distancia * disco.getTimePerCylinder();
        tiempoTotal += tiempoMovimiento;
        
        std::cout << "  [Ok] Moviendo brazo a cilindro: " << cilindroDestino 
                  << " | Marcando como atendida." << std::endl;
        std::cout << "       Distancia: " << min_distancia 
                  << " | Tiempo sumado: " << tiempoMovimiento << " ms" << std::endl;

        posicionBrazo = cilindroDestino;
        
        archivo << "Visitando cilindro: " << cilindroDestino << " | Distancia: " << min_distancia << "\n";
    }

    int tiempoPromedio = tiempoTotal / solicitudes.size();
    
    std::cout << "\nTiempo total de movimiento: " << tiempoTotal << " ms | Tiempo Promedio: " << tiempoPromedio << " ms\n" << std::endl;
    archivo << "\nTiempo total: " << tiempoTotal << " | Promedio: " << tiempoPromedio << "\n";
    
    archivo.close();
    return tiempoTotal;
}

// Diferencia entre SCAN y LOOK:
// SCAN: Siempre llega hasta el extremo absoluto del disco (0 o totalCylinders-1) antes de devolverse,
//       sin importar si ya no quedan más solicitudes reales en ese trayecto final.
// LOOK: Solo llega hasta la ÚLTIMA solicitud en esa dirección. Al atenderla, se devuelve 
//       inmediatamente sin tener que gastar tiempo viajando inútilmente hasta el borde del disco entero.

// Este algoritmo es típicamente O(n log n) debido a la necesidad de ordenar el arreglo de solicitudes inicialmente.
int ejecutarSCAN(Disk disco, std::vector<Request> solicitudes, int direccion) {
    if (solicitudes.empty()) {
        std::cout << "La lista de solicitudes está vacía." << std::endl;
        return 0;
    }

    int tiempoTotal = 0;
    int posicionBrazo = disco.getCurrentHeadPosition();
    int totalCilindros = disco.getTotalCylinders();

    // 2. Hacer una copia ordenada de las solicitudes (solicitudes ya es una copia por valor)
    std::sort(solicitudes.begin(), solicitudes.end(), [](const Request& a, const Request& b) {
        return a.cylinder < b.cylinder; // Orden ascendente
    });

    // 3. Separar las solicitudes según la dirección en que va el brazo
    std::vector<int> izquierda, derecha;
    for (size_t i = 0; i < solicitudes.size(); i++) {
        if (solicitudes[i].cylinder < posicionBrazo) {
            izquierda.push_back(solicitudes[i].cylinder);
        } else if (solicitudes[i].cylinder > posicionBrazo) {
            derecha.push_back(solicitudes[i].cylinder);
        } else {
            // Si hay una solicitud exactamente donde está el brazo, se atiende de inmediato
            std::cout << "  [Ok] Solicitud en la misma posición inicial (" << posicionBrazo << ") atendida de inmediato (0 ms).\n";
        }
    }

    std::ofstream archivo("results/scan_resultados.txt");
    std::cout << "\n--- Resultados Algoritmo SCAN ---" << std::endl;
    archivo << "--- Resultados Algoritmo SCAN ---\n";

    // Decisión de implementación (SCAN clásico):
    std::cout << "  [INFO] Versión elegida: SCAN Clásico (El brazo tocará el extremo del disco antes de devolverse).\n";

    std::vector<int> ordenRecorrido;
    
    // Función auxiliar para moverse e imprimir registro
    auto moverBrazo = [&](int cilindroDestino, std::string nota) {
        int distancia = std::abs(cilindroDestino - posicionBrazo);
        int tiempo = distancia * disco.getTimePerCylinder();
        tiempoTotal += tiempo;
        posicionBrazo = cilindroDestino;
        
        std::cout << "  [v] " << nota << cilindroDestino 
                  << " | Distancia: " << distancia 
                  << " | Suma tiempo: " << tiempo << " ms\n";
        archivo << "Visitando: " << cilindroDestino << " | Distancia: " << distancia << "\n";
    };

    if (direccion == 1) { // Moviendo hacia arriba (derecha)
        // Atender primero todas las de la dirección actual
        for (int cil : derecha) {
            moverBrazo(cil, "Atendido obj. en subida -> ");
        }
        // Tocar el extremo SIEMPRE (SCAN Clásico) si hay algo atrás. (Aunque si está en el extremo, distancia es 0).
        if (!izquierda.empty()) {
            moverBrazo(totalCilindros - 1, "Tocando el extremo (Final) -> ");
            // Devolverse en sentido contrario (de mayor a menor)
            for (int i = izquierda.size() - 1; i >= 0; i--) {
                moverBrazo(izquierda[i], "Atendido obj. en bajada -> ");
            }
        }
    } else { // Moviendo hacia abajo (izquierda)
        // Atender primero todas las de la dirección actual (bajando)
        for (int i = izquierda.size() - 1; i >= 0; i--) {
            moverBrazo(izquierda[i], "Atendido obj. en bajada -> ");
        }
        // Tocar el extremo
        if (!derecha.empty()) {
            moverBrazo(0, "Tocando el extremo (Inicio) -> ");
            // Devolverse en sentido contrario (subiendo)
            for (int cil : derecha) {
                moverBrazo(cil, "Atendido obj. en subida -> ");
            }
        }
    }

    int tiempoPromedio = tiempoTotal / solicitudes.size();
    
    std::cout << "\nTiempo total de movimiento: " << tiempoTotal << " ms | Promedio: " << tiempoPromedio << " ms\n" << std::endl;
    archivo << "\nTiempo total: " << tiempoTotal << " | Promedio: " << tiempoPromedio << "\n";
    
    archivo.close();
    return tiempoTotal;
}

// Implementación de Circular SCAN (C-SCAN)
int ejecutarCSCAN(Disk disco, std::vector<Request> solicitudes, int direccion) {
    if (solicitudes.empty()) {
        std::cout << "La lista de solicitudes está vacía." << std::endl;
        return 0;
    }

    int tiempoTotal = 0;
    int posicionBrazo = disco.getCurrentHeadPosition();
    int totalCilindros = disco.getTotalCylinders();

    std::sort(solicitudes.begin(), solicitudes.end(), [](const Request& a, const Request& b) {
        return a.cylinder < b.cylinder; // Orden ascendente
    });

    std::vector<int> izquierda, derecha;
    for (size_t i = 0; i < solicitudes.size(); i++) {
        if (solicitudes[i].cylinder < posicionBrazo) {
            izquierda.push_back(solicitudes[i].cylinder);
        } else if (solicitudes[i].cylinder > posicionBrazo) {
            derecha.push_back(solicitudes[i].cylinder);
        } else {
            std::cout << "  [Ok] Solicitud en la misma posición inicial (" << posicionBrazo << ") atendida de inmediato (0 ms).\n";
        }
    }

    std::ofstream archivo("results/cscan_resultados.txt");
    std::cout << "\n--- Resultados Algoritmo C-SCAN ---" << std::endl;
    archivo << "--- Resultados Algoritmo C-SCAN ---\n";
    std::cout << "  [INFO] C-SCAN viaja en una dirección, al extremo SALTA al otro extremo, y sigue en la MISMA dirección.\n";

    auto moverBrazo = [&](int cilindroDestino, std::string nota) {
        int distancia = std::abs(cilindroDestino - posicionBrazo);
        int tiempo = distancia * disco.getTimePerCylinder();
        tiempoTotal += tiempo;
        posicionBrazo = cilindroDestino;
        
        std::cout << "  [v] " << nota << cilindroDestino 
                  << " | Distancia: " << distancia 
                  << " | Suma tiempo: " << tiempo << " ms\n";
        archivo << "Visitando: " << cilindroDestino << " | Distancia: " << distancia << "\n";
    };

    if (direccion == 1) { // Moviendo hacia arriba (derecha)
        for (int cil : derecha) {
            moverBrazo(cil, "Atendido obj. en subida -> ");
        }
        if (!izquierda.empty()) {
            moverBrazo(totalCilindros - 1, "Llegando al extremo superior -> ");
            moverBrazo(0, "SALTO al comienzo del disco -> ");
            for (int cil : izquierda) {
                moverBrazo(cil, "Atendido obj. en subida -> ");
            }
        }
    } else { // Moviendo hacia abajo (izquierda)
        for (int i = izquierda.size() - 1; i >= 0; i--) {
            moverBrazo(izquierda[i], "Atendido obj. en bajada -> ");
        }
        if (!derecha.empty()) {
            moverBrazo(0, "Llegando al extremo inferior -> ");
            moverBrazo(totalCilindros - 1, "SALTO al final del disco -> ");
            for (int i = derecha.size() - 1; i >= 0; i--) {
                moverBrazo(derecha[i], "Atendido obj. en bajada -> ");
            }
        }
    }

    int tiempoPromedio = tiempoTotal / solicitudes.size();
    
    std::cout << "\nTiempo total de movimiento: " << tiempoTotal << " ms | Promedio: " << tiempoPromedio << " ms\n" << std::endl;
    archivo << "\nTiempo total: " << tiempoTotal << " | Promedio: " << tiempoPromedio << "\n";
    
    archivo.close();
    return tiempoTotal;
}