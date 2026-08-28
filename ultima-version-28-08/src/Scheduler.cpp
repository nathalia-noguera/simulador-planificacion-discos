#include "../include/Scheduler.hpp"
#include <iostream>
#include <fstream>
#include <cmath>
#include <algorithm>
#include <unordered_map>

// Algoritmo FCFS: atiende las solicitudes en el orden en que llegaron, es O(n).
ResultadoAlgoritmo ejecutarFCFS(Disk disco, std::vector<Request> solicitudes){
    ResultadoAlgoritmo resultado;
    resultado.nombreAlgoritmo = "FCFS";

    if(solicitudes.empty()){
        std::cout << "  La lista de solicitudes está vacía." << std::endl;
        resultado.tiempoTotalBusqueda = 0;
        resultado.latenciaPromedio = 0;
        resultado.tiempoBusquedaMax = 0;
        resultado.tiempoBusquedaMin = 0;
        resultado.tiempoAccesoPromedio = 0;
        resultado.numSolicitudesAtendidas = 0;
        return resultado;
    }

    int tiempoTotal = 0;
    int posicionBrazo = disco.getCurrentHeadPosition(); 

    // Variables para calcular las métricas de rendimiento.
    int tiempoBusquedaMax = 0;
    int tiempoBusquedaMin = -1;
    long long sumaTiemposAcceso = 0;
    int tiempoAccesoAcumulado = 0;
    
    std::ofstream archivo("results/fcfs_resultados.txt");
    
    std::cout << "  R e s u l t a d o s   A l g o r i t m o   F C F S\n" << std::endl;
    archivo << "\n  R e s u l t a d o s   A l g o r i t m o   F C F S\n\n";

    // Se recorren las solicitudes en el orden original y se mueve el brazo a cada una.
    for(size_t i = 0; i < solicitudes.size(); i++){
        
        int cilindroDestino = solicitudes[i].cylinder; 
        int pistaDestino = solicitudes[i].track;
        
        int distancia = std::abs(cilindroDestino - posicionBrazo);
        
        int tiempoMovimiento = distancia * disco.getTimePerCylinder(); 
        
        tiempoTotal = tiempoTotal + tiempoMovimiento;
        posicionBrazo = cilindroDestino;

        // Se actualizan las métricas con el resultado de este movimiento.
        if(tiempoMovimiento > tiempoBusquedaMax){
            tiempoBusquedaMax = tiempoMovimiento;
        }
        if(tiempoBusquedaMin == -1 || tiempoMovimiento < tiempoBusquedaMin){
            tiempoBusquedaMin = tiempoMovimiento;
        }

        // El tiempo de acceso es el tiempo acumulado desde el inicio hasta esta solicitud.
        tiempoAccesoAcumulado += tiempoMovimiento;
        sumaTiemposAcceso += tiempoAccesoAcumulado;

        std::cout << "  Visitando cilindro: " << cilindroDestino << ", Pista: " << pistaDestino << ", Distancia: " << distancia 
                  << ", Tiempo de búsqueda: " << tiempoMovimiento << " ms"
                  << ", Tiempo de acceso acumulado: " << tiempoAccesoAcumulado << " ms" << std::endl;
        archivo << "  Visitando cilindro: " << cilindroDestino << ", Pista: " << pistaDestino << ", Distancia: " << distancia 
                << ", Tiempo de búsqueda: " << tiempoMovimiento << " ms"
                << ", Tiempo de acceso acumulado: " << tiempoAccesoAcumulado << " ms\n";
    }

    int tiempoPromedio = tiempoTotal / solicitudes.size();
    double tiempoAccesoPromedio = (double) sumaTiemposAcceso / solicitudes.size();

    // Se muestran y guardan las métricas finales del algoritmo.
    std::cout << "\n\n  M é t r i c a s   d e   r e n d i m i e n t o   ( F C F S )\n\n";
    std::cout << "  Tiempo de búsqueda total:           " << tiempoTotal << " ms\n";
    std::cout << "  Latencia promedio (por solicitud):  " << tiempoPromedio << " ms\n";
    std::cout << "  Tiempo de búsqueda máximo:          " << tiempoBusquedaMax << " ms\n";
    std::cout << "  Tiempo de búsqueda mínimo:          " << tiempoBusquedaMin << " ms\n";
    std::cout << "  Tiempo de acceso promedio:          " << tiempoAccesoPromedio << " ms\n";

    archivo << "\n\n  M é t r i c a s   d e   r e n d i m i e n t o   ( F C F S )\n\n";
    archivo << "  Tiempo de búsqueda total:           " << tiempoTotal << " ms\n";
    archivo << "  Latencia promedio (por solicitud):  " << tiempoPromedio << " ms\n";
    archivo << "  Tiempo de búsqueda máximo:          " << tiempoBusquedaMax << " ms\n";
    archivo << "  Tiempo de búsqueda mínimo:          " << tiempoBusquedaMin << " ms\n";
    archivo << "  Tiempo de acceso promedio:          " << tiempoAccesoPromedio << " ms\n";
    
    archivo.close();

    resultado.tiempoTotalBusqueda = tiempoTotal;
    resultado.latenciaPromedio = tiempoPromedio;
    resultado.tiempoBusquedaMax = tiempoBusquedaMax;
    resultado.tiempoBusquedaMin = tiempoBusquedaMin;
    resultado.tiempoAccesoPromedio = tiempoAccesoPromedio;
    resultado.numSolicitudesAtendidas = solicitudes.size();

    return resultado;
}

