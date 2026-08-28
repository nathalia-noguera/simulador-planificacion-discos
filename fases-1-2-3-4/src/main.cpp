#include "../include/Scheduler.hpp"
#include <iostream>
#include "../include/Disk.hpp"
#include "../include/Request.hpp"

#ifdef _WIN32 // Para mostrar bien los acentos en consola (solo para Windows).
    #include <windows.h>
#endif

int main(){
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif
    std::cout << ">>> SIMULADOR DE DISCO - FASE 2 <<<\n\n";

    // 1. Configurar el disco.
    int userCilindros, userPosicion;
    double userTiempo;
    std::cout << "Configuración del Disco Duro Virtual:\n";
    std::cout << "Ingrese el número total de cilindros: ";
    std::cin >> userCilindros;
    std::cout << "Ingrese la posición inicial del cabezal: ";
    std::cin >> userPosicion;
    std::cout << "Ingrese el tiempo en ms por cada salto de cilindro: ";
    std::cin >> userTiempo;
    Disk miDisk(userCilindros, userPosicion, userTiempo);
    std::cout << "\n[OK] Disco creado exitosamente.\n";
    miDisk.printStatus();

    // 2. Probar las distintas formas de cargar solicitudes.
    std::vector<Request> requests;
    int opcion, cantidad;
    std::cout << "\n¿Cómo desea cargar las solicitudes?\n";
    std::cout << "1. Generar al azar.\n";
    std::cout << "2. Leer desde un archivo.\n";
    std::cout << "3. Escribirlas a mano.\n";
    std::cout << "Opción: ";
    std::cin >> opcion;

    switch(opcion){
        case 1:
            std::cout << "¿Cuántas solicitudes se van a generar?: ";
            std::cin >> cantidad;
            requests = generateRandomRequests(cantidad, miDisk.getTotalCylinders());
            break;
        case 2:{
            // Las llaves {} son necesarias porque se declara una variable dentro del case.
            std::string nombreArchivo;
            std::cout << "Nombre del archivo (ej. data/requests.txt): ";
            std::cin >> nombreArchivo;
            requests = loadRequestsFromFile(nombreArchivo, miDisk.getTotalCylinders());
            break;
        }
        case 3:
            requests = inputRequestsManually(miDisk.getTotalCylinders());
            break;
        default:
            std::cout << "[ERROR] Opción inválida.\n";
            break;
    }

    // 3. Mostrar las solicitudes cargadas
    std::cout << "\n";
    printRequests(requests);
    std::cout << "\n>>> FASE 2 FINALIZADA CORRECTAMENTE <<<\n";

    // 4. Ejecutar los planificadores después de cargar los datos
    if (!requests.empty()) {
        int algOpt;
        std::cout << "\n¿Qué algoritmo(s) desea ejecutar?\n";
        std::cout << "1. FCFS (First-Come, First-Served)\n";
        std::cout << "2. SSTF (Shortest Seek Time First)\n";
        std::cout << "3. SCAN (Algoritmo del Ascensor)\n";
        std::cout << "4. C-SCAN (Circular SCAN, ¡Nuevo!)\n";
        std::cout << "5. Ejecutar todos (Para comparar resultados y algoritmos en conjunto)\n";
        std::cout << "Opción: ";
        std::cin >> algOpt;

        int globalDirection = 1; // Default
        if (algOpt == 3 || algOpt == 4 || algOpt == 5) {
            std::cout << "\n[Configuración de brazo] Ingrese la dirección inicial del brazo (1 para subir/derecha, 0 para bajar/izquierda): ";
            std::cin >> globalDirection;
        }

        if (algOpt == 1 || algOpt == 5) {
            std::cout << "\n>>> EJECUTANDO PLANIFICADOR FCFS <<<\n";
            ejecutarFCFS(miDisk, requests);
        }
        if (algOpt == 2 || algOpt == 5) {
            std::cout << "\n>>> EJECUTANDO PLANIFICADOR SSTF <<<\n";
            ejecutarSSTF(miDisk, requests);
        }
        if (algOpt == 3 || algOpt == 5) {
            std::cout << "\n>>> EJECUTANDO PLANIFICADOR SCAN <<<\n";
            ejecutarSCAN(miDisk, requests, globalDirection);
        }
        if (algOpt == 4 || algOpt == 5) {
            std::cout << "\n>>> EJECUTANDO PLANIFICADOR C-SCAN <<<\n";
            ejecutarCSCAN(miDisk, requests, globalDirection);
        }
    }

    std::cout << "\n>>> FASES COMPLETADAS CORRECTAMENTE (incluyendo C-SCAN) <<<\n";

    return 0;
}