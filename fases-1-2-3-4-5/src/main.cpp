#include "../include/Scheduler.hpp"
#include <iostream>
#include <fstream>
#include <string>
#include <limits>
#include "../include/Disk.hpp"
#include "../include/Request.hpp"

#ifdef _WIN32 
    #include <windows.h>
#endif

// Función para limpiar cin en caso de error
void limpiarCin() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

int main(){
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // Inicialización del Disco Duro (Valores por defecto)
    Disk miDisk(200, 50, 1.0); 
    std::vector<Request> requests;

    std::cout << "=========================================\n";
    std::cout << ">>> SIMULADOR DE PLANIFICACIÓN DISCOS <<<\n";
    std::cout << ">>> Proyecto Arquitectura de C.       <<<\n";
    std::cout << "=========================================\n";
    std::cout << "\n[FASES 1-2-3-4-5-6] LISTAS Y OPERATIVAS!\n";
    std::cout << "[INFO] Configuracion actual del Disco:\n";
    miDisk.printStatus();

    bool salir = false;

    while(!salir) {
        std::cout << "\n-----------------------------------------\n";
        std::cout << "              MENÚ PRINCIPAL           \n";
        std::cout << "-----------------------------------------\n";
        std::cout << " 1. Cargar solicitudes desde archivo.\n";
        std::cout << " 2. Generar solicitudes al azar.\n";
        std::cout << " 3. Ingresar solicitudes a mano.\n";
        std::cout << " 4. Cambiar configuración del Disco Duro.\n";
        std::cout << " 5. Ver lista de solicitudes actual.\n";
        std::cout << " 6. Ejecutar: FCFS.\n";
        std::cout << " 7. Ejecutar: SSTF.\n";
        std::cout << " 8. Ejecutar: SCAN.\n";
        std::cout << " 9. Ejecutar: C-SCAN.\n";
        std::cout << "10. EJECUTAR TODOS y crear Tabla Comparativa.\n";
        std::cout << " 0. Salir del programa.\n";
        std::cout << "-----------------------------------------\n";
        std::cout << "Opción: ";
        
        int opcion;
        if (!(std::cin >> opcion)) {
            std::cout << "[ERROR] Entrada inválida. Por favor, introduzca un número.\n";
            limpiarCin();
            continue;
        }

        switch(opcion) {
            case 1: {
                std::string nombreArchivo;
                std::cout << "Nombre del archivo (ej. data/requests.txt): ";
                std::cin >> nombreArchivo;
                requests = loadRequestsFromFile(nombreArchivo, miDisk.getTotalCylinders());
                
                if(requests.empty()) {
                    std::cout << "[ADVERTENCIA] No se cargaron solicitudes (archivo no existe o vacío).\n";
                } else {
                    std::cout << "[OK] Solicitudes cargadas.\n";
                }
                break;
            }
            case 2: {
                int cantidad;
                std::cout << "¿Cuántas solicitudes se van a generar aleatoriamente?: ";
                if(std::cin >> cantidad && cantidad > 0) {
                    requests = generateRandomRequests(cantidad, miDisk.getTotalCylinders());
                    std::cout << "[OK] Solicitudes generadas exitosamente.\n";
                } else {
                    std::cout << "[ERROR] Cantidad inválida.\n";
                    limpiarCin();
                }
                break;
            }
            case 3:
                requests = inputRequestsManually(miDisk.getTotalCylinders());
                break;
            case 4: {
                int userCilindros, userPosicion;
                double userTiempo;
                std::cout << "\n>>> Configurando el Disco Duro <<<\n";
                std::cout << "Ingrese el número total de cilindros: ";
                std::cin >> userCilindros;
                std::cout << "Ingrese la posición inicial del cabezal: ";
                std::cin >> userPosicion;
                std::cout << "Ingrese el tiempo en ms por salto de cilindro: ";
                std::cin >> userTiempo;
                
                if (std::cin.fail()) {
                    std::cout << "[ERROR] Valores inválidos.\n";
                    limpiarCin();
                } else {
                    miDisk = Disk(userCilindros, userPosicion, userTiempo);
                    std::cout << "[OK] Parámetros del disco actualizados.\n";
                }
                break;
            }
            case 5:
                std::cout << "\n--- Solicitudes Actuales en Cola ---\n";
                printRequests(requests);
                break;
            case 6:
                if (!requests.empty()) {
                    std::cout << "\n>>> EJECUTANDO PLANIFICADOR FCFS <<<\n";
                    ejecutarFCFS(miDisk, requests);
                } else std::cout << "[INFO] Primero debes cargar solicitudes.\n";
                break;
            case 7:
                if (!requests.empty()) {
                    std::cout << "\n>>> EJECUTANDO PLANIFICADOR SSTF <<<\n";
                    ejecutarSSTF(miDisk, requests);
                } else std::cout << "[INFO] Primero debes cargar solicitudes.\n";
                break;
            case 8:
                if (!requests.empty()) {
                    int dir = 1;
                    std::cout << "[SCAN] Ingrese la dirección (1 para subir/derecha, 0 para bajar/izquierda): ";
                    std::cin >> dir;
                    if(std::cin.fail()){ limpiarCin(); dir=1; }
                    std::cout << "\n>>> EJECUTANDO PLANIFICADOR SCAN <<<\n";
                    ejecutarSCAN(miDisk, requests, dir);
                } else std::cout << "[INFO] Primero debes cargar solicitudes.\n";
                break;
            case 9:
                if (!requests.empty()) {
                    int dir = 1;
                    std::cout << "[C-SCAN] Ingrese la dirección (1 para subir/derecha, 0 para bajar/izquierda): ";
                    std::cin >> dir;
                    if(std::cin.fail()){ limpiarCin(); dir=1; }
                    std::cout << "\n>>> EJECUTANDO PLANIFICADOR C-SCAN <<<\n";
                    ejecutarCSCAN(miDisk, requests, dir);
                } else std::cout << "[INFO] Primero debes cargar solicitudes.\n";
                break;
            case 10:
                if (!requests.empty()) {
                    int dir = 1;
                    std::cout << "Se necesita la configuración del brazo.\nIngrese dirección para SCAN y C-SCAN (1 para subir, 0 para bajar): ";
                    std::cin >> dir;
                    if(std::cin.fail()){ limpiarCin(); dir=1; }
                    
                    std::cout << "\n============ EJECUTANDO TODO ===========\n";
                    int tFCFS = ejecutarFCFS(miDisk, requests);
                    int tSSTF = ejecutarSSTF(miDisk, requests);
                    int tSCAN = ejecutarSCAN(miDisk, requests, dir);
                    int tCSCAN = ejecutarCSCAN(miDisk, requests, dir);
                    
                    std::cout << "\n=========================================\n";
                    std::cout << "        TABLA COMPARATIVA FINAL          \n";
                    std::cout << "=========================================\n";
                    std::cout << "  FCFS   |  SSTF  |  SCAN  |  C-SCAN \n";
                    std::cout << "-----------------------------------------\n";
                    std::cout << " " << tFCFS << "ms\t | " << tSSTF << "ms | " << tSCAN << "ms | " << tCSCAN << "ms\n";
                    std::cout << "=========================================\n";
                    
                    std::ofstream tabla("results/comparacion_resultados.txt");
                    tabla << "=========================================\n";
                    tabla << "        TABLA COMPARATIVA FINAL          \n";
                    tabla << "=========================================\n";
                    tabla << " Algoritmo    |  Tiempo Total (ms)\n";
                    tabla << "-----------------------------------------\n";
                    tabla << " FCFS         | " << tFCFS << "\n";
                    tabla << " SSTF         | " << tSSTF << "\n";
                    tabla << " SCAN         | " << tSCAN << "\n";
                    tabla << " C-SCAN       | " << tCSCAN << "\n";
                    tabla << "=========================================\n";
                    tabla.close();
                    
                    std::cout << "\n[OK] Tabla guardada en results/comparacion_resultados.txt\n";
                } else std::cout << "[INFO] Primero debes cargar solicitudes.\n";
                break;
            case 0:
                salir = true;
                std::cout << "\n[FIN] Saliendo del simulador...\n";
                break;
            default:
                std::cout << "[ERROR] Opción no válida.\n";
                break;
        }
    }

    return 0;
}