// SCAN se mueve en una dirección atendiendo solicitudes hasta llegar al extremo y luego regresa.
// LOOK en cambio solo llega hasta la última solicitud real, sin llegar hasta el extremo del disco.
// Este algoritmo es O(n log n) por el ordenamiento inicial de las solicitudes.
ResultadoAlgoritmo ejecutarSCAN(Disk disco, std::vector<Request> solicitudes, int direccion){
    ResultadoAlgoritmo resultado;
    resultado.nombreAlgoritmo = "SCAN";

    if(solicitudes.empty()){
        std::cout << "  La lista de solicitudes está vacía." << std::endl;
        resultado.tiempoTotalBusqueda = 0;
        resultado.latenciaPromedio = 0;
        resultado.tiempoBusquedaMax = 0;
        resultado.tiempoBusquedaMin = 0;
        resultado.tiempoAccesoPromedio = 0;
        resultado.numSolicitudesAtendidas = 0;
        return resultado;
    }

    int tiempoTotal = 0;
    int posicionBrazo = disco.getCurrentHeadPosition();
    int totalCilindros = disco.getTotalCylinders();

    // Se ordenan las solicitudes por cilindro para poder separarlas por dirección.
    std::sort(solicitudes.begin(), solicitudes.end(), [](const Request& a, const Request& b){
        return a.cylinder < b.cylinder;
    });

    // Guarda la pista asociada a cada cilindro para mostrarla en el recorrido.
    std::unordered_map<int, int> cilindroAPista;

    // Se separan las solicitudes en dos grupos según estén antes o después del brazo.
    std::vector<int> izquierda, derecha;
    for(size_t i = 0; i < solicitudes.size(); i++){
        cilindroAPista[solicitudes[i].cylinder] = solicitudes[i].track;

        if(solicitudes[i].cylinder < posicionBrazo){
            izquierda.push_back(solicitudes[i].cylinder);
        }else{
            if(solicitudes[i].cylinder > posicionBrazo){
                derecha.push_back(solicitudes[i].cylinder);
            }else{
                std::cout << "  [Ok] Solicitud en la misma posición inicial (" << posicionBrazo << ") atendida de inmediato (0 ms).\n\n";
            }
        } 
    }

    std::ofstream archivo("results/scan_resultados.txt");
    std::cout << "  R e s u l t a d o s   A l g o r i t m o   S C A N\n" << std::endl;
    archivo << "\n  R e s u l t a d o s   A l g o r i t m o   S C A N\n\n";

    std::vector<int> ordenRecorrido;

    // Variables para calcular las métricas de rendimiento.
    int tiempoBusquedaMax = 0;
    int tiempoBusquedaMin = -1;
    long long sumaTiemposAcceso = 0;
    int tiempoAccesoAcumulado = 0;
    int solicitudesReales = 0;
    
    // Mueve el brazo a un cilindro y registra el movimiento; esSolicitud indica si cuenta para las métricas.
    auto moverBrazo = [&](int cilindroDestino, std::string nota, bool esSolicitud){
        int distancia = std::abs(cilindroDestino - posicionBrazo);
        int tiempo = distancia * disco.getTimePerCylinder();
        tiempoTotal += tiempo;
        posicionBrazo = cilindroDestino;

        tiempoAccesoAcumulado += tiempo;

        std::string pistaTexto;
        if(esSolicitud){
            pistaTexto = std::to_string(cilindroAPista[cilindroDestino]);
        }else{
            pistaTexto = "--";
        }

        if(esSolicitud){
            if (tiempo > tiempoBusquedaMax) tiempoBusquedaMax = tiempo;
            if (tiempoBusquedaMin == -1 || tiempo < tiempoBusquedaMin) tiempoBusquedaMin = tiempo;
            sumaTiemposAcceso += tiempoAccesoAcumulado;
            solicitudesReales++;
        }
        
        std::cout << "  [v] " << nota << cilindroDestino 
                  << ", Pista: " << pistaTexto
                  << ", Distancia: " << distancia 
                  << ", Suma tiempo: " << tiempo << " ms\n";
        archivo << "  Visitando: " << cilindroDestino << ", Pista: " << pistaTexto << ", Distancia: " << distancia << "\n";
    };

    // Según la dirección inicial, se atiende un lado, se toca el extremo si hace falta y se regresa por el otro lado.
    if(direccion == 1){
        for(int cil : derecha){
            moverBrazo(cil, "Atendido obj. en subida -> ", true);
        }
        if(!izquierda.empty()){
            moverBrazo(totalCilindros - 1, "Tocando el extremo (Final) -> ", false);
            for(int j = izquierda.size() - 1; j >= 0; j--){
                moverBrazo(izquierda[j], "Atendido obj. en bajada -> ", true);
            }
        }
    }else{
        for(int k = izquierda.size() - 1; k >= 0; k--){
            moverBrazo(izquierda[k], "Atendido obj. en bajada -> ", true);
        }
        if(!derecha.empty()){
            moverBrazo(0, "Tocando el extremo (Inicio) -> ", false);
            for(int cilDerecha : derecha){
                moverBrazo(cilDerecha, "Atendido obj. en subida -> ", true);
            }
        }
    }

    int tiempoPromedio = tiempoTotal / solicitudes.size();
    double tiempoAccesoPromedio;
    if(solicitudesReales > 0){
        tiempoAccesoPromedio = (double) sumaTiemposAcceso / solicitudesReales;
    }else{
        tiempoAccesoPromedio = 0;
    }

    // Se muestran y guardan las métricas finales del algoritmo.
    std::cout << "\n\n  M é t r i c a s   d e   r e n d i m i e n t o   ( S C A N )\n\n";
    std::cout << "  Tiempo de búsqueda total:           " << tiempoTotal << " ms\n";
    std::cout << "  Latencia promedio (por solicitud):  " << tiempoPromedio << " ms\n";
    std::cout << "  Tiempo de búsqueda máximo:          " << tiempoBusquedaMax << " ms\n";
    std::cout << "  Tiempo de búsqueda mínimo:          " << tiempoBusquedaMin << " ms\n";
    std::cout << "  Tiempo de acceso promedio:          " << tiempoAccesoPromedio << " ms\n";

    archivo << "\n\n  M é t r i c a s   d e   r e n d i m i e n t o   ( S C A N )\n\n";
    archivo << "  Tiempo de búsqueda total:           " << tiempoTotal << " ms\n";
    archivo << "  Latencia promedio (por solicitud):  " << tiempoPromedio << " ms\n";
    archivo << "  Tiempo de búsqueda máximo:          " << tiempoBusquedaMax << " ms\n";
    archivo << "  Tiempo de búsqueda mínimo:          " << tiempoBusquedaMin << " ms\n";
    archivo << "  Tiempo de acceso promedio:          " << tiempoAccesoPromedio << " ms\n";
    
    archivo.close();

    resultado.tiempoTotalBusqueda = tiempoTotal;
    resultado.latenciaPromedio = tiempoPromedio;
    resultado.tiempoBusquedaMax = tiempoBusquedaMax;
    resultado.tiempoBusquedaMin = tiempoBusquedaMin;
    resultado.tiempoAccesoPromedio = tiempoAccesoPromedio;
    resultado.numSolicitudesAtendidas = solicitudesReales;

    return resultado;
}