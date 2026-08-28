#include "../include/Scheduler.hpp"
#include <iostream>
#include <iomanip>      // justificar a la izq caracteres
#include <fstream>
#include <string>
#include <limits>
#include "../include/Disk.hpp"
#include "../include/Request.hpp"

#ifdef _WIN32 
    #include <windows.h>
#endif

// Limpia el estado de cin en caso de error de entrada.
void limpiarCin(){
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// Limpia la pantalla de la consola.
void limpiarPantalla(){
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

// Pausa la ejecución hasta que el usuario presione ENTER.
void pausar(){
    std::cout << "\n  Presione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}

// Pide la dirección para SCAN y valida que sea 0 o 1, rechazando letras y otros números.
int pedirDireccionSCAN(const std::string& mensaje){
    int dir;
    bool direccionValida = false;
    while(!direccionValida){
        std::cout << mensaje;
        std::cin >> dir;
        if(std::cin.fail()){
            std::cout << "\n  [ERROR] Debe ingresar un número válido.\n\n";
            limpiarCin();
        }else{
            if(dir != 0 && dir != 1){
                std::cout << "\n  [ERROR] La dirección debe ser 0 o 1.\n\n";
            }else{
                direccionValida = true;
            }
        }
    }
    return dir;
}

int main(){
    #ifdef _WIN32
        SetConsoleOutputCP(CP_UTF8);
    #endif

    // Se inicializa el disco con valores por defecto, incluyendo las pistas por cilindro.
    Disk miDisk(200, 50, 1.0, 4);
    std::vector<Request> requests;

    bool salir = false;
    bool enMenuSolicitudes = false;

    while(!salir){
        limpiarPantalla();

        std::cout << "-------------------------------------------------------------------------------\n";
        std::cout << "\n";
        std::cout << "  S i m u l a d o r   d e   P l a n i f i c a c i ó n\n\n";
        std::cout << "  Algoritmos de Planificación de Discos (FCFS y SCAN)\n";
        std::cout << "  Arquitectura del Computador\n";
        std::cout << "\n";

        int opcion;

        if(!enMenuSolicitudes){
            // Menú de configuración del disco y de solicitudes.
            std::cout << "-------------------------------------------------------------------------------\n";
            std::cout << "\n";
            std::cout << "  D i s c o   y   S o l i c i t u d e s\n";
            std::cout << "\n";
            std::cout << "  [INFO] Configuración actual del disco:\n\n";
            miDisk.printStatus();
            std::cout << "\n";
            std::cout << "  [INFO] Solicitudes cargadas actualmente: " << requests.size() << "\n";
            std::cout << "\n";
            std::cout << "-------------------------------------------------------------------------------\n";
            std::cout << "\n";
            std::cout << "   ( 1 ) Cargar solicitudes desde archivo.\n";
            std::cout << "   ( 2 ) Generar solicitudes al azar.\n";
            std::cout << "   ( 3 ) Ingresar solicitudes a mano.\n";
            std::cout << "   ( 4 ) Cambiar configuración del disco duro.\n";
            std::cout << "   ( 0 ) Salir del programa.\n";
            std::cout << "\n";
            std::cout << "-------------------------------------------------------------------------------\n";
            std::cout << "\n";
            std::cout << "  Opción: ";

            if(!(std::cin >> opcion)){
                std::cout << "\n  [ERROR] Entrada inválida. Por favor, introduzca un número.\n";
                limpiarCin();
                pausar();
                continue;
            }

            std::cout << "\n";

            switch(opcion){
                case 1:{
                    std::string nombreArchivo;
                    std::cout << "  Nombre del archivo (ej. data/requests.txt): ";
                    std::cin >> nombreArchivo;
                    requests = loadRequestsFromFile(nombreArchivo, miDisk.getTotalCylinders(), miDisk.getTracksPerCylinder());
                    
                    if(requests.empty()){
                        std::cout << "\n  [ADVERTENCIA] No se cargaron solicitudes (archivo no existe o vacío).\n";
                    }else{
                        std::cout << "\n  [OK] Solicitudes cargadas.\n";
                        enMenuSolicitudes = true;
                    }
                    pausar();
                    break;
                }
                case 2:{
                    int cantidad;
                    std::cout << "  ¿Cuántas solicitudes se van a generar aleatoriamente?: ";
                    if(std::cin >> cantidad && cantidad > 0){
                        requests = generateRandomRequests(cantidad, miDisk.getTotalCylinders(), miDisk.getTracksPerCylinder());
                        std::cout << "\n  [OK] Solicitudes generadas exitosamente.\n";
                        enMenuSolicitudes = true;
                    }else{
                        std::cout << "\n  [ERROR] Cantidad inválida.\n";
                        limpiarCin();
                    }
                    pausar();
                    break;
                }
                case 3:
                    requests = inputRequestsManually(miDisk.getTotalCylinders(), miDisk.getTracksPerCylinder());
                    if(!requests.empty()){
                        enMenuSolicitudes = true;
                    }
                    pausar();
                    break;
                case 4:{
                    int userCilindros, userPosicion, userPistas;
                    double userTiempo;
                    std::cout << "-------------------------------------------------------------------------------\n";
                    std::cout << "\n";
                    std::cout << "  C o n f i g u r a n d o   e l   D i s c o   D u r o\n";
                    std::cout << "\n";
                    std::cout << "-------------------------------------------------------------------------------\n";
                    std::cout << "\n";
                    std::cout << "  Ingrese el número total de cilindros: ";
                    std::cin >> userCilindros;
                    std::cout << "  Ingrese la posición inicial del cabezal: ";
                    std::cin >> userPosicion;
                    std::cout << "  Ingrese el tiempo en ms por salto de cilindro: ";
                    std::cin >> userTiempo;
                    std::cout << "  Ingrese el número de pistas por cilindro: ";
                    std::cin >> userPistas;
                    
                    if(std::cin.fail()){
                        std::cout << "\n  [ERROR] Valores inválidos. Debe ingresar solo números.\n";
                        limpiarCin();
                    }else{
                        // Se valida cada parámetro del disco antes de aplicar los cambios.
                        if(userCilindros <= 0){
                            std::cout << "\n  [ERROR] El número de cilindros debe ser mayor que 0.\n";
                        }else{
                            if(userPistas <= 0){
                                std::cout << "\n  [ERROR] El número de pistas debe ser mayor que 0.\n";
                            }else{
                                if(userTiempo < 0){
                                    std::cout << "\n  [ERROR] El tiempo por cilindro no puede ser negativo.\n";
                                }else{
                                    if(userPosicion < 0 || userPosicion >= userCilindros){
                                        std::cout << "\n  [ERROR] La posición inicial debe estar entre 0 y " << (userCilindros - 1) << ".\n";
                                    }else{
                                        miDisk = Disk(userCilindros, userPosicion, userTiempo, userPistas);
                                        std::cout << "  [OK] Parámetros del disco actualizados.\n";
                                    }
                                }
                            }
                        }
                    }
                    pausar();
                    break;
                }
                case 0:
                    salir = true;
                    std::cout << "  [FIN] Saliendo del simulador...\n";
                    break;
                default:
                    std::cout << "  [ERROR] Opción no válida.\n";
                    pausar();
                    break;
            }
        }else{
            // Menú de ejecución de los algoritmos de planificación.
            std::cout << "-------------------------------------------------------------------------------\n";
            std::cout << "\n";
            std::cout << "  E j e c u c i ó n   d e   A l g o r i t m o s\n";
            std::cout << "\n";
            std::cout << "\n";
            std::cout << "  [INFO] Solicitudes cargadas actualmente: " << requests.size() << "\n";
            std::cout << "\n";
            std::cout << "-------------------------------------------------------------------------------\n";
            std::cout << "\n";
            std::cout << "   ( 1 ) Ver lista de solicitudes actual.\n";
            std::cout << "   ( 2 ) Ejecutar FCFS.\n";
            std::cout << "   ( 3 ) Ejecutar SCAN.\n";
            std::cout << "   ( 4 ) Ejecutar ambos y crear una tabla comparativa.\n";
            std::cout << "   ( 5 ) Volver (cambiar solicitudes o configuración del disco).\n";
            std::cout << "   ( 0 ) Salir del programa.\n";
            std::cout << "\n";
            std::cout << "-------------------------------------------------------------------------------\n";
            std::cout << "\n";
            std::cout << "  Opción: ";

            if(!(std::cin >> opcion)){
                std::cout << "\n  [ERROR] Entrada inválida. Por favor, introduzca un número.\n";
                limpiarCin();
                pausar();
                continue;
            }

            std::cout << "\n";

            switch(opcion){
                case 1:
                    std::cout << "-------------------------------------------------------------------------------\n";
                    std::cout << "\n";
                    std::cout << "  S o l i c i t u d e s   A c t u a l e s   e n   C o l a\n";
                    std::cout << "\n";
                    printRequests(requests);
                    pausar();
                    break;
                case 2:
                    if(!requests.empty()){
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "\n";
                        std::cout << "  E j e c u t a n d o   P l a n i f i c a d o r   F C F S\n";
                        std::cout << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "\n";
                        ejecutarFCFS(miDisk, requests);
                    }else{
                        std::cout << "\n  [INFO] Primero debe cargar solicitudes.\n";
                    }
                    pausar();
                    break;
                case 3:
                    if(!requests.empty()){
                        int dir = pedirDireccionSCAN("  [SCAN] Ingrese la dirección (1 para subir/derecha, 0 para bajar/izquierda): ");
                        std::cout << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "\n";
                        std::cout << "  E j e c u t a n d o   P l a n i f i c a d o r   S C A N\n";
                        std::cout << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "\n";
                        ejecutarSCAN(miDisk, requests, dir);
                    }else{
                        std::cout << "\n  [INFO] Primero debe cargar solicitudes.\n";
                    }
                    pausar();
                    break;
                case 4:
                    if(!requests.empty()){
                        int dir = pedirDireccionSCAN("  Se necesita la configuración del brazo.\n\n  Ingrese dirección para SCAN (1 para subir, 0 para bajar): ");
                        
                        std::cout << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "\n";
                        std::cout << "  E j e c u t a n d o   A m b o s   A l g o r i t m o s\n";
                        std::cout << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "\n";
                        ResultadoAlgoritmo resFCFS = ejecutarFCFS(miDisk, requests);
                        std::cout << "\n-------------------------------------------------------------------------------\n\n";
                        ResultadoAlgoritmo resSCAN = ejecutarSCAN(miDisk, requests, dir);

                        // Se muestra una tabla comparando las métricas de ambos algoritmos.
                        std::cout << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "\n";
                        std::cout << "  M é t r i c a s   d e   R e n d i m i e n t o   ( F C F S   /   S C A N )\n";
                        std::cout << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        std::cout << "  Métrica                         |    FCFS    |    SCAN";
                        std::cout << "\n-------------------------------------------------------------------------------\n";
                        std::cout << "  Tiempo de búsqueda total (ms)   | " << std::left << std::setw(11) << resFCFS.tiempoTotalBusqueda << "| " << resSCAN.tiempoTotalBusqueda << "\n";
                        std::cout << "  Latencia promedio (ms)          | " << std::left << std::setw(11) << resFCFS.latenciaPromedio << "| " << resSCAN.latenciaPromedio << "\n";
                        std::cout << "  Tiempo de búsqueda máximo (ms)  | " << std::left << std::setw(11) << resFCFS.tiempoBusquedaMax << "| " << resSCAN.tiempoBusquedaMax << "\n";
                        std::cout << "  Tiempo de búsqueda mínimo (ms)  | " << std::left << std::setw(11) << resFCFS.tiempoBusquedaMin << "| " << resSCAN.tiempoBusquedaMin << "\n";
                        std::cout << "  Tiempo de acceso promedio (ms)  | " << std::left << std::setw(11) << resFCFS.tiempoAccesoPromedio << "| " << resSCAN.tiempoAccesoPromedio << "\n";
                        std::cout << "-------------------------------------------------------------------------------\n";
                        
                        std::ofstream tabla("results/comparacion_resultados.txt");

                        // Se guarda la misma tabla comparativa en un archivo de resultados.
                        tabla << "\n";
                        tabla << "-------------------------------------------------------------------------------\n";
                        tabla << "\n";
                        tabla << "  M é t r i c a s   d e   R e n d i m i e n t o   ( F C F S   /   S C A N )\n";
                        tabla << "\n";
                        tabla << "-------------------------------------------------------------------------------\n";
                        tabla << "  Métrica                         |    FCFS    |    SCAN\n";
                        tabla << "-------------------------------------------------------------------------------\n";
                        tabla << "  Tiempo de búsqueda total (ms)   | " << std::left << std::setw(11) << resFCFS.tiempoTotalBusqueda << "| " << resSCAN.tiempoTotalBusqueda << "\n";
                        tabla << "  Latencia promedio (ms)          | " << std::left << std::setw(11) << resFCFS.latenciaPromedio << "| " << resSCAN.latenciaPromedio << "\n";
                        tabla << "  Tiempo de búsqueda máximo (ms)  | " << std::left << std::setw(11) << resFCFS.tiempoBusquedaMax << "| " << resSCAN.tiempoBusquedaMax << "\n";
                        tabla << "  Tiempo de búsqueda mínimo (ms)  | " << std::left << std::setw(11) << resFCFS.tiempoBusquedaMin << "| " << resSCAN.tiempoBusquedaMin << "\n";
                        tabla << "  Tiempo de acceso promedio (ms)  | " << std::left << std::setw(11) << resFCFS.tiempoAccesoPromedio << "| " << resSCAN.tiempoAccesoPromedio << "\n";
                        tabla << "-------------------------------------------------------------------------------\n";
                        tabla.close();
                        
                        std::cout << "\n  [OK] Tabla guardada en results/comparacion_resultados.txt\n";
                    }else{
                        std::cout << "\n  [INFO] Primero debe cargar solicitudes.\n";
                    }
                    pausar();
                    break;
                case 5:
                    enMenuSolicitudes = false;
                    break;
                case 0:
                    salir = true;
                    std::cout << "  [FIN] Saliendo del simulador...\n";
                    break;
                default:
                    std::cout << "  [ERROR] Opción no válida.\n";
                    pausar();
                    break;
            }
        }
    }

    return 0;
}