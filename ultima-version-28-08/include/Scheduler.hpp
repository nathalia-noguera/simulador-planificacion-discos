#ifndef SCHEDULER_HPP
#define SCHEDULER_HPP

#include <vector>
#include <string>
#include "Disk.hpp"
#include "Request.hpp"

// ================== NUEVO: Estructura para las métricas del enunciado ==================
// Guarda: tiempos de búsqueda, latencia promedio y tiempos de acceso.
struct ResultadoAlgoritmo{
    std::string nombreAlgoritmo;
    int    tiempoTotalBusqueda;     // Suma de todos los tiempos de búsqueda (ms)
    double latenciaPromedio;        // Tiempo de búsqueda promedio por solicitud (ms)
    int    tiempoBusquedaMax;       // Mayor tiempo de búsqueda individual (ms)
    int    tiempoBusquedaMin;       // Menor tiempo de búsqueda individual (ms)
    double tiempoAccesoPromedio;    // Tiempo de acceso promedio acumulado (ms)
    int    numSolicitudesAtendidas;
};
// ==========================================================================================

// Función principal para First-Come, First-Served
ResultadoAlgoritmo ejecutarFCFS(Disk disco, std::vector<Request> solicitudes);

// Función principal para el algoritmo del ascensor (SCAN)
ResultadoAlgoritmo ejecutarSCAN(Disk disco, std::vector<Request> solicitudes, int direccion);

#endif